/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：parser.c
 *用途:语义划分
 **********************************/
#include "parser.h"

int parser(char *string,char arg[ARG_NUM][MAX_LEN])
{
   int i=0,sp=0;
   //除去指令前空格
   while(string[i]==' '||string[i]=='\t')
       i++; 
   while(string[i]!='\0'&&string[i]!='\n')
   {
       int k=0;
       //除去多余空格
       while(string[i]==' '||string[i]=='\t') i++;
       while(string[i]!=' '&&string[i]!='\t'&&string[i]!='\0'&&string[i]!='\n')
       {
           arg[sp][k++]=string[i++];
       }
       //若k为0,则输入指令最后有空格 
       if(k==0) 
           arg[sp][k]='\0';
       else
       	   arg[sp++][k]='\0';
   }   
   //返回参数个数
   return sp;
}

//显示划分结果
void show(char arg[ARG_NUM][MAX_LEN],int k)
{
    int i=0;
    printf("instruction:\n");
    for(i=0;i<k;i++)
    {
        int j=0;
        while(arg[i][j]!='\0') printf("%c",arg[i][j++]);
        printf("\n");
    }
}
