#ifndef AUTH_H
#define AUTH_H

#include "common.h"

int login(char *username);
int signup(char *username);
void getmasked_password(char *pass, int maxlen);
int user_exist(char *username);
void save_userdata(struct user *u);
void load_userdata(char *username, user *u);

#endif