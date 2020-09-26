//
// Created by 全 on 2020/9/22.
//

#include "message.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
/**
 * 消息转JSON
 * @param mess 消息
 * @return json string
 */
char * Mess_toJSON(Mess  *mess){
    cJSON *root = cJSON_CreateObject();
    //user
    if (strlen(mess->user)==0){
        cJSON_AddItemToObject(root, "user",cJSON_CreateNull());
    } else{
        cJSON_AddItemToObject(root,"user",cJSON_CreateString(mess->user));
    }
    //sendObject
    if (strlen(mess->sendObject)==0){
        cJSON_AddItemToObject(root, "sendObject",cJSON_CreateNull());
    } else{
        cJSON_AddItemToObject(root,"sendObject",cJSON_CreateString(mess->sendObject));
    }
    //news
    if (strlen(mess->news)==0){
        cJSON_AddItemToObject(root, "news",cJSON_CreateNull());
    } else{
        cJSON_AddItemToObject(root,"news",cJSON_CreateString(mess->news));
    }
    //date
    if (strlen(mess->date)==0){
        cJSON_AddItemToObject(root, "date",cJSON_CreateNull());
    } else{
        cJSON_AddItemToObject(root,"date",cJSON_CreateString(mess->date));
    }
    //function
    if (mess->function==0){
        cJSON_AddItemToObject(root, "news",cJSON_CreateNull());
    } else{
        cJSON_AddItemToObject(root,"function",cJSON_CreateNumber(mess->function));
    }
    char *string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return string;
}

/**
 * json转消息
 * @param value string json
 * @return 消息
 */
Mess Mess_getMess(const char *value){
//    printf("解析函数%s\n",value);
    Mess mess={};
    cJSON * root = cJSON_Parse(value);
    if(!root) {
        printf("解析错误!!!!");
        return mess;
    }
    cJSON *user;
    cJSON *sendObject;
    cJSON *news;
    cJSON *date;
    cJSON *function;

    user = cJSON_GetObjectItem(root, "user");
    sendObject = cJSON_GetObjectItem(root, "sendObject");
    news = cJSON_GetObjectItem(root, "news");
    date = cJSON_GetObjectItem(root, "date");
    function = cJSON_GetObjectItem(root, "function");
    if (!cJSON_IsNull(user)){
        strcpy(mess.user,user->valuestring);
    }
    if (!cJSON_IsNull(sendObject)){
        strcpy(mess.sendObject,sendObject->valuestring);
    }
    if (!cJSON_IsNull(news)){
        strcpy(mess.news,news->valuestring);
    }
    if (!cJSON_IsNull(date)){
        strcpy(mess.date,date->valuestring);
    }
    if (!cJSON_IsNull(function)){
        mess.function=function->valueint;
    }
    cJSON_Delete(root);
    return mess;
}

/**
 * 打印消息
 * @param mess 消息对象
 */
void Mess_print(Mess mess){
    printf("用户:%s\t\t发送目标:%s\t\t发送消息:%s\t\t时间:%s\t\t功能:%d\n",mess.user,mess.sendObject,mess.news,mess.date,mess.function);
}