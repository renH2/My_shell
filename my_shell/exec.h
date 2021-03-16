/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：exec.h
 *用途：指令，重定向，管道实现
 **********************************/
#ifndef EXEC_H
#define EXEC_H

#include "const.h"
#include "builtin.h"

//内置指令实现
void in_exec(int num,char arg[ARG_NUM][MAX_LEN]);
//外部指令实现
void out_exec(int num,char arg[ARG_NUM][MAX_LEN]);
//管道指令实现
void pipe_exec(int num, char arg[ARG_NUM][MAX_LEN]);
//清除指令
void flush_cmd(int num,char arg[ARG_NUM][MAX_LEN]);
//指令执行的main程序
void all_exec(int num,char arg[ARG_NUM][MAX_LEN]);
//jobs初始化
void jobs_init();
//添加进程
void push_job(int jid,int pid,char s[MAX_LEN]);
//删除进程
void delete_job(int jid);
//展示所有进程
void show_job();

#endif 
