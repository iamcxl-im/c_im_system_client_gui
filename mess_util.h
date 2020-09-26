//
// Created by daquan on 9/25/20.
//

#ifndef IMGUI_MESS_UTIL_H
#define IMGUI_MESS_UTIL_H

char* user_login(const char* user,const char* password);
char* user_register(char* user,char* password);
int is_login(char* data);
char* sendMessage(char* user,const char* sendObject,const char* mess);
void receive_json_to_mess_print(char* string_input,char *string_out);

#endif //IMGUI_MESS_UTIL_H
