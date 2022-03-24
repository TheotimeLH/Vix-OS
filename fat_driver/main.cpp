#include "fat_driver.h"
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

int main()
{
    File_Fat_system ffs("test");
    Fat_infos infos=fat_init(&ffs);
    printf(
"type : %d\nsector_size : %d\ncluster_size : %d\nfat : %d\nfat_size : %d\nroot : %d\nroot_size : %d\ndata : %d\ndata_size :%d\n",
    infos.fat_type,infos.sector_size,infos.cluster_size,infos.fat,infos.fat_size,infos.root,infos.root_size,
    infos.data,infos.data_size);

    Fat_entry buff[infos.sector_size/32];
    uint32_t written=infos.root_fat_entry.read_entries(buff,infos.sector_size/32,&infos,&ffs);
//    cout<<buff[0].m_first_cluster<<endl<<buff[0].is_directory()<<endl<<buff[0].m_size<<endl;

    uint8_t data_buff[infos.byte_per_cluster];
    uint32_t read=buff[0].read_data(data_buff,1,&infos,&ffs);
    cout<<read<<endl;
    data_buff[read]=0;
    cout<<string((char*)data_buff)<<endl;


    return 0;
}