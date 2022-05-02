#include "fat_driver.h"

uint32 fat_last_cluster(Fat_infos *infos)
{
    uint32_t last_cluster;
    switch (infos->fat_type)
    {
    case 32:
        last_cluster=0xFFFFFF8;
        break;
    case 16:
        last_cluster=0xFFF8;
        break;
    default:
        last_cluster=0xFF8;
        break;
    }
    return last_cluster;
}

Ata_fat_system::Ata_fat_system(Drive d)
:m_drive(d)
{
    m_id=ata_identify(d);
    if(!is_ready())
    {
        err("erreur lors de l'initialisation du disque :\n");
        if(!m_id.exists) err("le disque n'existe pas\n");
    }
}

bool Ata_fat_system::read(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size)
{
    if(sector_size%m_id.taille_secteur!=0||sector_size==0)
        err("taille de secteur non compatible\n");
    uint8_t logical_per_ph=sector_size/m_id.taille_secteur;
    for(int i=0;i<10;i++)
    {
        if(ata_read(m_drive,count*logical_per_ph,addr*logical_per_ph,(uint16_t*)buffer))
            return true;
    }
    return false;
}

bool Ata_fat_system::write(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size)
{
    if(sector_size%m_id.taille_secteur!=0||sector_size==0)
        err("taille de secteur non compatible\n");
    uint8_t logical_per_ph=sector_size/m_id.taille_secteur;
    return ata_write(m_drive,count*logical_per_ph,addr*logical_per_ph,(uint16_t*)buffer);
}

void Ata_fat_system::err(char* msg)
{
    print_string(msg);
}

uint32_t inline max(uint32_t a,uint32_t b)
{
    return (a<b)?b:a;
}

uint32_t inline min(uint32_t a,uint32_t b)
{
    return (a<b)?a:b;
}

uint16_t inline buff_8_16(uint8_t* buff,const uint32_t i)
{
    return *(uint16_t*)(&buff[i]);        
}

uint32_t inline buff_8_32(uint8_t* buff,const uint32_t i)
{
    return *(uint32_t*)(&buff[i]);        
}

Fat_infos fat_init(Fat_system* intf)
{
    Fat_infos ret;
    ret.fat_type=0;

    uint8_t BPB[512];
    if(!intf->read(1,0,BPB,intf->get_sectorsize()))
    {
        intf->err("erreur de lecture");
        return Fat_infos();
    }

    ret.sector_size=buff_8_16(BPB,0x0B);//sector_size
    ret.cluster_size=BPB[0x0D];

    uint32_t nb_sectors=buff_8_16(BPB,0x13);
    if(nb_sectors==0)
    {
        ret.fat_type=32;
        nb_sectors=buff_8_32(BPB,0x20);
    }

    uint16_t reserved=buff_8_16(BPB,0xE);
    ret.fat=(uint32_t)reserved;//fat

    if((ret.fat_size=buff_8_16(BPB,0x16))==0)//fat_size
    {
        ret.fat_type=32;
        ret.fat_size=buff_8_32(BPB,0x24);
    }

    ret.root=ret.fat+ret.fat_size*(uint32_t)(BPB[0x10]);//root

    ret.root_size=32*(uint32_t)buff_8_16(BPB,0x11)/(uint32_t)ret.sector_size;//root_size
    ret.data=ret.root+ret.root_size;//data

    if(ret.root_size==0)
    {
        ret.fat_type=32;
        ret.root=buff_8_32(BPB,0X2C);//root for FAT32
    }

    ret.data_size=nb_sectors-ret.data;//data_size

    uint32_t nb_clusters=ret.data_size/ret.cluster_size;
    if(nb_clusters<4085&&ret.fat_type==0)
    {
        ret.fat_type=12;
    }
    else if(nb_clusters<65525&&ret.fat_type==0)
    {
        ret.fat_type=16;
    }
    else
    {
        ret.fat_type=32;
    }

    ret.byte_per_cluster=ret.sector_size*ret.cluster_size;

    ret.root_fat_entry=Fat_entry((ret.fat_type==32)?ret.root:0,true,(char*)(BPB+0x2B),ret.root_size,0,0);

    return ret;
}

uint32_t fat_next_cluster(uint32_t current_cluster,Fat_system* intf,Fat_infos* infos)
{
    uint8_t byte_per_entry=(infos->fat_type==12)?3:(infos->fat_type/8);
    uint32_t entry_no=(infos->fat_type==12)?current_cluster/2:current_cluster;
    uint32_t sector_offset=(entry_no*byte_per_entry)/(uint32_t)infos->sector_size;
    uint16_t entry_offset=entry_no%(infos->sector_size/(uint16_t)byte_per_entry);
    
    uint8_t buffer[infos->sector_size*2];
    intf->read(2,sector_offset+infos->fat,buffer,infos->sector_size);

    uint32_t entry_val(0);
    for(int i=0;i<byte_per_entry;i++)
    {
        entry_val|=buffer[entry_offset*byte_per_entry+i]<<(i*8);
    }

    uint32_t mask(0);
    uint8_t shift(0);
    switch (infos->fat_type)
    {
    case 12:
        if(current_cluster%2==0)
        {
            mask=0xFFF;
        }
        else
        {
            mask=0xFFF000;
            shift=12;
        }
        break;
    case 16:
        mask=0xFFFF;
        break;    
    default:
        mask=0xFFFFFFF;
        break;
    }

    return (entry_val&mask)>>shift;
}

uint32 fat_free_cluster(Fat_system* intf,Fat_infos* infos)
{
    uint32 current_cluster;
    uint8_t byte_per_entry=(infos->fat_type==12)?3:(infos->fat_type/8);
    uint8_t buffer[infos->sector_size*2];
    bool empty_buffer=true;
    uint32 current_sector=0;
    for(current_cluster=2;;current_cluster++)
    {
        uint32_t entry_no=(infos->fat_type==12)?current_cluster/2:current_cluster;
        uint32_t sector_offset=(entry_no*byte_per_entry)/(uint32_t)infos->sector_size;
        uint16_t entry_offset=entry_no%(infos->sector_size/(uint16_t)byte_per_entry);
        if(sector_offset>=infos->fat_size)
        {
            return fat_last_cluster(infos);
        }
        
        if((current_sector!=sector_offset)||empty_buffer)
        {
            empty_buffer=false;
            current_sector=sector_offset;
            intf->read(2,sector_offset+infos->fat,buffer,infos->sector_size);
        }
    
        uint32_t entry_val(0);
        for(int i=0;i<byte_per_entry;i++)
        {
            entry_val|=buffer[entry_offset*byte_per_entry+i]<<(i*8);
        }
    
        uint32_t mask(0);
        uint8_t shift(0);
        switch (infos->fat_type)
        {
        case 12:
            if(current_cluster%2==0)
            {
                mask=0xFFF;
            }
            else
            {
                mask=0xFFF000;
                shift=12;
            }
            break;
        case 16:
            mask=0xFFFF;
            break;    
        default:
            mask=0xFFFFFFF;
            break;
        }
        if((entry_val&mask)>>shift==0)
        {
            return current_cluster;
        }
    }
}

bool set_fat_cluster(uint32 cluster,uint32 val,Fat_system* intf,Fat_infos* infos)
{
    uint8_t byte_per_entry=(infos->fat_type==12)?3:(infos->fat_type/8);
    uint32_t entry_no=(infos->fat_type==12)?cluster/2:cluster;
    uint32_t sector_offset=(entry_no*byte_per_entry)/(uint32_t)infos->sector_size;
    uint16_t entry_offset=entry_no%(infos->sector_size/(uint16_t)byte_per_entry);
    if(sector_offset>infos->fat_size)
        return false;
    
    uint8_t buffer[infos->sector_size*2];
    intf->read(2,sector_offset+infos->fat,buffer,infos->sector_size);

    uint32_t entry_val(0);
    for(int i=0;i<byte_per_entry;i++)
    {
        entry_val|=buffer[entry_offset*byte_per_entry+i]<<(i*8);
    }

    uint32_t mask(0);
    uint8_t shift(0);
    switch (infos->fat_type)
    {
    case 12:
        if(cluster%2==0)
        {
            mask=0xFFF;
        }
        else
        {
            mask=0xFFF000;
            shift=12;
        }
        break;
    case 16:
        mask=0xFFFF;
        break;    
    default:
        mask=0xFFFFFFF;
        break;
    }

    entry_val=(entry_val&(~mask))|((val<<shift)&mask);

    for(int i=0;i<byte_per_entry;i++)
    {
        buffer[entry_offset*byte_per_entry+i]=(entry_val&(0xFF<<(i*8)))>>(i*8);
    }
    intf->write(2,sector_offset+infos->fat,buffer,infos->sector_size);
    return true;
}

uint32_t Fat_entry::read_data(uint8_t *buffer,uint32_t cluster_count,Fat_infos* infos,Fat_system* intf)
{
    if(m_is_directory)
        return 0;

    uint32 last_cluster=fat_last_cluster(infos);
    
    if(m_current_cluster>=last_cluster)
        return 0;

    uint32_t ret(0);
    for(uint32_t i=0;i<cluster_count;i++)
    {
        intf->read(infos->cluster_size,
            infos->data+(m_current_cluster-2)*infos->cluster_size,
            buffer+i*infos->byte_per_cluster,
            infos->sector_size);

        m_current_cluster=fat_next_cluster(m_current_cluster,intf,infos);

        if(m_current_cluster>=last_cluster)
            return i*infos->byte_per_cluster+m_size%infos->byte_per_cluster;
    }

    return cluster_count*infos->byte_per_cluster;
}

uint32_t Fat_entry::read_entries(Fat_entry* buffer,uint32_t size,Fat_infos* infos,Fat_system* intf)
{
    if((!m_is_directory)||m_last_entry)
        return 0;
    
    uint32_t read(0);

    uint32_t buff_size=(m_first_cluster==0)?infos->sector_size:infos->byte_per_cluster;
    uint8_t buff[buff_size];
    uint16_t entry_per_buff=buff_size/32;
    bool buff_empty(true);
    uint32 buff_addr;

    
    uint32_t last_cluster=fat_last_cluster(infos);

    while(read<size)
    {
        if(m_current_entry_offset>=entry_per_buff)//update cluster si on peut
        {
            m_current_entry_offset=0;
            if(m_first_cluster==0)
            {
                if(m_current_cluster==infos->root_size-1)
                {
                    m_current_entry_offset=entry_per_buff;
                    m_last_entry=true;
                    break;
                }
                else
                    m_current_cluster++;
            }
            else
            {
                uint32_t next_cluster=fat_next_cluster(m_current_cluster,intf,infos);
                if(next_cluster>=last_cluster)
                {
                    m_current_entry_offset=entry_per_buff;
                    m_last_entry=true;
                    break;
                }
                else
                    m_current_cluster=next_cluster;
            }
            
        }

        if(m_current_entry_offset==0||buff_empty)//recharger de la memoire
        {
            if(m_first_cluster==0)
            {
                intf->read(1,infos->root+m_current_cluster,buff,infos->sector_size);
                buff_addr=infos->root+m_current_cluster;
            }
            else
            {
                intf->read(infos->cluster_size,
                    infos->data+(m_current_cluster-2)*infos->cluster_size,
                    buff,
                    infos->sector_size);
                buff_addr=infos->data+(m_current_cluster-2)*infos->cluster_size;
            }
            buff_empty=false;
        }

        if(buff[32*m_current_entry_offset]==0)
        {
            m_last_entry=true;
            break;
        }
        if(buff[32*m_current_entry_offset]==0x05
            ||buff[32*m_current_entry_offset]==0xE5
            ||(buff[32*m_current_entry_offset+0x0B]&0x8)==0x8)
        {
            m_current_entry_offset++;
            continue;
        }

        bool is_directory=(buff[32*m_current_entry_offset+0x0B]&0x10)==0x10;
        uint32_t first_cluster=buff_8_16(buff,32*m_current_entry_offset+0x1A);
        if(infos->fat_type==32)
        {
            first_cluster=first_cluster|(buff_8_16(buff,32*m_current_entry_offset+0x14)>>16);
        }
        uint32_t size=buff_8_32(buff,32*m_current_entry_offset+0X1C);
        buffer[read]=Fat_entry(first_cluster,is_directory,(char*)(buff+32*m_current_entry_offset),size,
            buff_addr+(m_current_entry_offset*32)/((uint32)infos->sector_size),(m_current_entry_offset*32)%((uint32)infos->sector_size));
        read++;
        m_current_entry_offset++;
    }

    return read;
}

Fat_entry open_file(char* name,Ata_fat_system *afs,Fat_infos* infos,Fat_entry *dir,bool* ok)
{
    Fat_entry entries[10];
    int i;
    int nb_entries;
    dir->init_offset();
    while(1)
    {
        int nb_entry=dir->read_entries(entries,10,infos,afs);
        if(nb_entry==0)
        {
            *ok=false;
            return Fat_entry();
        }
        for(i=0;i<nb_entry;i++)
        {
            if(strcmp(entries[i].get_name(),name))
            {
                *ok=true;
                return entries[i];
            }
        }
    }
}


bool Fat_entry::write_data(uint8* buffer,uint32 size,Fat_infos* infos,Fat_system* intf)
{
    if(m_is_directory)
        return false;
    init_offset();
    uint32 last_cluster=fat_last_cluster(infos);
    uint32 next_cluster=m_current_cluster;
    while(next_cluster<last_cluster)
    {
        m_current_cluster=next_cluster;
        next_cluster=fat_next_cluster(next_cluster,intf,infos);
    }
    uint32 offset=m_size%infos->byte_per_cluster;
    uint32 write_size(0);
    uint8 buffer_cluster[infos->byte_per_cluster];
    bool empty_buffer(true);
    while(write_size<size)
    {
        if(offset==0)
        {
            next_cluster=fat_free_cluster(intf,infos);
            if(next_cluster>=last_cluster)
                return false;
            if(!set_fat_cluster(next_cluster,0xFFFFFFFF,intf,infos))
                return false;
            if(!set_fat_cluster(m_current_cluster,next_cluster,intf,infos))
                return false;
            m_current_cluster=next_cluster;
        }
        if(offset==0||empty_buffer)
        {
            empty_buffer=false;
            intf->read(infos->cluster_size,
                infos->data+(m_current_cluster-2)*infos->cluster_size,
                buffer_cluster,
                infos->sector_size);
        }
        buffer_cluster[offset]=buffer[write_size];
        write_size++;
        offset++;
        m_size++;
        if(offset==infos->byte_per_cluster||write_size==size)
        {
            offset=0;
            intf->write(infos->cluster_size,
                infos->data+(m_current_cluster-2)*infos->cluster_size,
                buffer_cluster,
                infos->sector_size);
        }
    }
    
    if(!intf->read(1,m_entry_sector,buffer_cluster,infos->sector_size))
        return false;
    *((uint32*)&buffer_cluster[m_entry_offset+0X1C])=m_size;
    if(!intf->write(1,m_entry_sector,buffer_cluster,infos->sector_size))
        return false;
    

    return true;
}