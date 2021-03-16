#admin.sh
#黄仁泓 3180101969
#!/bin/bash
#管理员登录界面

login(){
    read -p "输入管理员密码: " input_pw
    #管理员登录默认账户为root，密码123456
    right=$(cat data/passwd/admin_pw |grep "root" |awk '{print $2}')
    #检测密码是否正确
    if [ $right = $input_pw ]
    then
           echo "正在登录操作界面"
           main 
    else
           echo -e "\033[31m密码错误！\033[0m"
           bash  main.sh
     fi  
}

main(){
    echo -e "\033[33m ----管理员系统操作界面----- \033[0m"
    echo "1.创建教师帐号"
    echo "2.修改教师帐号"
    echo "3.删除教师帐号"
    echo "4.显示教师帐号"
    echo "5.创建课程"
    echo "6.修改课程"
    echo "7.删除课程"
    echo "8.添加绑定"
    echo "9.删除绑定"
    echo "10.退出系统"
    echo "11.返回界面"
    read -p "输入选项： " choice
    case $choice in
        1) c_teacher;;
        2) u_teacher;;
        3) d_teacher;;
        4) l_teacher;;
        5) c_course ;;
        6) u_course;;
        7) d_course;;
        8) c_bind;;
        9) d_bind;;
        10) ;;
        11) bash main.sh;;
        *)  echo "未知输入！"
            main;;
     esac
}
#创建教师账户
c_teacher(){
   read -p "输入教师工号： " tid
   read -p "输入教师密码： " twd
   #判断工号存在情况
   if [ -z $(cat data/passwd/teacher_pw | grep "$tid"|cut -d " " -f1) ]
   then
        echo -e "\033[33m----创建教师账户-----\033[0m"
        read -p "新增教师姓名: " new_tname
        read -p "新增教师密码: " new_tpw
        #添加信息
        echo $tid $new_tname $new_tpw >> data/passwd/teacher_pw
        echo -e "\033[34m 教师信息添加成功\033[0m"
   else 
        echo "$tid already exist!"
   fi
   main
}
#删除教师账户
d_teacher(){
   read -p "输入教师工号： " tid
   #判断工号存在情况
   if [ -z $(cat data/passwd/teacher_pw | grep "$tid"|cut -d " " -f1) ]
   then
        echo -e "\033[31m $tid 工号不存在\033[0m"
        exit 2
   else 
        echo -e "\033[33m----删除教师账户-----\033[0m"
        echo "删除教师用户 $tid ?"
        read -p "是否删除 Y/N: " choice
        case $choice in
                  #利用Sed删除对应的行 //d
           Y | y) sed -i /$tid/d data/passwd/teacher_pw
                sed -i /$tid/d data/t_c
                echo -e "\033[34m 删除成功!\033[0m";; 
           N | n) echo "撤销删除操作"
                bash main.sh;;
           *)   echo "未知字符"
                exit 1;;
        esac
   fi   
   main
}
#列表教师账户，隐藏密码
l_teacher(){
   echo -e "\033[33m----显示教师账户-----\033[0m"
   echo "工号 教师姓名"
   cat data/passwd/teacher_pw | cut -d " " -f -2
   main
}

#修改教师账户
u_teacher(){
   read -p "输入教师工号： " tid
   #判断工号存在情况
   if [ -z $(cat data/passwd/teacher_pw | grep "$tid"|cut -d " " -f1) ]
   then
        echo -e "\033[31m $tid 工号不存在\033[0m"
        exit 2
   else 
        echo -e "\033[33m----修改教师账户-----\033[0m"
        echo "输入修改教师姓名和密码： "
        read -p "教师姓名： " new_tname
        read -p "教师密码： " new_tpw
        #删除账户并添加账户（这里不需要删除t_c，因为工号未改变）
        sed -i /$tid/d data/passwd/teacher_pw
        echo $tid $new_tname $new_tpw >> data/passwd/teacher_pw
        echo -e "\033[34m 教师信息添加成功\033[0m"
   fi
   main
}
#创建课程
c_course(){
   echo -e "\033[33m----创建课程-----\033[0m"
   read -p "输入课程名称： " cname
   #判断课程存在情况
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        #添加信息并创建相应文件夹
        echo $cname >> data/course
        mkdir -p course/$cname
        echo -e "\033[34m 课程信息添加成功\033[0m"
   else 
        echo "course $cname already exist!"
   fi
   main
}
#删除课程
d_course(){
   echo -e "\033[33m----删除课程-----\033[0m"
   read -p "输入课程名称： " cname
   #判断课程存在情况
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo -e "\033[31m $cname 工号不存在\033[0m"
   else 
        #删除对应课程信息并删除相应文件夹
        sed -i /$cname/d data/course
        sed -i /$cname/d data/t_c
        rm -rf course/$cname
        echo -e "\033[34m删除成功!\033[0m" 
   fi
   main
}
#修改课程
u_course(){
   echo -e "\033[33m----修改课程-----\033[0m"
   read -p "输入课程名称： " cname
   #判断课程是否存在
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo -e "\033[31m $cname 工号不存在\033[0m"
   else 
        read -p "输入新课程名称： " new_cname  
        #利用sed进行名字替换      
        sed -i /$cname/d data/course
        sed -i 's/$cname/$new_cname/g' data/t_c
        echo $new_cname >> data/course
        #修改文件名
        mv course/$cname course/$new_cname
        echo -e "\033[34m修改成功!\033[0m"  
   fi 
   main 
}
#创建联系
c_bind(){
   echo -e "\033[33m----创建联系-----\033[0m"
   read -p "输入教师工号：" tid
   read -p "输入课程名称：" cname
   #判断课程存在情况
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo -e "\033[31m $cname 课程名不存在\033[0m"
        exit 2
   else 
        #判断教师存在情况
        if [ -z $(cat data/passwd/teacher_pw | grep "$tid"|cut -d " " -f1) ]
        then 
             echo -e "\033[31m $tid 教师工号不存在\033[0m"
             exit 2
        #判断绑定信息存在情况
        elif [ -z $(cat data/t_c | grep "$tid"|grep "$cname"|cut -d " " -f1) ]
        then
             echo $tid $cname>>data/t_c
             echo -e "\033[34m联系添加成功!\033[0m"
        fi
   fi
   main
}
#删除联系
d_bind(){
   echo -e "\033[33m----删除联系-----\033[0m"
   read -p "输入教师工号：" tid
   read -p "输入课程名称：" cname
   #判断课程存在情况
   if [ -z $(cat data/course | grep "$cname"|cut -d " " -f1) ]
   then
        echo -e "\033[31m $cname 课程名不存在\033[0m"
        exit 2
   else 
        #判断课程存在情况
        if [ -z $(cat data/passwd/teacher_pw | grep "$tid"|cut -d " " -f1) ]
        then 
             echo -e "\033[31m $tid 教师工号不存在\033[0m"
             exit 2
        elif [ -z $(cat data/t_c | grep "$tid"|grep "$cname"|cut -d " " -f1) ]
        then
             echo "already delete!"
        else 
             echo "$tid $cname"
             sed -i /"$tid $cname"/d data/t_c
             echo "删除联系成功"
        fi
   fi
   main
}
login
