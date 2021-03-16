/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：parser.h
 *用途:语义划分
 **********************************/
#ifndef PARSER_H
#define PARSER_H

#include "const.h"

//语义划分
int parser(char *string,char arg[ARG_NUM][MAX_LEN]);
//显示划分结果
void show(char arg[ARG_NUM][MAX_LEN],int k);

#endif
