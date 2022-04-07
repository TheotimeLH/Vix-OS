#include "ata_driver.h"

Drive_id ata_identify(Drive d)
{
    bool primary=((d&0b10)==0);
    bool master=((d&0b01)==0);
	uint16_t io_base=(primary)?0x1f0:0x170;

	outb(io_base|0x6,(master)?0xA0:0xB0);

	outb(io_base|0x7,0xEC);

	uint8_t status=inb(io_base|0x7);
	Drive_id ret;
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
	

	for(;i<256;i++);

	return ret;
}

bool ata_read(Drive d,uint8_t count,uint32_t lba,uint16_t* buffer)
{
    bool primary=((d&0b10)==0);
    bool master=((d&0b01)==0);
	lba=lba&0xFFFFFFF;

	uint16_t io_base=(primary)?0x1f0:0x170;

	outb(io_base|0x6,((master)?0xE0:0xF0)|(lba>>24));//drive select + 24-27 of lba
	outb(io_base|0x1,0);//error
	outb(io_base|0x2,count);//count
	outb(io_base|0x3,lba);//0-7 of lba
	outb(io_base|0x4,lba>>8);//8-15 of lba
	outb(io_base|0x5,lba>>16);//16-23 of lba
	outb(io_base|0x7,0x20);//command

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

	outb(io_base|0x6,((master)?0xE0:0xF0)|(lba>>24));//drive select + 24-27 of lba
	outb(io_base|0x1,0);//error
	outb(io_base|0x2,count);//count
	outb(io_base|0x3,lba);//0-7 of lba
	outb(io_base|0x4,lba>>8);//8-15 of lba
	outb(io_base|0x5,lba>>16);//16-23 of lba
	outb(io_base|0x7,0x30);//command

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
			outw(io_base,buffer[i*256+j]);
		}
	}

	return true;
	
}