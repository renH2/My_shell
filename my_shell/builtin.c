/**********************************
 *姓名：黄仁泓 
 *学号：3180101969
 *程序名：builtin.c
 **********************************/
#include "builtin.h"

//执行cd <directory> 指令
int exec_cd(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[])
{
     char temp[MAX_LEN];
     //cd 没有<directory> 参数
     if(num==1)
     {
        getcwd(temp,MAX_LEN);
     	printf("当前目录为：%s\n",temp);
     }
     // 命令参数过多
     else if(num>2)
     {
          printf("\033[31mcd指令：参数过多！\033[0m\n");
          return 1;
     }
     //将默认目录改变为<directory>
     else
     {
     	  if(!chdir(arg[1]))
     	  {
     	  	printf("跳转到目录： %s\n",arg[1]);
     	  	getcwd(temp,MAX_LEN);
     	  	//更新pwd环境变量
     	  	env_delete(env_table,"pwd");
     	  	env_push(env_table,"pwd",temp);
     	  }
     	  //若输入不是目录
     	  else{
     	  	printf("\033[31m目录不存在！\033[0m\n");
     	  }
     }
     return 0;
}
//获得当前目录绝对地址
int exec_pwd(int num,char arg[ARG_NUM][MAX_LEN])
{
     char temp[MAX_LEN];
     //利用系统调用，获得pwd
     getcwd(temp,MAX_LEN);
     //显示pwd值
     printf("%s\n",temp);
     return 0;
}
//清除屏幕
int exec_clr(int num,char arg[ARG_NUM][MAX_LEN])
{
     printf("\033[2J");
}
//获得系统时间
int exec_time(int num,char arg[ARG_NUM][MAX_LEN])
{
     time_t t;
     struct tm *temp;
     //获取时间结构体
     time(&t);
     temp=localtime(&t);
     printf("%d/%d/%d %d:%d:%d\n",temp->tm_year+1900,temp->tm_mon,temp->tm_mday,temp->tm_hour,temp->tm_min,temp->tm_sec);
     return 0;
}
//退出程序
int exec_quit(int num,char arg[ARG_NUM][MAX_LEN])
{
     exit(0);
}
//查看系统环境变量
int exec_environ(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[])
{
    //遍历环境列表
    env_show(env_table);
    return 0;
}
//帮助界面
int exec_help(int num,char arg[ARG_NUM][MAX_LEN])
{
    char text[MAX_LEN];
    FILE *fp=fopen("readme","r");
    //打开文件失败
    if(fp==NULL)
    {
    	fprintf(stderr,"help指令打开文件失败！\n");
    	return 1;
    }
    else
    {
    	//k记录命令行数
        int k=0;
    	while(fgets(text,MAX_LEN,fp)!=0)
    	{
    	     if(k==MORE_NUM)
    	     {
    	     	char s;
    	     	printf("\033[34mmore?(q:quit)\033[0m\n");
    	     	s=getchar();
    	     	while(1)
    	     	{
    	     	    if(s=='q')
    	     	    {
    	     	        fclose(fp);
    	     	    	return 0;
    	     	    }
    	     	    if(s==' ')
    	     	    {
    	     	    	k=0;
    	     	    	break;
    	     	    }
    	     	    if(s=='\n')
    	     	    {
    	     	    	k=MORE_NUM-1;
    	     	    	break;
    	     	    }
    	     	}   	     	
    	     }
    	     k++;
    	     printf("%s",text);
    	}
    	return 0;
    }
}
//显示命令
int exec_echo(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[])
{
    int i;
    if(num==0) 
    {
    	printf(" \n");
    	return 1;
    }
    //按顺序显示各个变量结果，若为环境变量则进行转换
    for(i=1;i<num;i++)
    {
    	//环境变量
    	if(arg[i][0]=='$')
    	{
    	    char *temp=env_get(env_table,arg[i]+1);
    	    printf("%s",temp);
    	}
    	else
    	{
    	    for(int k=0;k<MAX_LEN;k++)
    	    {
    	         if(arg[i][k]!='\0') printf("%c",arg[i][k]);
    	         else break;
    	    }
    	}
    	if(i!=num-1) printf(" ");
    	else printf("\n");
    }
    return 0;  
}
//显示当前目录下的所有文件
int exec_dir(int num,char arg[ARG_NUM][MAX_LEN])
{ 
    //输入参数不符合
    if(num==1||num>3)
    {
    	printf("\033[31m参数太少！\033[0m\n");
    	return 1;    	
    }
    //目录结构体，获取目录信息
    DIR *dir;
    struct dirent* dst;
    //打开目录
    dir=opendir(arg[1]);
    //若打开目录失败
    if(dir==NULL)
    {
    	printf("\033[31m打不开%s目录!\033[0m\n",arg[1]);
    }
    else
    {
        int i=0;
        //循环读取文件名称
    	while((dst=readdir(dir))!=NULL)
    	{
    	     i++;
    	     printf("%s\n",dst->d_name);
    	    // if(i%LS_NUM==0) printf("\n");
    	    // else printf("\t");
   	}
   //	if(i%LS_NUM!=0) printf("\n");
    }
    //关闭目录
    closedir(dir);	
}

//设置环境变量
int exec_set(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[])
{
    if(num!=3)
    {
    	printf("\033[31m参数个数不正确!\033[0m\n");
    }
    //若环境变量存在则修改变量，若环境变量不存在则创建变量
    env_delete(env_table,arg[1]);
    env_push(env_table,arg[1],arg[2]);
}
//删除环境变量
int exec_unset(int num,char arg[ARG_NUM][MAX_LEN],struct env env_table[])
{
    //参数错误
    if(num!=2)
    {
    	printf("\033[31m参数个数不正确!\033[0m\n");
    }
    //删除环境变量
    env_delete(env_table,arg[1]);
}

int exec_mask(int num,char arg[ARG_NUM][MAX_LEN])
{
    extern char shell_umask[20];
    printf("mask:%s\n",shell_umask);
}

int exec_umask(int num,char arg[ARG_NUM][MAX_LEN])
{
    extern char shell_umask[20];
    strcpy(shell_umask,arg[1]);
    printf("new mask:%s\n",shell_umask);
}

//测试语句真假，目前只支持字符串（-z -n）和整数（-eq）
int exec_test(int num,char arg[ARG_NUM][MAX_LEN])
{
    //字符串不存在为真
    if(!strcmp(arg[1],"-z"))
    {
    	if(arg[2][0]=='\0') printf("true\n");
    	else printf("false\n");
    }
    //字符串不存在为假
    else if(!strcmp(arg[1],"-n"))
    {
    	if(arg[2][0]=='\0') printf("false\n");
    	else printf("true\n");
    }
    //判断是否满足eq条件
    else if(isnumber(arg[1])&&isnumber(arg[3])&&!strcmp(arg[2],"-eq"))
    {
    	int a=atoi(arg[1]),b=atoi(arg[3]);
    	if(a==b) printf("true\n");
    	else printf("false\n");
    }
    return 0;
}
//判断参数是否为数字
int isnumber(char *s)
{
    int i,flag=1;
    for(i=0;s[i]!='\0';i++)
    {
    	if(s[i]<'0'||s[i]>'9') 
    		flag=0;
    }
    return flag;
}
//左移参数表一位
int exec_shift(int num,char arg[ARG_NUM][MAX_LEN])
{
    char temp[MAX_LEN]="";
    if(num==2)
    {
    	strcpy(arg[1],temp);
    }
    else
    {
    	//向左移动一位
    	for(int i=1;i<num-1;i++)
    	{
    	    strcpy(arg[i],arg[i+1]);
    	}
    }
    //为方便验证，加入移位后显示
    printf("shift移位后：\n");
    for(int i=1;i<num-1;i++)
    {
    	printf("%s ",arg[i]);
    }
    printf("\n");
}
