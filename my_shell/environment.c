/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：environment.c
 *用途：环境操作实现
 **********************************/
#include "environment.h"

int envid;
struct env env_table[NUM_ENV];

//初始化环境
void env_init()
{
    //分配内存
    memset(env_table,0,sizeof(struct env)*NUM_ENV);
    //添加pwd，shell环境变量
    char s[MAX_LEN]="pwd";
    char pwd[MAX_LEN],temp[MAX_LEN];
    getcwd(pwd,MAX_LEN);
    env_push(env_table,s,pwd);
    strcat(pwd,"/myshell");
    env_push(env_table,"shell",pwd);
}

//添加环境变量
void env_push(struct env env_table[],char* env_name, char * env_value)
{
    int i;
    char temp[MAX_LEN];
    for(i=0;i<NUM_ENV;i++)
    {
    	//若对应的环境列表某项为空，则进行添加
       if(env_table[i].env_name[0]=='\0')
       {
           strcpy(env_table[i].env_name,env_name);
           strcpy(env_table[i].env_value,env_value);
           strcpy(temp,env_name);
           strcat(temp,"=");
           strcpy(temp,env_value);
           //进行系统调用，添加环境变量
           putenv(temp);
           break;           
       }
    }
}

//环境变量的删除
void env_delete(struct env env_table[],char* env_name)
{
    int i;
    char temp[MAX_LEN]="";
    for(i=0;i<NUM_ENV;i++)
    {
    	//查找对应变量并进行删除
   	if(!strcmp(env_name,env_table[i].env_name))
   	{
   	   strcpy(env_table[i].env_name,temp);
           strcpy(env_table[i].env_value,temp);
   	}
    }
}

//获取环境变量
char *env_get(struct env env_table[],char *env_name)
{
   int i;
   for(i=0;i<NUM_ENV;i++)
   {
   	//查找对应环境变量并进行输出
   	if(!strcmp(env_name,env_table[i].env_name))
   	    return env_table[i].env_value;
   }
   return "no such environment value.";
}

//遍历所有的环境变量
void env_show(struct env env_table[])
{
    int i;
    char temp[MAX_LEN];
    for(i=0;i<NUM_ENV;i++)
    {
    	//若存在，输出环境变量
       if(env_table[i].env_name[0]!='\0')
       {
           printf("%s=%s\n",env_table[i].env_name,env_table[i].env_value);
       }
    }
}

