/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：const.h
 *用途：定义myshell数据结构和常量
 **********************************/
#ifndef CONST_H
#define CONST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//系统调用模块
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <pwd.h>
#include <stddef.h> /* for offsetof */
#include <string.h> /* for convenience */
#include <unistd.h> /* for convenience */

//指令最大长度
#define MAX_LEN 128
//最大环境变量数
#define NUM_ENV 32
//语句包含参数个数
#define ARG_NUM 64
//显示目录下文件，每行显示数
#define LS_NUM 5
//more显示的行数
#define MORE_NUM 15
//最大支持后台进程数
#define JOB_NUM 5

//环境变量结构定义
struct env{
  char env_name[MAX_LEN];
  char env_value[MAX_LEN];
};

typedef struct env * envptr;

//建立进程列表
struct sjob{
  int jobid;
  int pid;
  char cmd[MAX_LEN];
  int state;
};

//建立环境变量列表
extern struct env env_table[NUM_ENV];
//创建进程变量列表
extern struct sjob jobs [JOB_NUM];
#endif
