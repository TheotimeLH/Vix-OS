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
    fat_init(&afs);
    while(1);
    print_string("fat_type : ");
    //print_int(fi.fat_type);

	while(1);
}
