/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：builtin.h
 *用途：内置myshell命令实现
 **********************************/
#ifndef BUILTIN_H
#define BUILTIN_H
#include <time.h>
#include <dirent.h>
#include "environment.h"
//cd命令
int exec_cd(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[]);
//pwd命令
int exec_pwd(int num,char arg[ARG_NUM][MAX_LEN]);
//clr命令
int exec_clr(int num,char arg[ARG_NUM][MAX_LEN]);
//time命令
int exec_time(int num,char arg[ARG_NUM][MAX_LEN]);
//quit命令
int exec_quit(int num,char arg[ARG_NUM][MAX_LEN]);
//environ命令
int exec_environ(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[]);
//help命令
int exec_help(int num,char arg[ARG_NUM][MAX_LEN]);
//echo命令
int exec_echo(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[]);
//dir命令
int exec_dir(int num,char arg[ARG_NUM][MAX_LEN]);
//set命令
int exec_set(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[]);
//unset命令
int exec_unset(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[]);
//umask命令
int exec_umask(int num,char arg[ARG_NUM][MAX_LEN]);
//mask命令
int exec_mask(int num,char arg[ARG_NUM][MAX_LEN]);
//test命令
int exec_test(int num,char arg[ARG_NUM][MAX_LEN]);
//shift命令
int exec_shift(int num,char arg[ARG_NUM][MAX_LEN]);
//bg
//fg
//jobs
//判断参数是否为整数
int isnumber(char *s);
#endif
