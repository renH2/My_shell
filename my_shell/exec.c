/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：exec.c
 *用途：指令，重定向，管道实现
 **********************************/
#include "exec.h"

//环境变量列表
struct env env_table[NUM_ENV];
//进程列表
struct sjob jobs [JOB_NUM];

//初始化进程表
void jobs_init()
{
   //初始化内存为0
   memset(jobs,0,sizeof(struct sjob)*JOB_NUM);
}

void push_job(int jid,int pid,char s[MAX_LEN])
{
   int i;
   for(i=0;i<JOB_NUM;i++)
   {
      //寻找进程表空位置
      if(!strcmp(jobs[i].cmd,""))
      {
      	//添加进程队列号，进程PID，相应进程
       	jobs[i].jobid=jid;
       	jobs[i].pid=pid;
       	strcpy(jobs[i].cmd,s);
       	break;
      }
   }
}

//删除进程
void delete_job(int jid)
{
   int i;
   char s[MAX_LEN]="";
   for(i=0;i<JOB_NUM;i++)
   {
      //删除队列号对应的进程
      if(jobs[i].jobid==jid)
      {
       	jobs[i].jobid=0;
       	jobs[i].pid=0;
       	jobs[i].state=0;
       	strcpy(jobs[i].cmd,s);
       	break;
      }
   }
}

//显示进程表
void show_job()
{
   int i;
   printf("------jobs------\n");
   for(i=0;i<JOB_NUM;i++)
   {
      if(jobs[i].cmd[0]!='\0')
      {
          //根据进程的状态进行输出
          if(jobs[i].state==0)
          	printf("[%d]%d  进行中  %s\n",jobs[i].jobid,jobs[i].pid,jobs[i].cmd);
          else 
          	printf("[%d]%d  中断中  %s\n",jobs[i].jobid,jobs[i].pid,jobs[i].cmd);
      }
   }
}

//内部指令状态机
void in_exec(int arg_num,char arg[ARG_NUM][MAX_LEN])
{
	if(!strcmp(arg[0],"cd"))
        {
        exec_cd(arg_num,arg,env_table);
        }
        else if(!strcmp(arg[0],"pwd"))
        {
           exec_pwd(arg_num,arg);
        }
        else if(!strcmp(arg[0],"echo"))
        {
           exec_echo(arg_num,arg,env_table);
        }
        else if(!strcmp(arg[0],"clr"))
        {
           exec_clr(arg_num,arg);
        }
        else if(!strcmp(arg[0],"time"))
        {
           exec_time(arg_num,arg);
        }
        else if(!strcmp(arg[0],"quit")||!strcmp(arg[0],"exit"))
        {
            exec_quit(arg_num,arg);
        }   
        else if(!strcmp(arg[0],"environ"))
        {
           env_show(env_table);
        }
        else if(!strcmp(arg[0],"set"))
        {
           exec_set(arg_num,arg,env_table);
        }    
        else if(!strcmp(arg[0],"unset"))
        {
           exec_unset(arg_num,arg,env_table);
        }
        else if(!strcmp(arg[0],"test"))
        {
           exec_test(arg_num,arg);
        }
        else if(!strcmp(arg[0],"shift"))
        {
           exec_shift(arg_num,arg);
        }
        else if(!strcmp(arg[0],"dir"))
        {
           exec_dir(arg_num,arg);
        }
        else if(!strcmp(arg[0],"help"))
        {
           exec_help(arg_num,arg);
        }
        else if(!strcmp(arg[0],"mask"))
        {
           exec_mask(arg_num,arg);
        }
        else if(!strcmp(arg[0],"umask"))
        {
           exec_umask(arg_num,arg);
        }        
        else if(!strcmp(arg[0],"")||!strcmp(arg[0],"")||!strcmp(arg[0],"\n"))
        {
           printf("\n");
        }
        else if(!strcmp(arg[0],"sleept"))
        {
           sleep(atoi(arg[1]));
        }
        else if(!strcmp(arg[0],"jobs"))
        {
           show_job(jobs);
        }
        else if(!strcmp(arg[0],"fg"))
        {
           int i=0;
           for(;i<JOB_NUM;i++)
           {
               if(jobs[i].state==1)
               {
                  //将后台指令终止
                  char temp[ARG_NUM][MAX_LEN];
                  kill(jobs[i].pid,SIGCONT);
                  jobs[i].state=0;
                  //转到前台执行指令
                  int k=parser(jobs[i].cmd,temp);
                  all_exec(k,temp);
                  //删除进程
                  delete_job(jobs[i].jobid);
               }
           }
        }
        else if(!strcmp(arg[0],"bg"))
        {
           int i=0; 
           for(;i<JOB_NUM;i++)
           {
               if(jobs[i].state==1)
               {
                  //让后台中断的指令继续执行
                  kill(jobs[i].pid,SIGCONT);
                  jobs[i].state=0;
                  //等待执行结束
                  waitpid(jobs[i].pid,NULL,0);
                  //输出执行信息
                  printf("[%d]%d  已完成  %s\n",jobs[i].jobid,jobs[i].pid,jobs[i].cmd);
                  //删除进程
                  delete_job(jobs[i].jobid);
               }
           }
        }
        else if(!strcmp(arg[0],"exec"))
        {
              char temp[MAX_LEN]="";
    	      //向左移动一位，执行后面指令
    	      for(int i=0;i<arg_num-1;i++)
    	      {
    	          strcpy(arg[i],arg[i+1]);
    	      }
    	      in_exec(arg_num-1,arg); 
    	      //退出程序
    	      exit(0);          
        }                    
        else
        {
           //执行外部指令
           out_exec(arg_num,arg);
        }          
}

//外部指令实现
void out_exec(int num,char arg[ARG_NUM][MAX_LEN])
{
      pid_t pid;
      char s[MAX_LEN];
      extern job;
      //添加parent环境变量
      getcwd(s,MAX_LEN);
      strcat(s,"/myshell");
      env_delete(env_table,"parent");
      env_push(env_table,"parent",s);
      
      //临时参数表
      char * temp_array[MAX_LEN];
      
      //调用子程序
      pid=fork();
      if(pid<0)
      	printf("无法创建子程序");
      //子程序入口
      else if (pid==0)
      {
      	int i=0;
      	for(i=0;i<num;i++)
      		temp_array[i]=arg[i];
      	 temp_array[i]=NULL;
      	 if(num==1)
      	 	execlp(arg[0],arg[0],(char *)0);
      	 else
      	 	execvp(arg[0],temp_array);
      	 exit(0);
      }
      //父程序入口
      else
      {	 
         char cmd [MAX_LEN]="";
         int s_pid=pid;
         //获得指令以添加进程
	 for (int k=0;k<num;k++)
	 {
	     strcat(cmd,arg[k]);
	     strcat(cmd," ");
	 }
	 job++;
	 //添加进程
	 push_job(job,s_pid,cmd);
	 //等待子程序结束（或者发生中断）
      	 waitpid(pid,NULL,WUNTRACED);
      	 env_delete(env_table,"parent");
      	 //若子程序正常结束，删除进程
      	 if(jobs[job-1].state==0)
	 {
	     delete_job(job);
	     job--;
	 } 
      	 return ;
      }
}

//管道进程执行
void pipe_exec(int num,char arg[ARG_NUM][MAX_LEN])
{
    //若参数个数为0,跳出递归程序
    if(num==0) return 0;
    pid_t pid;
    int i,j,pipe_flag=-1;
    int fd[2];
    //识别最近管道
    for(i=0;i<num;i++)
    {
    	if(!strcmp(arg[i],"|"))
    	{
    	    pipe_flag=i;
    	    break;
    	}
    }
    //若没有管道
    if(pipe_flag==-1)
    {
    	all_exec(num,arg);
    	return;
    }
    //若管道标志位于最后，格式错误
    else if(pipe_flag==num-1)
    {
    	printf("\033[31m管道格式错误！\n");
    	return ;
    }
    
    //创建管道，返回-1则为创建失败
    if(pipe(fd)==-1)
    {
    	 printf("\033[31m管道创建错误!\033[0m\n");
    }
    
    //使用vfork创建子程序
    pid=vfork();
    if(pid==-1)
    {
    	printf("无法创建子程序");
    }
    //子程序入口
    else if(pid==0)
    {
        close(fileno(stdout));
        close(fd[0]);
        //将写端定义在fd[1]，使用fileno转换
        dup2(fd[1],fileno(stdout));
        close(fd[1]);
        
        //截取指令并执行
        char cmd[MAX_LEN]="";
        char temp[ARG_NUM][MAX_LEN];
        for (int k=0;k<pipe_flag;k++)
	{
	    strcat(cmd,arg[k]);
	    strcat(cmd," ");
	}
	int temp_k=parser(cmd,temp);
	in_exec(temp_k,temp);
	exit(0);
    }
    else
    {
        //等待子程序执行完毕
        waitpid(pid,NULL,0);
    	if(pipe_flag<num-1)
    	{
    	     //关闭写端口，打开输入端，定位到stdin
    	     close(fd[1]);
    	     dup2(fd[0],fileno(stdin));
    	     close(fd[0]);
    	}
        //递归执行程序
    	pipe_exec(num-pipe_flag-1,arg+pipe_flag+1);
    	return ;
    }
}

//清除指令数组
void flush_cmd(int num,char arg[ARG_NUM][MAX_LEN])
{
	int i;
	char temp[MAX_LEN]="";
	//赋值为\0
	for(i=0;i<num;i++)
	{
	     strcpy(arg[i],temp);
	}
}

//后台指令执行
void all_exec(int num,char arg[ARG_NUM][MAX_LEN])
{
        if(num==0) return ;
	int i,j;
	int bg_flag=0;
	pid_t pid;
        char temp[ARG_NUM][MAX_LEN];
	extern job;
	int s_pid;
	int temp_k;
	char cmd[MAX_LEN]="";
	

	//遍历寻找是否存在后台运行标志
	for(i=0;i<num;i++)
	{
	    if(!strcmp(arg[i],"&"))
	    {
	    	bg_flag=1;
	    	break;
	    }
	}
	//没有后台运行
	if(bg_flag==0)
	{
	    in_exec(num,arg);
	    return ;
	}
	//有后台运行
	if((pid=vfork())<0)
	     printf("无法创建子程序");
	//子程序入口
	else if(pid==0)
	{    
             	//添加parent环境变量
	     char s[MAX_LEN];
	     getcwd(s,MAX_LEN);
	     strcat(s,"/myshell");
	     env_delete(env_table,"parent");
	     env_push(env_table,"parent",s);	
  	     for (int k = 0;k < i;k++)
	     {
	        strcat(cmd, arg[k]);
		strcat(cmd, " ");
	     }
	     temp_k = parser(cmd, temp);
	     printf("[%d]%d\n", job, getpid());
	     //执行程序
	     in_exec(temp_k,temp);
	     //显示完成信息
	     printf("[%d]%d  已完成  %s\n", job, getpid(), cmd);
	     exit(1);
	} 
	else
	{
	     //等待子程序完成
	     waitpid(pid,NULL,0);
	     //删除parent环境变量
	     env_delete(env_table,"parent");
	     //递归执行后台执行
	     all_exec(num-i-1,arg+i+1);
	     return;
	}
}

