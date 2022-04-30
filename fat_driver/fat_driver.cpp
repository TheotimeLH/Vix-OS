#include "fat_driver.h"

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

    ret.root_fat_entry=Fat_entry((ret.fat_type==32)?ret.root:0,true,(char*)(BPB+0x2B),ret.root_size);

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

uint32_t Fat_entry::read_data(uint8_t *buffer,uint32_t cluster_count,Fat_infos* infos,Fat_system* intf)
{
    if(m_is_directory)
        return 0;

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
            }
            else
            {
                intf->read(infos->cluster_size,
                    infos->data+(m_current_cluster-2)*infos->cluster_size,
                    buff,
                    infos->sector_size);
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
        buffer[read]=Fat_entry(first_cluster,is_directory,(char*)(buff+32*m_current_entry_offset),size);
        read++;
        m_current_entry_offset++;
    }

    return read;
}

// bool Fat_entry::add_entry(char* name,bool is_directory,Fat_entry* buff)
// {
    // if(!m_is_directory)
        // return false;
    // *buff
// }