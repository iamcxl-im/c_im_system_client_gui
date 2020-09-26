//
// Created by daquan on 9/24/20.
//
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "mess_util.h"


#define PORT 6666
#define IP_ADDR "47.94.13.255"
#define BUFFLEN 256 //读取消息长度

int server_fd;
char buffer[BUFFLEN]={0};//xie数据 数组
char pri_out[128]={0};
GtkWidget *loginWindow;
GtkWidget *list;
GtkWidget *window;
char user[16]={0};

enum{  LIST_ITEM = 0,  N_COLUMNS};
typedef struct {
    GtkWidget *entry_a;
    GtkWidget *entry_b;
}entry_mess;

/**
 * 关闭页面
 */
void closApp(){
    gtk_main_quit();
}

/**
 * 登陆按钮处理事件
 * @param button 按钮
 * @param data entry_mess结构体积指针 包含用户和密码输入框指针
 */
void button_login_clicked(GtkWidget *button,gpointer data){
    const char *str_user = gtk_entry_get_text(GTK_ENTRY(((entry_mess*)data)->entry_a));
    const char *str_pass = gtk_entry_get_text(GTK_ENTRY(((entry_mess*)data)->entry_b));
//    printf("uers:%s--------password:%s\n",str_user,str_pass);

    char* string = user_login(str_user,str_pass);
//    printf("send mess:%s\n",string);
    int n = write(server_fd,string, strlen(string)*sizeof(char));
//    printf("push---------n:%d\n",n);
    //写入登陆指令
    write(server_fd,"\n", strlen("\n")*sizeof(char));

    memset(buffer,0,BUFFLEN);//归0
    int mm = read(server_fd,buffer,BUFFLEN);

    if(mm==-1){
        printf("client is close\n");
        return;
    }
    int m = is_login(buffer);
//    printf("=========%d\n",m);
    if (m==0){
        strcpy(user,str_user);
        printf("success\n");
        gtk_widget_destroy(loginWindow);
        closApp();
    } else{
        //登陆失败 全部退出
        printf("error\n");
        gtk_widget_destroy(loginWindow);
        closApp();
        close(server_fd);
        _exit(0);
    }
}

/**
 * 主页面初始化方法 0,0 会生成最小高度
 * @param title 主页面title
 * @param width 主页面宽
 * @param height 主页面高
 * @return 主页面指针
 */
GtkWidget * init_main_windows(char* title,int width,int height){
    GtkWidget  *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),title);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window),width,height);
    g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(closApp),NULL);
    return window;
}

/**
 * 主页面内容生成
 * @param window 主页面
 * @param entryMess entry_mess结构体积指针 包含用户和密码输入框指针
 */
void init_main_windows_view(GtkWidget *window,entry_mess *entryMess){
    GtkWidget *username_label,*password_label;
    GtkWidget *username_entry,*password_entry;
    GtkWidget *ok_button;
    GtkWidget *hbox1,*hbox2;
    GtkWidget *vbox1;
    //文本框
    username_label = gtk_label_new("user:");
    password_label = gtk_label_new("password:");
    //输入框
    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();
    //设置为不可见
    gtk_entry_set_visibility(GTK_ENTRY(password_entry),FALSE);

    //声明按钮 增加回调事件
    ok_button = gtk_button_new_with_label("login");
    entryMess->entry_a = username_entry;
    entryMess->entry_b = password_entry;
    g_signal_connect(GTK_OBJECT(ok_button),"clicked",GTK_SIGNAL_FUNC(button_login_clicked),entryMess);

    /*
     * 布局
     */
    hbox1 = gtk_hbox_new(TRUE,5);
    hbox2 = gtk_hbox_new(TRUE,5);
    vbox1 = gtk_vbox_new(TRUE,10);

    gtk_box_pack_start(GTK_BOX(hbox1),username_label,FALSE,TRUE,2);
    gtk_box_pack_start(GTK_BOX(hbox1),username_entry,TRUE,TRUE,2);

    gtk_box_pack_start(GTK_BOX(hbox2),password_label,FALSE,TRUE,2);
    gtk_box_pack_start(GTK_BOX(hbox2),password_entry,TRUE,TRUE,2);

    gtk_box_pack_start(GTK_BOX(vbox1),hbox1,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(vbox1),hbox2,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(vbox1),ok_button,TRUE,TRUE,0);


    //添加到容器
    gtk_container_add(GTK_CONTAINER(window),vbox1);
}

/**
 * list 始化
 * @param list
 */
static void init_list(GtkWidget *list){
    GtkCellRenderer    *renderer;
    GtkTreeViewColumn  *column;
    GtkListStore       *store;
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("List Item",renderer, "text", LIST_ITEM, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW (list), column);
    store = gtk_list_store_new (N_COLUMNS, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW (list), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

/**
 * list 消息增加
 * @param widget list
 * @param entry string 消息
 */
static void append_item_string(GtkWidget *widget, gpointer entry){
    GtkListStore *store;
    GtkTreeIter  iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, LIST_ITEM, (char*)entry, -1);
}

/**
 * 按钮点击事件
 * @param button 按钮
 * @param data entry_mess结构体积指针 包含用户和消息输入框指针
 */
void button_send_message(GtkWidget *button,gpointer data){
    const char *str_sender = gtk_entry_get_text(GTK_ENTRY(((entry_mess*)data)->entry_a));
    const char *str_mess = gtk_entry_get_text(GTK_ENTRY(((entry_mess*)data)->entry_b));
//    printf("senduers:%s--------mess:%s\n",str_sender,str_mess);

    char *string = sendMessage(user,str_sender,str_mess);
//    printf("send mess:%s\n",string);

    int n = write(server_fd,string, strlen(string)*sizeof(char));

    if(n==-1){
        printf("连接断开\n");
        /**
         * 关闭连接
         */
        gtk_widget_destroy(window);
        closApp();
        close(server_fd);
        _exit(0);
    } else{
        strcpy(pri_out,"\t\t\t\t\t\t\t\t\t\t");
        strcat(pri_out,str_mess);
        append_item_string(list,pri_out);
    }
}

/**
 * 聊天页面内容生成
 * @param window im聊天页面
 * @param entryMess entry_mess结构体积指针 包含用户和消息输入框指针
 */
void init_im_windows_view(GtkWidget *window,entry_mess *entryMess){
    GtkWidget *sw;
    GtkWidget *add;
    GtkWidget *entry_user;
    GtkWidget *entry_message;
    GtkWidget *vbox;
    GtkWidget *hbox;

    sw = gtk_scrolled_window_new(NULL, NULL);
    list = gtk_tree_view_new();
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),GTK_SHADOW_ETCHED_IN);

    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 5);
    hbox = gtk_hbox_new(TRUE, 5);
    add = gtk_button_new_with_label("send");
    entry_user = gtk_entry_new();
    entry_message = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), entry_user, FALSE, TRUE, 3);
    gtk_box_pack_start(GTK_BOX(hbox), entry_message, FALSE, TRUE, 3);

    gtk_box_pack_start(GTK_BOX(hbox), add, FALSE, TRUE, 3);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 3);
    gtk_container_add(GTK_CONTAINER (sw), list);
    gtk_container_add(GTK_CONTAINER (window), vbox);
    init_list(list);

//    g_signal_connect(G_OBJECT(add), "clicked",G_CALLBACK(append_item), &data);
    //append_item_string

//    entry_mess entryMesses = {.entry_a=entry_user,.entry_b=entry_message};

    entryMess->entry_a = entry_user;
    entryMess->entry_b = entry_message;

    g_signal_connect(G_OBJECT(add), "clicked",G_CALLBACK(button_send_message), entryMess);
    g_signal_connect (G_OBJECT (window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
}

/**
 * 网络初始化
 * @return 初始化成功失败-1 成功返回0
 */
int init_network(){
    struct sockaddr_in server_addr;
    int ret=0;

    server_fd = socket(AF_INET,SOCK_STREAM,0);

    if(server_fd < 0){
        printf("socket error\n");
        return -1;
    }
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    inet_pton(AF_INET,IP_ADDR,&server_addr.sin_addr);

    ret = connect(server_fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
    if (ret==-1){
        printf("socket connect error\n");
        return -1;
    }
    return 0;
}

/**
 * 读线程
 * @param argv
 */
static void handle_read(void *argv){
//    int server_fd = *((int*)argv);//客户端文件描述符
    char string_out[256]="";
    memset(buffer,0,BUFFLEN);//归0

    int mm = 0;
    int temp;
    char *string_temp;
    int str_temp;

    while (mm!=-1){
        memset(buffer,0,BUFFLEN);//归0
        mm = read(server_fd,buffer,BUFFLEN);
        if(mm==0){
            continue;
        }
        printf("read len:%d\n",mm);
        string_temp = buffer;
        temp=0;
        while (temp<mm){
            str_temp = strlen(string_temp)+1;
//            printf("read message len is %d\t is:%s\n",str_temp,string_temp);
            if(str_temp>1){
                receive_json_to_mess_print(string_temp,string_out);
                if(strlen(string_out)>0){
                    printf("print:%s\n",string_out);
                    append_item_string(list,string_out);
                }
                memset(string_out,0,256);
            }
            temp+=str_temp;
            string_temp+=str_temp;
        }
    }
    printf("tui chu le!!!!\n");
}

int main(int argc,char *argv[]){
//    GtkWidget *loginWindow;
    //网络初始化
    int err = init_network();
    if(err==-1){
        close(server_fd);
        return -1;
    }
    //组件初始化
    gtk_init(&argc,&argv);
    loginWindow = init_main_windows("login",0,0);
    entry_mess entryMess;//用户输入框密码输入框
    init_main_windows_view(loginWindow,&entryMess);
    //显示组件
    gtk_widget_show_all(loginWindow);
    gtk_main();

    printf("into im system\n");

    /**
     * 新窗口
     */
//    GtkWidget *window;
    ////组件初始化
    gtk_init(&argc, &argv);
    window = init_main_windows(user,370,270);
    entry_mess mess;
    init_im_windows_view(window,& mess);
    //读线程初始化
    pthread_t pthread;//读线程
    pthread_create(&pthread,NULL,(void*)handle_read,&server_fd);
    //显示组件
    gtk_widget_show_all(window);
    gtk_main ();

    close(server_fd);
    return 0;
}