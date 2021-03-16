#teacher.sh
#黄仁泓 3180101969
#!/bin/bash
#教师登录界面

login(){
    read -p "输入教师工号: " input_tid
    read -p "输入教师密码: " input_pw
    #得到正确密码
    right=$(cat data/passwd/teacher_pw |grep "$input_tid" |awk '{print $3}')
    #得到使用者姓名
    usrn=$(cat data/passwd/teacher_pw |grep "$input_tid" |awk '{print $2}')
    echo $right
    if [ $right = $input_pw ]
    then
           echo "正在登录操作界面"
           echo "欢迎回来，$usrn！"
           main 
    else
           echo -e "\033[31m密码错误！\033[0m"
           bash  main.sh
    fi  
}
#主程序
main(){
    echo -e "\033[33m ----教师系统操作界面----- \033[0m"
    echo "1.学生账户管理"
    echo "2.课程信息管理"
    echo "3.作业和实验管理"
    echo "4.退出系统"
    echo "5.返回界面"
    read -p "输入选项： " choice
    case $choice in
        1) student_log;;
        2) course_info;;
        3) course_hw;;
        4) exit 0;;
        5) bash main.sh;;
        *)  echo "未知输入！"
            main;;
     esac
}
#学生账户处理
student_log(){
    echo -e "\033[33m ----学生账户操作界面----- \033[0m"
    echo "1.创建学生账户"
    echo "2.导入学生账户"
    echo "3.修改学生账号"
    echo "4.删除学生账号"
    echo "5.查找学生账号"
    echo "6.返回上一级"
    read -p "输入选项： " choice
    case $choice in
        1) c_student;;
        2) p_student;;
        3) u_student;;
        4) d_student;;
        5) s_student;;
        6) main;;
        *)  echo "未知输入！"
            main;;
     esac
}
#创建学生账户，如果存在则直接创建课程学生链接
c_student(){
   echo -e "\033[33m----创建学生账户-----\033[0m"
   read -p "输入课程名称： " cname
   read -p "输入学号：" sid
   #判断课程信息是否存在
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo "课程信息不存在"
        exit 2
   else 
      #判断学生信息是否存在
      if [ -z $(cat data/passwd/student_pw | grep "$sid"|cut -d " " -f1) ]
      then
        read -p "学生信息不存在，是否创建？Y/N: " choice
        #输入选项
        case $choice in
           Y | y) read -p "输入姓名: " sname 
                  read -p "输入密码: " spw
                  echo $sid $sname $spw >>data/passwd/student_pw
                  echo $cname $sid >>data/c_s
                  echo "学生账户创建成功！";;
           N | n) echo "撤销创建操作"
                bash main.sh;;
           *)   echo "未知字符"
                exit 1;;
        esac
       #判断绑定信息中是否已经存在
      elif [ -z $(cat data/c_s |grep "$sid"|grep "$cname"|cut -d " " -f1) ]; then
        echo $cname $sid >>data/c_s
      fi
   fi
}
#直接导入学生账户
p_student(){
   echo -e "\033[33m----导入学生账户-----\033[0m"
   read -p "输入课程名称：" cname
   read -p "输入导入路径: " s_path 
   #判断课程信息是否存在
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo "课程信息不存在"
        exit 2
   fi
   #提取导入文件的第一列信息（姓名）
   declare -i j=0
   for i in $(cat $s_path)
   do 
      j=$((j+1))
      #文件共3列，故j%3==1读出第一列
      if ((j%3==1));then
        echo "导入 "$i" 信息"
      #判断学生和课程是否存在
      if [ -z $(cat data/c_s |grep "$i"|grep "$cname"|cut -d " " -f1)];then
        echo $cname $i >>data/c_s
      fi
      fi 
   done
   cat $s_path >>data/passwd/student_pw
   echo "导入后名单： "
   #输出排序后结果
   sort -n data/passwd/student_pw |uniq
   echo "导入成功！"
}
#删除学生账户
d_student(){
   echo -e "\033[33m----删除学生账户-----\033[0m"
   read -p "输入课程名称：" cname
   read -p "输入学号：" sid
   #判断课程是否存在
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo "课程信息不存在"
        exit 2
   fi
   if [ -z $(cat data/c_s |grep "$sid"|grep "$cname"|cut -d " " -f1) ];   then
        echo "不存在该账户"
        exit 2
   else
        #删除课程学生链接表信息
        sed -i /$sid/d data/c_s
        echo "成功删除 $sid 在$cname记录"
   fi
}
#修改学生账户
u_student(){
   echo -e "\033[33m----删除学生账户-----\033[0m"
   read -p "输入课程名称：" cname
   read -p "输入学号：" sid
   #判断课程是否存在
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo "课程信息不存在"
        exit 2
   fi
   if [ -z $(cat data/c_s |grep "$sid"|grep "$cname"|cut -d " " -f1) ];   then
        echo "不存在该账户"
        exit 2
   else
        read -p "输入新学生姓名：" new_sname
        read -p "输入新学生密码: " new_spw
        #删除课程学生链接表信息
        sed -i /$sid/d data/passwd/student_pw 
        echo $sid $new_sname $new_spw >> data/passwd/student_pw
        echo "修改学生信息成功"
   fi
}
#搜索学生
s_student(){
   echo -e "\033[33m----删除学生账户-----\033[0m"
   read -p "输入课程名称：" cname
   read -p "输入学号：" sid
   #判断课程是否存在
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo "课程信息不存在"
        exit 2
   fi
   #判断链接信息是否存在
   if [ -z $(cat data/c_s |grep "$sid"|grep "$cname"|cut -d " " -f1) ];  then
        echo "不存在该账户"
        exit 2
   fi
   echo "查询结果如下："
   cat data/passwd/student_pw |grep "$sid"|cut -d " " -f -2
   student_log
}

course_info(){
    echo -e "\033[33m ----课程信息界面----- \033[0m"
    echo "1.新建课程信息"
    echo "2.编辑课程信息"
    echo "3.删除课程信息"
    echo "4.显示课程信息"
    echo "5.返回上一级"
    read -p "输入选项： " choice
    case $choice in
        1) c_course_info;;
        2) u_course_info;;
        3) d_course_info;;
        4) l_course_info;;
        5) main;;
        *)  echo "未知输入！"
            main;;
     esac
}
#创建课程信息
c_course_info(){
   echo -e "\033[33m----创建课程信息-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       if [ ! -e course/$cname/info ]
       then 
             touch course/$cname/info
             echo "创建课程信息成功"
       else
             echo "已经存在该课程信息"
       fi
   fi
   #course_info
}
#通过gedit编辑课程信息
u_course_info(){
   echo -e "\033[33m----编辑课程信息-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       echo "编辑课程信息"
       gedit course/$cname/info 
   fi
}
#删除课程信息
d_course_info(){
   echo -e "\033[33m----删除课程信息-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       if [ -e course/$cname/info ]
       then 
             rm -rf course/$cname/info
             echo "删除课程信息成功！"
       fi
   fi
}
#显示课程信息
l_course_info(){
   echo -e "\033[33m----显示课程信息-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       if [ -e course/$cname/info ]
       then 
             echo "课程信息如下："
             cat course/$cname/info
       fi
   fi
}
#作业&实验管理界面
course_hw(){
    echo -e "\033[33m ----作业&实验管理界面----- \033[0m"
    echo "1.新建作业或实验"
    echo "2.编辑作业或实验"
    echo "3.删除作业或实验"
    echo "4.显示作业或实验"
    echo "5.查找打印学生完成作业情况"
    echo "6.返回上一级"
    read -p "输入选项： " choice
    case $choice in
        1) c_hw;;
        2) u_hw;;
        3) d_hw;;
        4) l_hw;;
        5) s_all;;
        6) main;;
        *)  echo "未知输入！"
            main;;
     esac
}
#布置作业和实验
c_hw(){
   echo -e "\033[33m----布置作业和实验-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       read -p "输入作业名称: " hwname
       if [ ! -e course/$cname/hw_$hwname ]
       then 
             touch course/$cname/hw_$hwname
             mkdir course/$cname/upload_$hwname
             echo "创建作业和实验$hwname成功"
       else
             echo "已经存在该作业"
       fi
   fi
}
#编辑作业和实验
u_hw(){
   echo -e "\033[33m----编辑作业和实验-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       read -p "输入作业名称: " hwname
       #判断作业是否存在
       if [ -e course/$cname/hw_$hwname ]
       then 
             gedit course/$cname/hw_$hwname
       fi
   fi
}
#删除作业和实验
d_hw(){
   echo -e "\033[33m----删除作业和实验-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       read -p "输入作业名称: " hwname
       #判断文件是否存在
       if [ -e course/$cname/hw_$hwname ]
       then 
             rm -rf course/$cname/hw_$hwname
             rm -rf course/$cname/upload_$hwname
             echo "删除作业和实验$hwname成功"
       else
             echo "不存在该作业"
       fi
   fi
}
#显示作业和实验
l_hw(){
   echo -e "\033[33m----显示作业和实验-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       read -p "输入作业名称: " hwname
       #判断作业是否存在
       if [ -e course/$cname/hw_$hwname ]
       then 
             echo "显示作业和实验如下："             
             cat course/$cname/hw_$hwname
       fi
   fi
}
#查找打印学生完成作业情况
s_all(){
   echo -e "\033[33m----查询作业和实验完成情况-----\033[0m"
   read -p "输入课程名称：" cname
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       echo "$cname所有作业如下："
       #利用grep遍历所有hw开头的文件
       for i in $(ls course/$cname|grep "^hw")
       do 
             echo $i
             echo -e "\033[34m学生的提交：\033[0m"
             #提取单独的作业名字
             da=$(basename $i|cut -d "_" -f2)
             echo $da
             for j in $(ls course/$cname/upload_$da )
             do  
                  echo $j
             done  
       done
       #查看作业详情
       read -p "是否需要查看上传作业详情 Y/N: " choice
       case $choice in
          Y | y)
                read -p "输入作业名称: " times
                read -p "输入学生学号: " sid 
                #遍历学生上传的文件并输出
                for i in $(ls course/$cname/upload_$times |grep "$sid")
                do 
                    cat course/$cname/upload_$times/$i
                done;;
          N | n) main;;
          *)   echo "未知字符"
                exit 1;;
       esac
   fi 
}
login
