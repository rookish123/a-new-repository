#pragma once
#ifndef BANK_H
#define BANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* 常量定义 */
#define N_MAXREC_NO   100
#define FILE_PATH     "D:\\c语言实践作业\\bank\\account.txt"
#define USER_NAME     "admin"
#define USER_PWD      "admin"
#define MAX_TRY       3

/* 结构体定义 */
typedef struct record {
    int    accNumber;      /* 账号 */
    char   accName[9];     /* 户名（8字符+结束符） */
    char   accPWD[7];      /* 密码（6字符+结束符） */
    double balance;        /* 余额 */
    char   flag[2];        /* 状态："0"正常，"1"销户 */
} RECORD;

/* 全局变量声明 */
extern RECORD rs[];
extern int maxAccount;

/* 服务模块函数声明 */
int  readData(void);
int  writeData(void);
int  createAccNumber(void);
int  findAnAccount(int accNumber);
void dispAnAccount(int i);
void queryall(void);
int  checkInput(int flag, char* checkStr);
char* rightTrim(char* str);
void getaccName(char accName[]);

/* 业务模块函数声明 */
void openAnAccount(void);
void deposit(void);
void withdraw(void);
void query(void);
void closeAnAcount(void);
void mainmenu(void);
void wellcome(void);

#endif