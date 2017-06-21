#include <gtk/gtk.h>
#include "gui.h"
#include "trade.h"
#include "player.h"
#include "word_func.h"
block prop_map[8][8];
//int store[]={
//  PROP1,PROP2 //现在要解决一个技术问题，如何为集合中的元素添加属性，比如为store里的PROPx添加仓储量属性。位图就是令数组下标之集合作为主集合，数组元素的值作为要添加的属性。实际上构建两个元素两个集合之间的联系的方式就是赋值，比如A=1；这里的A和1都可以表示为某者的抽象或者说代表，比如大写字母表示男人，数字表示女人，等号表示结婚。之前讨论位管理系统时似乎也没有能把问题描述清楚，现在就做个了断吧。实际上为集合中的元素添加属性就是半广义同转化为半广义不同之也即为一集合建立与另一集合的联系之当然这里可以复层级循重，就像把静态的PROPx改造成动态的按钮或下拉菜单，也像设立一个PROPx结构体，则PROPx.仓储量之类就很好理解了。可是如果不是定义store时就确定PROPx的数据类型就会有数据类型冲突的问题，当然如何在PROPx定义为int的前提下实现为其设立结构体也可以成为一个颇具挑战性的技术问题，而这种技术问题就是高层级的具有作为大公式之编程思想效力的普适性问题了，值得重点研究。之编程要解决的不只是模拟的问题，而也包括其自身的技术问题。首先是如何为集合中的元素添加属性，这里使用结构体的嵌套就行，比如PROPx是结构体A的结构变量，则store就要定义为A类型。如何在PROPx定义为int的前提下实现为其设立结构体，似乎可以设立一个int集合，或者就是结构体，或者就是数组:仓储量[],用以记录PROPx的仓储量，这跟设立PROPx相比就是集合划分的不同了，之本来可以不设立结构体不设立数组而使用独立的变量或者说标志位来表示、存储信息的，而当所有这些标志位统一编号之于一个集合中，这就是图灵机的纸带模型。这可以完美说明如何在PROPx定义为int的前提下实现为其设立结构体的可行性以及思想深度。a.b[3].c[3][3]之似集线器那样，之结构体的作用就是设立集合之间的联系，之在这里就是a一个元素b三个元素c九个元素之间的联系，之用.来表示联系。结构体确实就是终结符变非终结符那样，半广义同变半广义不同那样，之要明白结构变量跟其元素之间的关系就是集合之间的关系。毕竟就像这里设立的仓储量[]，其与store[]的联系就只是靠函数建立了，而设立PROPx结构体则就是设立了一些集合之一个系统来体现其联系了。与多维数组之下标本身就是少生多生半广义不同者，多维数组之若干下标就是一个向量之一个集合，生成的半广义不同通过赋值之类者与别的集合建立联系，这当然也是shell。离散无序的元素是很普遍的，则这种思想可以很好地管理集合及集合中的元素。
//};
void full()
{
  GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_store),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Sorry,but your backpack is full！");
  gtk_window_set_title(GTK_WINDOW(dialog), "东方不挂组英语学习系统");
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(dialog,GTK_STATE_NORMAL,&c1);
  
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
void empty()
{
  GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_store),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Sorry,but there is no such prop in your backpack！");
  gtk_window_set_title(GTK_WINDOW(dialog), "东方不挂组英语学习系统");
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(dialog,GTK_STATE_NORMAL,&c1);
  
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
void game_empty()
{
  GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(word_help_window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Sorry,but there is no such prop in your backpack！");
  gtk_window_set_title(GTK_WINDOW(dialog), "东方不挂组英语学习系统");
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(dialog,GTK_STATE_NORMAL,&c1);
  
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
void refresh_gold_num()//本来是在buy()和sell()中都写有的相同过程，因为会被用得比较多所以独立出来
{
  char str[66]="Gold:";
  char money[22];
  sprintf(money,"%d",player.object.gold);
  add(str,money);
  if(sell_switch)gtk_label_set_text(GTK_LABEL(gold_num),str); //在商店模式才刷新，这也刚好沾了商店模式游戏模式不能同时启动的光
}
int buy(int prop)
{
  int i,j,quit=0;
  for(i=0;i<8;i++)
  { 
    for(j=0;j<8;j++)
    {
      if(player.prop[i][j]==0)
      {
        player.prop[i][j]=prop;
        if(prop==2)gtk_button_set_label(GTK_BUTTON(prop_map[i][j].button), "T");
        else gtk_button_set_label(GTK_BUTTON(prop_map[i][j].button), "B");
        quit++;
        break;
      }
    }
    if(quit)break;
  }
  if(!quit)
  {
    full();
    return 1;
  }
  player.object.gold-=222;
  refresh_gold_num();
  return 0;
}
int sell(int prop)
{
  int i,j,quit=0;
  for(i=0;i<8;i++)
  {
    for(j=0;j<8;j++)
    {
      if(player.prop[i][j]==prop)//本来是if(player.prop[i][j]=prop)这种低级错误要注意避免
      {
        player.prop[i][j]=0;
        if(sell_switch)gtk_button_set_label(GTK_BUTTON(prop_map[i][j].button), "");//在商店模式才刷新
        quit++;
        break;
      }
    }
    if(quit)break;
  }
  if(!quit)
  {
    if(sell_switch)empty();
    else game_empty();
    return 1;
  }
  player.object.gold+=111;
  refresh_gold_num();
  return 0; 
}
