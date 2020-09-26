//
// Created by daquan on 9/25/20.
//
#include "mess_util.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "mess_util.h"

#define PORT 6666
#define IP_ADDR "47.94.13.255"
#define BUFFLEN 256 //读取消息长度

int server_fd;
char buffer[BUFFLEN]={0};//xie数据 数组

int main(){
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

    char* string = user_login("0001","0001");
    printf("send mess:%s\n",string);
    int n = write(server_fd,string, strlen(string)*sizeof(char));
    printf("push---------n:%d\n",n);

    memset(buffer,0,BUFFLEN);//归0
    int mm = recv(server_fd,buffer,BUFFLEN,0);
    printf("====%d====%s\n",mm,buffer);
    int m = is_login(buffer);

    printf("=========%d\n",m);
}