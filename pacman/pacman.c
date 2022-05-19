#include "../stdlib/stdlib.h"

char map[1459]="\
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\
xx                      xx                      xx\
xx  xxxxxxxx  xxxxxxxx  xx  xxxxxxxx  xxxxxxxx  xx\
xx  xxxxxxxx  xxxxxxxx  xx  xxxxxxxx  xxxxxxxx  xx\
xx                                              xx\
xx  xxxxxxxx  xx  xxxxxxxxxxxxxx  xx  xxxxxxxx  xx\
xx            xx  xxxxxxxxxxxxxx  xx            xx\
xxxxxxxxxxxx  xx        xx        xx  xxxxxxxxxxxx\
eeeeeeeeeexx  xxxxxxxx      xxxxxxxx  xxeeeeeeeeee\
eeeeeeeeeexx  xxeeeeeeeeeeeeeeeeeexx  xxeeeeeeeeee\
eeeeeeeeeexx  xxeexxxx______xxxxeexx  xxeeeeeeeeee\
xxxxxxxxxxxx  xxeexxeeeeeeeeeexxeexx  xxxxxxxxxxxx\
eeeeeeeeeeee  eeeexxeeeeeeeeeexxeeee  eeeeeeeeeeee\
xxxxxxxxxxxx  xxeexxeeeeeeeeeexxeexx  xxxxxxxxxxxx\
eeeeeeeeeexx  xxeexxxxxxxxxxxxxxeexx  xxeeeeeeeeee\
xxxxxxxxxxxx  xxeeeeeeeeeeeeeeeeeexx  xxxxxxxxxxxx\
xxxxxxxxxxxx  xx  xxxxxxxxxxxxxx  xx  xxxxxxxxxxxx\
xx                                              xx\
xx  xxxxxxxx  xxxxxxxx  xx  xxxxxxxx  xxxxxxxx  xx\
xx  xxxxxxxx  xxxxxxxx  xx  xxxxxxxx  xxxxxxxx  xx\
xx                      xx                      xx\
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

#define mapx 25
#define mapy 22

char map_points[mapx*mapy];

typedef struct
{
  int x;
  int y;
} Coord;


void draw_perso(Coord c)
{
  print_screen(2*c.x,c.y,'o',YELLOW,BLACK);
  print_screen(2*c.x+1,c.y,'<',YELLOW,BLACK);
}

void draw_fantome(Coord c,vga_color col)
{
  print_screen(2*c.x,c.y,'o',WHITE,col);
  print_screen(2*c.x+1,c.y,'o',WHITE,col);
}

void draw_map(int x,int y)
{
  for(int i=0;i<2;i++)
  {
    switch (map[2*(x+y*mapx)+i])
    {
    case 'e':
    case ' ':
      print_screen(2*x+i,y,' ',WHITE,BLACK);
      break;
    case 'x':
      print_screen(2*x+i,y,' ',WHITE,BLUE);
      break;
    default:
      print_screen(2*x+i,y,map[2*(x+y*mapx)+i],WHITE,BLACK);
      break;
    }
  }
  if(map_points[x+y*mapx]=='.')
  {
    print_screen(2*x,y,'.',WHITE,BLACK);
  }
}

char possible_dir(Coord c)
{
  Coord dirs[4]={{-1,0},{0,-1},{0,1},{1,0}};
  char ret=0;
  for(int i=0;i<4;i++)
  {
    int x=c.x+dirs[i].x;
    int y=c.y+dirs[i].y;
    if(map[2*(x+y*mapx)]=='e'||map[2*(x+y*mapx)]==' '||map[2*(x+y*mapx)]=='_')
    {
      ret|=1<<i;
    }
  }
  return ret;
}

int is_possible(Coord c,char pos)
{
  Coord dirs[4]={{-1,0},{0,-1},{0,1},{1,0}};
  for(int i=0;i<4;i++)
  {
    if(dirs[i].x==c.x&&dirs[i].y==c.y)
    {
      if(pos&(1<<i))
        return 1;
    }
  }
  return 0;
}

void pause(uint32 t)
{
  uint32 ticks=get_ticks();
  while(ticks+t>=get_ticks());
}

int main(char* arg)
{
  Coord perso;
  perso.x=12;
  perso.y=17;
  Coord perso_dir;
  perso_dir.x=0;
  perso_dir.y=0;
  Coord perso_next_dir;
  perso_next_dir.x=0;
  perso_next_dir.y=0;
  Coord fantomes[4];
  Coord fantomes_dir[4];
  vga_color fantomes_col[4]={RED,BRIGHT_RED,BROWN,CYAN};
  for(int i=0;i<4;i++)
  {
    fantomes[i].x=12;
    fantomes[i].y=11;
    fantomes_dir[i].x=0;
    fantomes_dir[i].y=0;
  }
  for(int x=0;x<mapx;x++)
  {
    for(int y=0;y<mapy;y++)
    {
      if(map[2*(x+y*mapx)]==' ')
      {
        map_points[x+y*mapx]='.';
      }
      else
      {
        map_points[x+y*mapx]=' ';
      }
    }
  }

  for(int x=0;x<mapx;x++)
  {
    for(int y=0;y<mapy;y++)
    {
      draw_map(x,y);
    }
  }

  draw_perso(perso);
  for(int i=0;i<4;i++)
    draw_fantome(fantomes[i],fantomes_col[i]);

  uint32 random_x=get_ticks();

  uint32 vim_bindings=0;

  while(1)
  {
    keyboard_t k=get_keyboard();
    if(k.type==0)
    {
      if(k.k.ch=='a')
      {
        vim_bindings=!vim_bindings;
      }
      if(vim_bindings)
      {
        switch (k.k.ch)
        {
        case 'k':
          perso_next_dir.x=0;
          perso_next_dir.y=-1;
          break;
        case 'j':
          perso_next_dir.x=0;
          perso_next_dir.y=1;
          break;
        case 'l':
          perso_next_dir.x=1;
          perso_next_dir.y=0;
          break;
        case 'h':
          perso_next_dir.x=-1;
          perso_next_dir.y=0;
          break;
        default:
          break;
        }
      }
      else
      {
        switch (k.k.ch)
        {
        case 'i':
          perso_next_dir.x=0;
          perso_next_dir.y=-1;
          break;
        case 'k':
          perso_next_dir.x=0;
          perso_next_dir.y=1;
          break;
        case 'l':
          perso_next_dir.x=1;
          perso_next_dir.y=0;
          break;
        case 'j':
          perso_next_dir.x=-1;
          perso_next_dir.y=0;
          break;
        default:
          break;
        }
      }
    }
    if(is_possible(perso_next_dir,possible_dir(perso)))
    {
      perso_dir.x=perso_next_dir.x;
      perso_dir.y=perso_next_dir.y;
      perso_next_dir.x=0;
      perso_next_dir.y=0;
    }
    if(is_possible(perso_dir,possible_dir(perso)))
    {
      draw_map(perso.x,perso.y);
      perso.x+=perso_dir.x;
      perso.y+=perso_dir.y;
      if(map_points[perso.x+perso.y*mapx]=='.')
      {
        map_points[perso.x+perso.y*mapx]=' ';
      }
      if(perso.x>=mapx-1)
      {
        perso.x=1;
      }
      if(perso.x<=0)
      {
        perso.x=mapx-2;
      }
      draw_perso(perso);
    }
    for(int i=0;i<4;i++)
    {
      if(fantomes[i].x==perso.x&&fantomes[i].y==perso.y)
      {
        write(0,"perdu");
        pause(2000);
        return 42;
      }
    }

    for(int i=0;i<4;i++)
    {
      Coord dirs[4]={{-1,0},{0,-1},{0,1},{1,0}};
      Coord dirs_pos[4];
      int nb_dirs=0;
      char pos=possible_dir(fantomes[i]);
      for(int j=0;j<4;j++)
      {
        if(is_possible(dirs[j],pos)&&((fantomes_dir[i].x!=-dirs[j].x)||(fantomes_dir[i].y!=-dirs[j].y)))
        {
          dirs_pos[nb_dirs].x=dirs[j].x;
          dirs_pos[nb_dirs].y=dirs[j].y;
          nb_dirs++;
        }
      }
      if(nb_dirs!=0)
      {
        int rand=random_x%nb_dirs;
        random_x=16807*random_x;
        fantomes_dir[i].x=dirs_pos[rand].x;
        fantomes_dir[i].y=dirs_pos[rand].y;
      }
      else
      {
        fantomes_dir[i].x=0;
        fantomes_dir[i].y=0;
      }

      draw_map(fantomes[i].x,fantomes[i].y);
      fantomes[i].x+=fantomes_dir[i].x;
      fantomes[i].y+=fantomes_dir[i].y;
      if(fantomes[i].x>=mapx-1)
      {
        fantomes[i].x=1;
      }
      if(fantomes[i].x<=0)
      {
        fantomes[i].x=mapx-2;
      }
      draw_fantome(fantomes[i],fantomes_col[i]);

      if(fantomes[i].x==perso.x&&fantomes[i].y==perso.y)
      {
        write(0,"perdu");
        pause(2000);
        return 42;
      }
    }

    pause(100);
  }  
}