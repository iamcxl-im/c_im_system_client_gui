//
// Created by daquan on 9/25/20.
//

#include "mess_util.h"
#include "message.h"
#include "string.h"
#include <time.h>

char* user_login(const char* user,const char* password){
    Mess mess = {.function=2};
    strcpy(mess.user,user);
    strcpy(mess.news,password);
    return Mess_toJSON(&mess);
}
char* user_register(char* user,char* password){
    Mess mess = {.function=3};
    strcpy(mess.user,user);
    strcpy(mess.news,password);
    return Mess_toJSON(&mess);
}

/**
 *
 * @param data
 * @return 0success -1error
 */
int is_login(char* data){
    Mess mess = Mess_getMess(data);
    if (mess.function==5)
        return 0;
    return -1;
}

/**
 *
 * @param user
 * @param sendObject fa shong mu biao
 * @param mess
 * @return
 */
char* sendMessage(char* user,const char* sendObject,const char* mess){
    Mess mes = {.function=1};
    strcpy(mes.user,user);
    strcpy(mes.news,mess);
    strcpy(mes.sendObject,sendObject);
    struct tm *p;
    char timestr[128]="";
    time_t app_time_stamp = 0;
    app_time_stamp = time( 0 );
    p=gmtime(&app_time_stamp); //无时区信息
    strftime(timestr, 128, "%Y-%m-%d %H:%M:%S", p);
    strcpy(mes.date,timestr);
    return Mess_toJSON(&mes);
}

void receive_json_to_mess_print(char* string_input,char *string_out){
    Mess mess= Mess_getMess(string_input);
    if (mess.function!=1){
        return;
    }
    strcpy(string_out,mess.user);
    strcat(string_out,"\t\t");
    strcat(string_out,mess.date);
    strcat(string_out,"\n");
    strcat(string_out,mess.news);
}