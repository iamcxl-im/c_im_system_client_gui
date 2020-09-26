//
// Created by 全 on 2020/9/22.
//
//消息类
#include <time.h>
#ifndef IM_SYSTEM_MESSAGE_H
#define IM_SYSTEM_MESSAGE_H
typedef struct Mess
{
    char user[16];
    char sendObject[16];//发送对象
    char news[128];
    char date[32];
    int function;//指令 1为消息 2为登陆 3为注册 4为退出 5服务器允许登陆 默认为0
} Mess;
//消息转json
char * Mess_toJSON(Mess  *Mess);
//json 转消息
Mess Mess_getMess(const char *value);
//打印mess
void Mess_print(Mess);
#endif //IM_SYSTEM_MESSAGE_H
