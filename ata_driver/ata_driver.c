#include "ata_driver.h"

Drive_id ata_identify(Drive d)
{
    bool primary=((d&0b10)==0);
    bool master=((d&0b01)==0);
	uint16_t io_base=(primary)?0x1f0:0x170;
    uint16_t controle_base=(primary)?0x3F6:0x376;

	outb(0b00000010,controle_base);

	outb((master)?0xA0:0xB0,io_base|0x6);
	for(int i=2;i<5;i++)
		outb(0,io_base|i);

	outb(0xEC,io_base|0x7);///identify cmd

	Drive_id ret;

	uint8_t status=inb(io_base|0x7);
	if(status==0)
	{
		ret.exists=false;
		return ret;
	}
	ret.exists=true;

	while(status&(1<<7)&&!(status&(1<<3)))
	{
		status=inb(io_base|0x7);
		if((status&1)||(status&(1<<5)))
		{
			ret.erreur=true;
			return ret;
		}
	}
	ret.erreur=false;

	uint16_t temp;
	int i=0;
	for(;i<60;i++)
	{
		inw(io_base|0x0);
	}

	temp=inw(io_base|0x0);
	ret.nb_secteur=temp;
	temp=inw(io_base|0x0);
	ret.nb_secteur=ret.nb_secteur|(temp<<16);
	i+=2;

	for(;i<106;i++)
	{
		inw(io_base|0x0);
	}

	temp=inw(io_base|0x0);
	i++;
	if((temp|(1<<12)))
	{
		for(;i<117;i++)
			inw(io_base|0x0);

		temp=inw(io_base|0x0);
		ret.taille_secteur=temp;
		temp=inw(io_base|0x0);
		ret.taille_secteur=ret.taille_secteur|(temp<<16);
		i+=2;
	}
	else
	{
		ret.taille_secteur=512;		
	}
	if(ret.taille_secteur==0)
		ret.taille_secteur=512;
	

	for(;i<256;i++) inw(io_base);

	
	return ret;
}

bool ata_read(Drive d,uint8_t count,uint32_t lba,uint16_t* buffer)
{
    bool primary=((d&0b10)==0);
    bool master=((d&0b01)==0);

	lba=lba&0xFFFFFFF;

	uint16_t io_base=(primary)?0x1f0:0x170;
    uint16_t controle_base=(primary)?0x3F6:0x376;
    uint8_t status;
	int n=0;

    do
    {
		n++;
        for(int i=0;i<50;i++)
            status=inb(io_base|0x7);
    }while((status&(1<<7)!=0));
	if(n>=1000)
	{
		return false;
	}

	outb(((master)?0xE0:0xF0)|((lba>>24)&0xF),io_base|0x6);//drive select + 24-27 of lba
	outb(0,io_base|0x1);//error
	outb(count,io_base|0x2);//count
	outb((uint8_t)lba,io_base|0x3);//0-7 of lba
	outb((uint8_t)(lba>>8),io_base|0x4);//8-15 of lba
	outb((uint8_t)(lba>>16),io_base|0x5);//16-23 of lba
	outb(0x20,io_base|0x7);//command

	uint16_t count_16=(count==0)?256:count;
	for(int i=0;i<count_16;i++)
	{
		status=inb(io_base|0x7);
		n=0;
		while((status&(1<<7)!=0)||((status&(1<<3))==0)&&n<1000)
		{
			n++;
			status=inb(io_base|0x7);
			if((status&1)||(status&(1<<5)))//err
			{
				return false;
			}
		}
		if(n>=1000)
		{
			return false;
		}
		if((status&1)||(status&(1<<5)))//err
		{
			return false;
		}

		for(int j=0;j<256;j++)
		{
			buffer[i*256+j]=inw(io_base);
		}
	}

	return true;
}

bool ata_write(Drive d,uint8_t count,uint32_t lba,uint16_t *buffer)
{
    bool primary=((d&0b10)==0);
    bool master=((d&0b01)==0);

	lba=lba&0xFFFFFFF;

	uint16_t io_base=(primary)?0x1f0:0x170;

	outb(((master)?0xE0:0xF0)|(lba>>24),io_base|0x6);//drive select + 24-27 of lba
	outb(0,io_base|0x1);//error
	outb(count,io_base|0x2);//count
	outb(lba,io_base|0x3);//0-7 of lba
	outb(lba>>8,io_base|0x4);//8-15 of lba
	outb(lba>>16,io_base|0x5);//16-23 of lba
	outb(0x30,io_base|0x7);//command

	uint16_t count_16=(count==0)?256:count;
	for(int i=0;i<count_16;i++)
	{
		uint8_t status=inb(io_base|0x7);
		while((status&(1<<7))&&(!(status&(1<<3))))
		{
			status=inb(io_base|0x7);
			if((status&1)||(status&(1<<5)))//err
			{
				return false;
			}
		}

		for(int j=0;j<256;j++)
		{
			outw(buffer[i*256+j],io_base);
		}
	}

	return true;
	
}