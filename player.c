#include<stdio.h>
#include"player.h"
Player player;
int init_prop(int prop[8][8])
{
  int i,j;
  for(i=0;i<8;i++)for(j=0;j<8;j++)prop[i][j]=0;
  return 0;
}
Player create_player(Player player)
{
  int i;
  player.object.sex=1;
  player.object.gold=1000000;
  init_prop(player.prop);
  return player;
}
