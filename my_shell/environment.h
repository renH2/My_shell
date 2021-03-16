/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：environment.h
 *用途：环境操作实现
 **********************************/
#ifndef ENV_H
#define ENV_H

#include "const.h"

//初始化环境
void env_init();
//添加环境变量
void env_push(struct env env_table[],char* env_name, char * env_value);
//删除环境变量
void env_delete(struct env env_table[],char* env_name);
//获取环境变量的值
char * env_get(struct env env_table[],char *env_name);
//显示环境变量
void env_show(struct env env_table[]);

#endif
