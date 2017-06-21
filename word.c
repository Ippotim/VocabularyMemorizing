//#include<stdio.h>
#include<time.h>
//#include<math.h>
#include <gtk/gtk.h>//这个放在#include "gui.h"下面就会报错
#include<stdlib.h>
#include"word.h"
#include "player.h"
#include "gui.h"
#include "trade.h"
//使用数据库的好处是使用了文件化的接口，更灵活，就像数据库导入条目数据，本来是要使用相关命令一个个导入的，但数据库有借助文本文件批量导入的特性，这跟使用系统调用是一个道理的，之设立了shell提高效率。对于这里就是由于没有使用文件化接口则要改变其中的数据都要重新编译，很不灵活。事实上数据库也是人编的，之己设立文件化接口的话己这个也是数据库了。
//可加入指定起始位置顺序练习功能以及词典功能，以及该次练习错题回顾功能之用错题再练一次,道具系统也可保留，之由玩家设定初始难度，通过答题可以获取金钱并购买道具，甚至重新拓展成一个游戏之脱胎于原来的英语学习系统。这里可实现伪输入法之不断查找之随着输入字母的个数的增加不断定位到所需单词。
//这个本来是先于字符界面实现的，后来穿上图形界面的外皮之使用图形界面接管程序的输入输出而已，这也是图形界面的所谓接口
//gtk_widget_set_usize(window,COL*25,ROW*25+55);这就是窗口大小改变函数
//学好英语是为了阅读英语文献和外国人交流参加国际会议，真的可以背下一本词典里的单词
//单词练习系统精妙之处在于少生多生成了极大数量的选择题，而且利用了人们普遍的心理之单是背单词会记不牢这是因为单词和释义就躺那里我们可以不费吹灰之力就得到其对应关系而只有亲自去争取过才会懂得要珍惜,a new way to recite words
#define word_num sizeof(word)/sizeof(word[0])
int become_three=0;
int tell_me=0;
int r,question_type,selection;
int left;
int change=0;
int lock=0;//增加落子无悔特性
Word option[4];//这里可以把select并入option结构体中，之select也即玩家选择的A、B、C、D四个选项中的一个，option也即四个选项的具体内容，两者并入一个结构体的话当玩家作出选择之给select赋值系统就可以很容易地找到相应的option并进行相关评判操作。当然不并入也可以，那就是使用函数之程序逻辑建立联系了。

int were_wrong=0;
int wrong[word_num]={0};//增加错题重练功能
int is_sequence=0;
int word_start;
//int sequence[word_num]={0};//增加指定位置练习功能

int select_question;
GtkWidget *word_window;//本来是GtkWidget *window;但产生了一个提示窗口弹不出的错误，判断为找不到父窗口，改成这个则修复了这个bug，所以对共用指针要慎重
GtkWidget *button;
GtkWidget *box;
GtkWidget *question;
GtkWidget *table;
GtkWidget *A;
GtkWidget *B;
GtkWidget *C;
GtkWidget *D;
GtkWidget *tip;
GtkWidget *next;
GtkWidget *help;
gchar *color[]={
  "red","orange","yellow","green","cyan","blue","purple"	
};//这个实际就是二维数组，也可以说是二维指针，之字符串与指针颇有渊源
void add(char *a,char *b){
  while(*a != '\0')a ++;
  while((*a ++ = *b ++) != '\0');//*a = '\0';
}
int magic1()
{
  player.object.gold-=111;//使用道具受到背包物品量限制
  if(sell(1))return 1;                    //
  char become[66]="A wrong answer is: ";
  if(question_type)//使用去掉一个错误答案的锦囊
  {
    int a;
    do{a=rand()%4;}while(a==r);
    add(become,option[a].Chinese);
  }
  else
  {
    int a;
    do{a=rand()%4;}while(a==r);
    add(become,option[a].English);
  }
  gtk_entry_set_text(GTK_ENTRY(tip),become);
  return 0;
}
int magic2()
{
  player.object.gold-=111;//这句是中和sell增加的金币之只有在商店模式卖出才有金币返还，这里是使用而不是卖，只是借用卖来模拟使用，简单巧妙
  if(sell(2))return 1;
  char tell[66]="A possible answer is: ";
  if(question_type)//使用去掉一个错误答案的锦囊
  {
    int a=rand()%4;
    if(a)add(tell,option[r].Chinese);
    else add(tell,option[rand()%4].Chinese);
    
  }
  else
  {
    int a=rand()%4;
    if(a)add(tell,option[r].English);
    else add(tell,option[rand()%4].English);
  }
  gtk_entry_set_text(GTK_ENTRY(tip),tell);
  return 0;
}
GtkWidget *word_help_window;//自立门户是好的，共用指针容易导致提示窗口找不到父窗口
int word_help()
{
  GtkWidget *become_three;
  GtkWidget *tell_me;

  word_help_window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/
  gtk_window_set_title(GTK_WINDOW(word_help_window), "东方不挂组英语学习系统");
  gtk_window_set_default_size(GTK_WINDOW(word_help_window), 230, 66);
  gtk_window_set_position(GTK_WINDOW(word_help_window), GTK_WIN_POS_CENTER);
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(word_help_window,GTK_STATE_NORMAL,&c1);
  
  //g_signal_connect(G_OBJECT(word_help_window),"delete_event",G_CALLBACK(gtk_main_quit),NULL); /*登记窗体delete_event信号*/
  box=gtk_vbox_new(FALSE,0);/*创建组合框*/

  button=gtk_button_new_with_label("become_three");/*创建按钮*/
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);/*把按钮加入组合框*/
  g_signal_connect(G_OBJECT(button),"pressed",G_CALLBACK(magic1),word_help_window);
  gtk_widget_show(button);/*显示按钮*/
  
  button=gtk_button_new_with_label("tell_me");/*创建按钮*/
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);/*把按钮加入组合框*/
  g_signal_connect(G_OBJECT(button),"pressed",G_CALLBACK(magic2),word_help_window);
  gtk_widget_show(button);/*显示按钮*/

  gtk_container_add(GTK_CONTAINER(word_help_window),box);/*把组合框加入窗体*/
  gtk_widget_show(box);/*显示组合框*/
  gtk_widget_show(word_help_window);/*显示窗体*/

  return 0;
}
void select_A()
{
  selection=0;
  word_judge();
}
void select_B()
{
  selection=1;
  word_judge();
}
void select_C()
{
  selection=2;
  word_judge();
}
void select_D()
{
  selection=3;
  word_judge();
}
int word_wrong=0;//错题数
int go_on;//使错题练习可以多次进行
int level=1;//记录第若干次进行错题练习
int word_exercise_wrong()
{
  level++;
  were_wrong=0;
  word_wrong=0;
  int i;
  for(i=0;i<word_num;i++)if(wrong[i])word_wrong++;
  if(!word_wrong)return 1;//没有错题
  left=word_wrong;
  go_on=-1;//取-1是因为0是数组中合法元素
  char finish[66]="You need to answer ";
  char num[22];
  sprintf(num,"%d",left);
  add(finish,num);
  add(finish," more questions correctly.");
  gtk_entry_set_text(GTK_ENTRY(tip),finish);
  word_exercise();
  return 0;
}
void succeed()
{
  GtkWidget *dialog;//本来是在if-else里有定义的且一定会定义到，可是编译器不认
  if(were_wrong)
  {
    dialog=gtk_message_dialog_new(GTK_WINDOW(word_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "Sorry,but you were wrong！");
    gtk_window_set_title(GTK_WINDOW(dialog), "Try again");
  }
  else
  {
    dialog=gtk_message_dialog_new(GTK_WINDOW(word_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "Thanks for playing with me！");
    gtk_window_set_title(GTK_WINDOW(dialog), "Excellent");
  }
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(dialog,GTK_STATE_NORMAL,&c1);
  
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
  word_exercise_wrong();
}
int word_next()
{
  if(left)if(!lock)
  {
    if(!word_wrong)
    {
      left++;//有一个bug，之可以不答题而跳到下一题，这样会降低难度,这个是简单修复方法。之没有if(left)也会有bug
    }
    else left--;//错题练习阶段跳过或做错也减少剩余题数
    player.object.gold-=222;//跳过也是答错也要扣钱
    refresh_gold_num();
    were_wrong=1;
    wrong[select_question]=level;
  }
  lock=0;
  left+=change;
  change=0;//须复位标志位
  if(!left)
  {
    succeed();
    return 1;
  }
  char finish[66]="You need to answer ";
  char num[22];
  sprintf(num,"%d",left);
  add(finish,num);
  add(finish," more questions correctly.");
  gtk_entry_set_text(GTK_ENTRY(tip),finish);
  word_exercise();
  return 0;
}
int word_judge(GtkButton *button,gpointer data)
{
  char a[66]="Answer is： ";
  if(question_type)add(a,option[r].Chinese);
  else add(a,option[r].English);
  if(selection==r)
  {
    if(left)if(!lock)//增加if(left)修复答题完毕按了Next显示答题完毕提示窗口，关闭后再答题由于lock已复位则left会变负数的bug
    {
      change=-1;
      player.object.gold+=111;
      refresh_gold_num();
      lock=1;
    }
    add(a,"      Well done!");
    wrong[select_question]=0;
    gtk_entry_set_text(GTK_ENTRY(tip),a);
  }
  else 
  {
    if(left)if(!lock)
    {
      if(!word_wrong)change=+1;//if(!word_wrong)是让练习错题阶段不再罚题
      else change=-1;//错题练习阶段跳过或做错也减少剩余题数
      were_wrong=1;
      player.object.gold-=222;
      refresh_gold_num();
      lock=1;
    }
    add(a,"      Wrong......");
    wrong[select_question]=level;
    gtk_entry_set_text(GTK_ENTRY(tip),a);
  }
}
int word_exercise()
{
  //int select_question;//本来是int question;这个在没有写图形界面时没有错，问题是图形界面有GtkWidget *question;这样给question赋值似乎就会出错，之编译器认为int question;是局部变量则没有报错么。与似乎这也说明了段错误往往就是给指针有意无意错误赋值
  if(is_sequence)
  {
    select_question=word_start++;    
  }
  else select_question=rand()%word_num;
  if(word_wrong)//当有很多功能需要介入select_question选取，则select_question选取可独立出来供函数按需调用，也即排列组合之少生多，而不是写一个体现排列组合可得的所有可能的结果的过程之低层级罗列所有if，这样既容易出错又麻烦，应高层级之高效。这样耦合度低可扩展性可维护性强
  {
    int i;
    int wrong_done=1;//防止wrog[]遍历完后由于已没有该level的错题而依然执行后续语句
    for(i=0;i<word_num;i++)
      if(wrong[i]&&(wrong[i]<level))
      {
        if(i==go_on)continue;
        go_on=i;
        select_question=i;
        wrong_done=0;
      }
    if(wrong_done)return 1;
  }
  
  //printf("%d",select_question);//为什么没设置srand仍可每次获得不同的随机数序列，关键是在main函数中调用rand()%7却总是获得同一个值
  
  question_type=rand()%2;
  if(question_type)//使用随机数少生多生成复杂性,这里其实可以压缩代码之把这两种情况用广义变量之合为一种半广义情况，一种思路是解决一个技术问题之构造元素变量之似word[i].X之X可为word结构体中的某一元素之所谓变量的变量，不过除了typedef或define似乎没什么可以做到这个之毕竟编译器认为.后就是元素变量。当然达到相当的效果也可以了，首先这也是无编码化集合元素编码化操作，之毕竟结构体中的元素就是离散的元素，所以可以使用此技术进行处理；然后可以在word结构体中增加一个元素之就是X，在这里就是char X[88]，不过当结构体或者说集合中的元素的数据类型当然可以不同的所以的话这样就不能处理一般的情况了，这样就又需要一种类似面向对象程序设计语言中的容器的技术之使得X可以成为变量的变量。这里就是可以让X选择性被word结构体中的其他元素赋值，这样就是间接选取了word结构体内的元素之实现了变量的变量。这些就是跟模拟跟广义运算似乎有些不同的纯净的技术，很耐人寻味。由于像变量的变量这样的技术得先要想得到这样的概念，拥有高层级思想的可怕之处就在于此，这种特性或许是本来此程序设计语言没有的但可以用现有的特性模拟出来，而这种想法明显是基于集合操作提出的，集合就是高层级思想的范畴，这样说来巧妙的技术就是为高层级思想而实现的，之毕竟思想才是最强大的语言。
  {
    int i;
    r=rand()%4;
    //printf("%s\n",word[select_question].English);
    
    gtk_label_set_text(GTK_LABEL(question),word[select_question].English);
    
    option[r]=word[select_question];
    for(i=0;i<4;i++)
    {
      if(i==r)continue;
      int fill_up;
      int puppet[word_num]={0};
      puppet[select_question]=1;
      do{
        fill_up=rand()%word_num;
      }while(puppet[fill_up]==1);//判断语句尤其要注意=与==，之往往是后者之一般不会是赋值
      puppet[fill_up]=1;
      option[i]=word[fill_up];
    }
    //for(i=0;i<4;i++)printf("%s\t",option[i].Chinese);
    
    gtk_button_set_label(GTK_BUTTON(A),option[0].Chinese);
    gtk_button_set_label(GTK_BUTTON(B),option[1].Chinese);
    gtk_button_set_label(GTK_BUTTON(C),option[2].Chinese);
    gtk_button_set_label(GTK_BUTTON(D),option[3].Chinese);
    
  }
  else
  {
    int i;
    r=rand()%4;//本来是int i,r=rand()%4;这样就是重定义r了，所以程序逻辑出错
    //printf("%s\n",word[select_question].Chinese);
    
    gtk_label_set_text(GTK_LABEL(question),word[select_question].Chinese);
    
    option[r]=word[select_question];
    for(i=0;i<4;i++)
    {
      if(i==r)continue;
      int fill_up;
      int puppet[word_num]={0};
      puppet[select_question]=1;
      do{
        fill_up=rand()%word_num;
      }while(puppet[fill_up]==1);
      puppet[fill_up]=1;
      option[i]=word[fill_up];
    }
    //for(i=0;i<4;i++)printf("%s\t",option[i].English);
    
    gtk_button_set_label(GTK_BUTTON(A),option[0].English);
    gtk_button_set_label(GTK_BUTTON(B),option[1].English);
    gtk_button_set_label(GTK_BUTTON(C),option[2].English);
    gtk_button_set_label(GTK_BUTTON(D),option[3].English);
    
  }
  return 0;
}
void play_more()
{
  GtkWidget *dialog=gtk_message_dialog_new(GTK_WINDOW(word_window),
  GTK_DIALOG_DESTROY_WITH_PARENT,
  GTK_MESSAGE_INFO,
  GTK_BUTTONS_OK,
  "Please go on playing with me！");
  gtk_window_set_title(GTK_WINDOW(dialog), "Q_Q");
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(dialog,GTK_STATE_NORMAL,&c1);
  
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}
int word_close()
{
  if(left)
  {
    play_more();
    return(1);
  }
  gtk_widget_destroy(GTK_WIDGET(word_window));
  return 0;
}

GtkWidget *window;
GtkWidget *button;
GtkWidget *box;
GtkWidget *label;
GtkWidget *num;

gint button_event()
{
  const gchar *_str;
  _str=gtk_entry_get_text(GTK_ENTRY(num));
  char *str=(char*)_str;//强制类型转换就可以，不需要进行数组元素复制操作
  //printf("%s\n",str);
  if(str[0]=='S')
  {
    //printf("%s\n",str);
    str[0]=' ';//简单有效去掉第一个字符，而不必大动干戈之进行数组大量元素移动操作
    //printf("%s\n",str);
    word_start=atoi(str);
    if(word_start<=0||word_start>=word_num)
    {
      str[0]='S';//是中和str[0]=' ';
      return 1;
    }
    is_sequence=1;
    left=word_num-word_start;
    //int i;
    //for(i=word_start-1;i<word_num;i++)sequence[i]=1;
  }
  else
  {
    left=atoi(str);
    if(left<=0)return 1;//这样就不必伤脑筋判断输入者是否数字了
  }	
  gtk_widget_destroy(GTK_WIDGET(window));
  second_main();
  return 0;
}
void word_main_close()
{
  gtk_widget_destroy(GTK_WIDGET(window));
}
int word_main()
{ 
  //gtk_init(&argc, &argv); 
  window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/
  gtk_window_set_title(GTK_WINDOW(window), "东方不挂组英语学习系统");
  gtk_window_set_default_size(GTK_WINDOW(window), 222, 66);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(G_OBJECT(window),"delete_event",word_main_close,NULL); /*登记窗体delete_event信号*/
  
  srand((unsigned)time(NULL));
  
  GdkColor c1;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(window,GTK_STATE_NORMAL,&c1);
  
  box=gtk_vbox_new(FALSE,0);/*创建组合框*/

  label=gtk_label_new("Input the quantity of exercises:");/*创建标号*/
  gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,5);/*把标号加入组合框*/
  gtk_widget_show(label);/*显示标号*/
  
  GdkColor c2;
  do{gdk_color_parse (color[rand()%7], &c2);}while(c2.red==c1.red&&c2.green==c1.green&&c2.blue==c1.blue);
  gtk_widget_modify_fg(label,GTK_STATE_NORMAL,&c2);
  
  num=gtk_entry_new();/*创建录入构件*/
  gtk_box_pack_start(GTK_BOX(box),num,FALSE,FALSE,5);/*把录入构件加入组合框*/
  gtk_widget_show(num);/*显示录入构件*/
  
  button=gtk_button_new_with_label("Start！");/*创建按钮*/
  gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);/*把按钮加入组合框*/
  g_signal_connect(G_OBJECT(button),"pressed",G_CALLBACK(button_event),window);
  gtk_widget_show(button);/*显示按钮*/

  gtk_container_add(GTK_CONTAINER(window),box);/*把组合框加入窗体*/
  gtk_widget_show(box);/*显示组合框*/
  gtk_widget_show(window);/*显示窗体*/
  //gtk_main();
}

gint second_main()//单词练习界面,通过实现这个图形界面实际就是攻克了图形界面与程序内核输入输出进行交互的技术
{
  
  //gtk_init(&argc, &argv); 
  
  word_window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/
  gtk_window_set_title(GTK_WINDOW(word_window), "东方不挂组英语学习系统");
  gtk_window_set_default_size(GTK_WINDOW(word_window), 400, 150);
  gtk_window_set_position(GTK_WINDOW(word_window), GTK_WIN_POS_CENTER);
  g_signal_connect(G_OBJECT(word_window),"delete_event",G_CALLBACK(word_close),NULL); /*登记窗体delete_event信号*/
  box=gtk_vbox_new(FALSE,0);/*创建组合框*/
  table=gtk_table_new(2,2,TRUE);
  
  question=gtk_label_new("Question:");/*创建标号*/
  gtk_box_pack_start(GTK_BOX(box),question,FALSE,FALSE,5);/*把标号加入组合框*/
  gtk_widget_show(question);/*显示标号*/
  
  A=gtk_button_new_with_label("A.");
  B=gtk_button_new_with_label("B.");
  C=gtk_button_new_with_label("C.");
  D=gtk_button_new_with_label("D.");
  
  GdkColor c1,c2;
  //c1.red=0xffff; c1.blue=0; c1.green=0;
  gdk_color_parse (color[rand()%7], &c1);
  gtk_widget_modify_bg(word_window,GTK_STATE_NORMAL,&c1);
  do{gdk_color_parse (color[rand()%7], &c2);}while(c2.red==c1.red&&c2.green==c1.green&&c2.blue==c1.blue);
  gtk_widget_modify_fg (question, GTK_STATE_NORMAL, &c2);

  
  gtk_table_attach_defaults(GTK_TABLE(table), A, 0,1,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table), B, 1,2,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table), C, 0,1,1,2);
  gtk_table_attach_defaults(GTK_TABLE(table), D, 1,2,1,2);
  
  gtk_box_pack_start(GTK_BOX(box), table, FALSE, TRUE, 3);
  
  tip=gtk_entry_new();/*创建录入构件*/
  gtk_box_pack_start(GTK_BOX(box),tip,FALSE,FALSE,5);/*把录入构件加入组合框*/
  gtk_widget_show(tip);/*显示录入构件*/
  
  GtkWidget *label_next,*label_help;
  next=gtk_button_new();
  help=gtk_button_new();
  label_next=gtk_label_new("Next");
  label_help=gtk_label_new("Help");
  gtk_container_add(GTK_CONTAINER(next),label_next);
  gtk_container_add(GTK_CONTAINER(help),label_help);
  gtk_widget_modify_fg(label_next,GTK_STATE_NORMAL,&c1);
  gtk_widget_modify_fg(label_help,GTK_STATE_NORMAL,&c1);
  
  gtk_box_pack_start(GTK_BOX(box),next,FALSE,FALSE,5);/*把标号加入组合框*/
  gtk_box_pack_start(GTK_BOX(box),help,FALSE,FALSE,5);/*把标号加入组合框*/
  
  g_signal_connect(G_OBJECT(A),"pressed",G_CALLBACK(select_A),word_window);
  g_signal_connect(G_OBJECT(B),"pressed",G_CALLBACK(select_B),word_window);
  g_signal_connect(G_OBJECT(C),"pressed",G_CALLBACK(select_C),word_window);
  g_signal_connect(G_OBJECT(D),"pressed",G_CALLBACK(select_D),word_window);
  g_signal_connect(G_OBJECT(next),"pressed",G_CALLBACK(word_next),word_window);
  g_signal_connect(G_OBJECT(help),"pressed",G_CALLBACK(word_help),word_window);
  
  gtk_container_add(GTK_CONTAINER (word_window), box);
  gtk_widget_show_all(word_window);
  
  word_exercise();
  //gtk_main();
}
