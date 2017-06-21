#include <gtk/gtk.h>
#include"player.h"       
#include"gui.h"
#include"trade.h"
#include"word_func.h"
GtkWidget *window_main;//main函数窗口
GtkWidget *game;
GtkWidget *store;
static gint button_size=30; //每个按钮的大小

int sell_switch;//确定调用sell()时应调用哪个空处理函数
GtkWidget *gold_num;
GtkWidget *window_store;
GtkWidget *prop1;
GtkWidget *prop2;
GtkWidget *store_label;
GtkWidget *num;
int select_prop;
int quantity=0;

void no_enough_money()
{
  GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_store),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Sorry,but you do not have enough money！");
  gtk_window_set_title(GTK_WINDOW(dialog), "东方不挂组英语学习系统");
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(dialog,GTK_STATE_NORMAL,&c1);
  
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
void select_become_three()
{
  select_prop=1;
  const gchar *str;
  str=gtk_button_get_label(GTK_BUTTON(prop1));
  gtk_label_set_text(GTK_LABEL(store_label),str);
}
void select_tell_me()
{
  select_prop=2;
  const gchar *str;
  str=gtk_button_get_label(GTK_BUTTON(prop2));
  gtk_label_set_text(GTK_LABEL(store_label),str);
}
void buy_prop()
{
  const gchar *str;
  int i;
  str=gtk_entry_get_text(GTK_ENTRY(num));
  quantity=atoi(str);
  if(player.object.gold>222*quantity)for(i=0;i<quantity;i++){if(buy(select_prop))break;}//本来初始金币数设得足够少就可以不用避免输入的买入数目过大之对该程序设置bug就被屏蔽了，不过还是全面点好。另外这里for里的if要加花括号，否则就会被最近原则弄出bug
  else no_enough_money();
}
void sell_prop()
{
  const gchar *str;
  str=gtk_entry_get_text(GTK_ENTRY(num));
  quantity=atoi(str);
  int m,n,sum=0;
  for(m=0;m<8;m++)
  {
    for(n=0;n<8;n++)
    {
      if(player.prop[m][n]==select_prop)
      {
        sum++;
      }
    }
  }
  if(sum<quantity)quantity=sum;//避免输入的卖出数目过大
  int i;
  for(i=0;i<quantity;i++)
    sell(select_prop);
}
void store_close()
{
  sell_switch=0;
  gtk_widget_destroy(GTK_WIDGET(window_store));
}
gint store_main()//商店界面
{
  GtkWidget *table1;
  GtkWidget *table2;
  GtkWidget *buy;
  GtkWidget *sell;
  GtkWidget *box;
  GtkWidget *hbox;

  //gtk_init_check(&argc,&argv); /*初始化GTK+工具库*/
  window_store=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/
  gtk_window_set_title(GTK_WINDOW(window_store), "东方不挂组英语学习系统");
  gtk_widget_set_size_request(window_store, 240, 480);
  gtk_window_set_resizable(GTK_WINDOW(window_store), FALSE);
  gtk_window_set_position(GTK_WINDOW(window_store), GTK_WIN_POS_CENTER);
  g_signal_connect(G_OBJECT(window_store),"delete_event",G_CALLBACK(store_close),NULL); /*登记窗体delete_event信号*/
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(window_store,GTK_STATE_NORMAL,&c1);
  
  box=gtk_vbox_new(FALSE,0);/*创建组合框*/
  table1=gtk_table_new(1,2,TRUE);
  table2=gtk_table_new(2,1,TRUE);
  
  prop1=gtk_button_new_with_label("become_three");/*创建按钮*/
  g_signal_connect(G_OBJECT(prop1),"pressed",G_CALLBACK(select_become_three),window_store);
  prop2=gtk_button_new_with_label("tell_me");/*创建按钮*/
  g_signal_connect(G_OBJECT(prop2),"pressed",G_CALLBACK(select_tell_me),window_store);
  gtk_table_attach_defaults(GTK_TABLE(table1), prop1, 0,1,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table1), prop2, 1,2,0,1);
  gtk_box_pack_start(GTK_BOX(box), table1, FALSE, TRUE, 3);

  store_label=gtk_label_new("Please select a prop above.");/*创建标号*/
  gtk_box_pack_start(GTK_BOX(box),store_label,FALSE,FALSE,5);/*把标号加入组合框*/
  gtk_widget_show(store_label);/*显示标号*/
  
  GdkColor c2;
  do{gdk_color_parse (color[rand()%7], &c2);}while(c2.red==c1.red&&c2.green==c1.green&&c2.blue==c1.blue);//给标签上色同时避免与窗口背景色撞色而被淹没
  gtk_widget_modify_fg(store_label,GTK_STATE_NORMAL,&c2);
  
  num=gtk_entry_new();/*创建录入构件*/
  gtk_box_pack_start(GTK_BOX(box),num,FALSE,FALSE,5);/*把录入构件加入组合框*/
  gtk_widget_show(num);/*显示录入构件*/
  gtk_entry_set_text(GTK_ENTRY(num),"Please enter the quantity here.");
  
  buy=gtk_button_new_with_label("Buy");/*创建按钮*/
  g_signal_connect(G_OBJECT(buy),"pressed",G_CALLBACK(buy_prop),window_store);
  sell=gtk_button_new_with_label("Sell");/*创建按钮*/
  g_signal_connect(G_OBJECT(sell),"pressed",G_CALLBACK(sell_prop),window_store);
  gtk_table_attach_defaults(GTK_TABLE(table2), buy, 0,1,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table2), sell, 1,2,0,1);
  gtk_box_pack_start(GTK_BOX(box), table2, FALSE, TRUE, 3);
  
  int i,j;
  for(i=0;i<8;i++)//所有格子的初始化
  {
    hbox=gtk_hbox_new(FALSE, 0);
    for(j=0;j<8;j++)
    {
      GtkWidget *button;
      if(player.prop[i][j]==1)button=gtk_button_new_with_label("B");
      else if(player.prop[i][j]==2)button=gtk_button_new_with_label("T");
      else button=gtk_button_new_with_label("");
      gtk_widget_set_size_request(button,button_size, button_size);
      g_object_set(G_OBJECT(button),"can-focus", FALSE, NULL);
      gtk_box_pack_start(GTK_BOX(hbox),button, FALSE, FALSE, 0);
      gtk_widget_show(button);
      //g_signal_connect(G_OBJECT(button),"button-press-event",G_CALLBACK(sell_prop),NULL);
      prop_map[i][j].button=button;
    }
    gtk_box_pack_start(GTK_BOX(box), hbox,FALSE, FALSE, 0);
    gtk_widget_show(hbox);
  }
  
  char str[66]="Gold:";
  char money[22];
  sprintf(money,"%d",player.object.gold);
  add(str,money);
  gold_num=gtk_label_new(str);/*创建标号*/
  gtk_box_pack_start(GTK_BOX(box),gold_num,FALSE,FALSE,5);/*把标号加入组合框*/
  gtk_widget_show(gold_num);/*显示标号*/
  
  GdkColor c3;
  do{gdk_color_parse (color[rand()%7], &c3);}while(c3.red==c1.red&&c3.green==c1.green&&c3.blue==c1.blue);
  gtk_widget_modify_fg(gold_num,GTK_STATE_NORMAL,&c3);
  
  gtk_container_add(GTK_CONTAINER(window_store),box);/*把组合框加入窗体*/
  //gtk_widget_show(box);/*显示组合框*/
  gtk_widget_show_all(window_store);/*显示窗体*/
  //gtk_main();

  //gtk_widget_set_sensitive(GTK_WIDGET(game),FALSE);    /* 禁用button */
  sell_switch=1; 

  return 0;
}
int about_system()
{
  GtkWidget *dialog=gtk_message_dialog_new(GTK_WINDOW(window_main),
  GTK_DIALOG_DESTROY_WITH_PARENT,
  GTK_MESSAGE_INFO,
  GTK_BUTTONS_OK,
  "郭乃榕英语学习系统，版权所有，翻版必究！");
  gtk_window_set_title(GTK_WINDOW(dialog), "About");
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(dialog,GTK_STATE_NORMAL,&c1);
  
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}

int main_close()
{
  if(left)
  {
    play_more();
    return(1);
  }
  gtk_main_quit();
  return 0;
}

GdkPixbuf *create_pixbuf(const char *image_name) 
{
  GdkPixbuf *pixbuf;
  //加载图片文件，第二个参数设置错误变量的，不需要则设为 NULL  
  pixbuf = gdk_pixbuf_new_from_file(image_name , NULL);
  if(!pixbuf)
  {
    perror("gdk_pixbuf_new_from_file()\n");
    return NULL; 
  }
  return pixbuf; 
}

gint main(gint argc,gchar **argv)//主界面。虽然程序外在表现是实现随机提供不同种类的不同练习，但使用游戏之shell的做法跟单纯的随机还是有区别的。用随机数控制游戏流程，这实际是使用了广义比喻之建立联系技术，半广义不同之建立数字与字符代表的事物的联系，之使用结构体建立联系也是这个思想
{
  GtkWidget *box;
  GtkWidget *about;
  GtkWidget *table;

  gtk_init_check(&argc,&argv); /*初始化GTK+工具库*/
  window_main=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/
  
  GdkPixbuf *icon = create_pixbuf("logo.jpg");
  gtk_window_set_icon(GTK_WINDOW(window_main) , icon); 
  gtk_window_set_title(GTK_WINDOW(window_main), "东方不挂组英语学习系统");
  gtk_widget_set_size_request(window_main, 210, 300);
  gtk_window_set_resizable(GTK_WINDOW(window_main), FALSE);
  gtk_window_set_position(GTK_WINDOW(window_main), GTK_WIN_POS_CENTER);
  g_signal_connect(G_OBJECT(window_main),"delete_event",G_CALLBACK(main_close),NULL); /*登记窗体delete_event信号*/
  box=gtk_vbox_new(FALSE,0);/*创建组合框*/
  table=gtk_table_new(11,3,TRUE);

  game=gtk_button_new_with_label("Game");/*创建按钮*/
  //gtk_box_pack_start(GTK_BOX(box),game,FALSE,FALSE,5);/*把按钮加入组合框*/
  g_signal_connect(G_OBJECT(game),"pressed",G_CALLBACK(word_main),window_main);
  //gtk_widget_show(game);/*显示按钮*/
  
  store=gtk_button_new_with_label("Store");/*创建按钮*/
  //gtk_box_pack_start(GTK_BOX(box),store,FALSE,FALSE,5);/*把按钮加入组合框*/
  g_signal_connect(G_OBJECT(store),"pressed",G_CALLBACK(store_main),window_main);
  //gtk_widget_show(store);/*显示按钮*/

  about=gtk_button_new_with_label("About");/*创建按钮*/
  //gtk_box_pack_start(GTK_BOX(box),about,FALSE,FALSE,5);/*把按钮加入组合框*/
  g_signal_connect(G_OBJECT(about),"pressed",G_CALLBACK(about_system),window_main);
  //gtk_widget_show(about);/*显示按钮*/

  gtk_table_attach_defaults(GTK_TABLE(table), game, 0,1,10,11);
  gtk_table_attach_defaults(GTK_TABLE(table), store, 1,2,10,11);
  gtk_table_attach_defaults(GTK_TABLE(table), about, 2,3,10,11);
  
  GtkWidget *image = gtk_image_new_from_file("logo.jpg"); 
  gtk_table_attach_defaults(GTK_TABLE(table), image, 0,3,0,10);
  
  
  gtk_box_pack_start(GTK_BOX(box), table, FALSE, TRUE, 3);
  
  gtk_container_add(GTK_CONTAINER(window_main),box);/*把组合框加入窗体*/
  gtk_widget_show(box);/*显示组合框*/
  gtk_widget_show_all(window_main);/*显示窗体*/
  
  player=create_player(player);
  
  gtk_main();

  return 0;
}
