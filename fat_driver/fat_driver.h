#ifndef _FAT_DRIVER_H_
#define _FAT_DRIVER_H_

#include <stddef.h>
#include "../common/common.h"
#include "../ata_driver/ata_driver.h"
#include "../video/vga_driver.h"

typedef struct Fat_infos Fat_infos;

//interface entre le driver disque et le driver fat
class Fat_system
{
public:
    virtual bool read(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512)=0;
    virtual bool write(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512)=0;
    virtual void err(char* msg)=0;
    virtual uint32_t get_sectorsize(){return 512;};
};

//implémentation pour le driver ata
class Ata_fat_system:public Fat_system
{
public:
    Ata_fat_system(Drive d);
    virtual bool read(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512);
    virtual bool write(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512);
    virtual void err(char* msg);
    bool is_ready(){return m_id.exists&&!m_id.erreur;};
    virtual uint32_t get_sectorsize(){return m_id.taille_secteur;};
private:
    Drive_id m_id;
    Drive m_drive;
};

//une entrée dans le système fat (fichier ou dossier)
class Fat_entry
{
public:
    Fat_entry(){};
    Fat_entry(bool is_directory,char* name):Fat_entry(0,is_directory,name,0,0,0){};
    Fat_entry(uint32_t cluster,bool is_directory,char* name,uint32_t size,uint32 entry_sector,uint32 entry_offset)
    :m_is_directory(is_directory),m_first_cluster(cluster),m_current_cluster(cluster),
    m_current_entry_offset(0),m_last_entry(0),m_size(size),m_entry_sector(entry_sector),m_entry_offset(entry_offset)
    {
        m_name[8]='\0';
        for(int i=0;i<8;i++)
        {
            m_name[i]=name[i];
            if(m_name[i]==0||m_name[i]==' ')
            {
                m_name[i]=0;
                break;
            }
        }
    };
    Fat_entry(const Fat_entry &obj)
    :Fat_entry(obj.m_first_cluster,obj.m_is_directory,(char*)obj.m_name,obj.m_size,obj.m_entry_sector,obj.m_entry_offset)
    {};
    constexpr Fat_entry& operator=(const Fat_entry& obj)
    {
        m_is_directory=obj.m_is_directory;
        m_first_cluster=obj.m_first_cluster;
        m_current_cluster=obj.m_first_cluster;
        m_current_entry_offset=0;
        m_last_entry=0;
        m_size=obj.m_size;
        m_entry_sector=obj.m_entry_sector;
        m_entry_offset=obj.m_entry_offset;
        m_name[8]='\0';
        for(int i=0;i<8;i++)
        {
            m_name[i]=obj.m_name[i];
            if(m_name[i]==0||m_name[i]==' ')
            {
                m_name[i]=0;
                break;
            }
        }
        
        return *this;
    }
    char* get_name(){return m_name;};
    bool is_directory(){return m_is_directory;};

    //réinitialise le curseur à 0 dans le fichier/dossier
    void init_offset(){m_current_cluster=m_first_cluster;m_current_entry_offset=0;m_last_entry=false;};
    uint32_t get_size(){return m_size;};

    //uniquement pour les fichiers

    //renvoit la taille lue en octet
    //si elle est inférieure à cluster_count*infos.byte_per_cluster (la taille du buffer),
    //c'est que la fin du fichier a été atteinte
    //utiliser init_offset pour repartir du début
    //attention, le buffer doit être de taille cluster_count*infos.byte_per_cluster
    uint32_t read_data(uint8_t *buffer,uint32_t cluster_count,Fat_infos* infos,Fat_system* intf);
    bool write_data(uint8* buffer,uint32 size,Fat_infos* infos,Fat_system* intf);//append data at the end of the file

    //uniquement pour les répertoires

    //renvoit le nombre d'entrées lues, s'il est inférieur à size,
    //c'est que toutes les entrées du répertoire ont été lues
    //(size est la taille du buffer)
    //utiliser init_offset pour repartir du début
    //taille de buffer conseillée :
    //  - pour la racine en fat12/16 : infos.sector_size/32
    //  - sinon, infos.byte_per_cluster/32
    uint32_t read_entries(Fat_entry* buffer,uint32_t size,Fat_infos* infos,Fat_system* intf);

    //entry_ret est un pointeur vers un objet Fat_entry non initialisé
    //l'entrée créée est renvoyé dans ce pointeur
    bool add_entry(char* name,bool is_directory,Fat_entry* entry_ret,Fat_infos* infos,Fat_system* intf);
    bool delete_entry(char* name,Fat_infos* infos,Fat_system *intf);
private:
    bool m_is_directory;
    uint32_t m_first_cluster;//0 for root (FAT12/16)
    uint32_t m_current_cluster;//sector offset for fat12/16 root
    uint32_t m_current_entry_offset;//for directory
    bool m_last_entry;//for directory
    uint32_t m_size;//en octets
    char m_name[9];
    uint32 m_entry_sector;//addresse des infos sur l'entrée en secteur
    uint32 m_entry_offset;//offset en byte
};

Fat_infos fat_init(Fat_system* intf);

struct Fat_infos
{
    uint8_t fat_type;//12,16,32
    uint16_t sector_size;//en octet
    uint8_t cluster_size;//en secteurs
    uint32_t byte_per_cluster;//cluster size en octet
    uint32_t fat;//en secteurs
    uint32_t fat_size;//...
    uint32_t root;//en secteur pour 12/16 et en cluster pour 32
    uint32_t root_size;//en secteur
    uint32_t data;//...
    uint32_t data_size;//...
    Fat_entry root_fat_entry;//la racine
};

Fat_entry open_file(char* name,Fat_system *intf,Fat_infos* infos,Fat_entry *dir,bool* ok);
Fat_entry open_dir(char* name,Fat_system *intf,Fat_infos* infos,Fat_entry *dir,bool* ok);

#endif
