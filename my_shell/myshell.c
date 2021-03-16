/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：myshell.c
 *用途：主程序
 **********************************/

//外部模块
#include "const.h"
#include "environment.h"
#include "parser.h"
#include "builtin.h"
#include "exec.h"

//获取用户名
void get_uname(char *s);
//获取路径
void get_path(char *s);
//获取当前路径
void get_pwd(char *s);
//更新信号
void signal_init();
//中断进程
void kill_job();

//后台进程数
extern int job=0;
struct sjob jobs [JOB_NUM];
//掩码
extern char shell_umask[20]="0002";
//指令
char cmd[MAX_LEN];
//参数解析表
char arg[ARG_NUM][MAX_LEN];
//当前目录，路径，使用者
char pwd[MAX_LEN];
char path[MAX_LEN];
char uname[20];
struct env env_table[NUM_ENV];


int main(int argc, char *argv[])
{
    int arg_num=0;
    char* s=malloc(MAX_LEN);
    //参数个数为1,为命令行输入模式
    if(argc==1)
    {
        printf("\033[31m----My Shell----\033[0m\n");
        env_init();
        signal_init();
        jobs_init();
        while (1){
          //获取当前目录，用户名，主目录等信息并输出shell提示
          get_pwd(pwd);
          get_uname(uname);
          get_path(path);
          printf("\033[33m%s@%s: \033[0m",uname,pwd);
          //清空输入缓冲区
          fflush(stdin);
          //读入指令
          fgets(cmd,MAX_LEN,stdin);
          //语义划分
          arg_num=parser(cmd,arg);
          //重定向
          int cost=0;
          char kk[MAX_LEN]="";
          for(int i=0;i<arg_num;i++)
          {
          	if(!strcmp(arg[i],"<"))
          	{
          	     i++;
                     if(!freopen(arg[i],"r",stdin))
          	           printf("无法打开文件%s",arg[i]);
          	     else
          	     {
          	     	strcpy(arg[i-1],kk);
          	     	strcpy(arg[i],kk);
          	     	cost+=2;
          	     }
          	}
          	else if(!strcmp(arg[i],">"))
          	{
          	     i++;
                     if(!freopen(arg[i],"w",stdout))
          	           printf("无法打开文件%s",arg[i]);
          	     else
          	     {
          	     	strcpy(arg[i-1],kk);
          	     	strcpy(arg[i],kk);
          	     	cost+=2;
          	     }
          	}
          	if(!strcmp(arg[i],">>"))
          	{
          	     i++;
                     if(!freopen(arg[i],"a",stdout))
          	             printf("无法打开文件%s",arg[i]);
          	     else
          	     {
          	     	strcpy(arg[i-1],kk);
          	     	strcpy(arg[i],kk);
          	     	cost+=2;
          	     }
          	}
          }
          arg_num=arg_num-cost;
          //执行指令
          pipe_exec(arg_num,arg);
          //清除指令
          flush_cmd(arg_num,arg);    
	  freopen("/dev/tty","r",stdin);
	  freopen("/dev/tty","w",stdout);                                                   
         }
     }
    else
    {
    	FILE *fp=fopen(argv[1],"r");
    	//文件打开失败
    	if(fp==NULL)
    	{
           printf("不存在文件%s\n",argv[1]);
    	   exit(0);
    	}
        env_init(env_table);
        env_show(env_table);
        while (1)
        {
          //读取指令
          if(fgets(cmd,MAX_LEN,fp)==0) 
          	return 0;
          //划分指令
          arg_num=parser(cmd,arg);
          //执行指令
          all_exec(arg_num,arg);
          printf("\033[33m---------------------------------------------------\033[0m\n");
        }
   }
   return 0;
}

void get_uname(char *s)
{
   struct passwd * temp=getpwuid(getuid());
   strcpy(s,temp->pw_name);
   if(s==NULL){
       printf("用户名称获取失败！");
   }   
}

void get_path(char *s)
{
    struct passwd * temp=getpwuid(getuid());
    strcpy(s,temp->pw_dir);
}

void get_pwd(char *s)
{
   getcwd(s,MAX_LEN);
}

void signal_init()
{
   signal(SIGTSTP,kill_job);
}

void kill_job()
{
   int i=0;
   for(;i<JOB_NUM;i++)
   {
       	if(jobs[i].cmd[0]!='\0')
        {
              if(jobs[i].state==0)
                   kill(jobs[i].pid,SIGSTOP);
              jobs[i].state=1;
              //show_job(jobs);
              printf("中断 : %s\n",jobs[i].cmd);
        }
   }
}

