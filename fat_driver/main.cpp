#include "fat_driver.h"

/*
#include <iostream>
#include <fstream>

using namespace std;

class File_Fat_system:public Fat_system
{
public:
    File_Fat_system(string filename):m_filename(filename){};
    bool read(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size)
    {
        filebuf file;
        if(!file.open(m_filename,ios_base::binary|ios_base::out|ios_base::in))
            return false;
        file.pubseekpos(sector_size*addr);
        for(int i=0;i<count*sector_size;i++)
        {
            buffer[i]=file.sbumpc();
        }
        if(!file.close())
            return false;

        return true;
    };
    bool write(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512)
    {
        filebuf file;
        if(!file.open(m_filename,ios_base::binary|ios_base::out|ios_base::in))
            return false;
        file.pubseekpos(sector_size*addr);
        for(int i=0;i<count*sector_size;i++)
        {
            file.sputc(buffer[i]);
        }
        if(!file.close())
            return false;

        return true;
    }
    void err(char* msg)
    {
        cout<<string(msg);
    }
    ~File_Fat_system(){};
private:
    string m_filename;
};
*/


extern "C" void fat_main()
{
    init_vga(0x07,0x0);
    Ata_fat_system afs(hda);
    Fat_infos infos=fat_init(&afs);
    print_string("fat_type : ");
    print_int(infos.fat_type);
    print_new_line();

    print_string((char*)infos.root_fat_entry.get_name());
    print_string(" : \n");
    
    Fat_entry entry_buff[infos.sector_size/32];
    int n=infos.root_fat_entry.read_entries(entry_buff,infos.sector_size/32,&infos,&afs);
    print_int(n);
    print_new_line();

    int fichier1;
    for(int i=0;i<n;i++)
    {
        print_string((char*)entry_buff[i].get_name());
        if(entry_buff[i].is_directory())
        {
            print_string(" (dir)");
        }
        else
        {
            print_string(" ");
            print_int(entry_buff[i].get_size());
        }
        print_new_line();
        if(strcmp(entry_buff[i].get_name(),"FICHIER1"))
            fichier1=i;
    }

    print_string("\nfichier1 : \n");
    uint8_t data_buff[infos.byte_per_cluster];
    n=entry_buff[fichier1].read_data(data_buff,1,&infos,&afs);
    data_buff[n+1]=0;
    print_string((char*)data_buff);

    while(1);
}
