#include <string.h>
typedef struct{
  char *name;
  int sex;
  int gold;
}Object;//游戏中的实体
typedef struct{
  Object object;
  int prop[8][8];
}Player;//玩家

extern Player player;

extern int init_prop(int prop[8][8]);
extern Player create_player(Player player);


