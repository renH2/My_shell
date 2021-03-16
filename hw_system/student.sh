#student.sh
#黄仁泓 3180101969
#!/bin/bash
#执行学生操作
login(){
    echo -e "\033[33m----学生登录界面----\033[0m"
    read -p "输入学生学号: " input_sid
    read -p "输入学生密码: " input_pw
    #学生系统密码文件：学号+姓名+密码
    right=$(cat data/passwd/student_pw |grep "$input_sid" |awk '{print $3}')
    if [ $right = $input_pw ]
    then
           echo "正在登录操作界面"
           main $input_sid
    else
           echo -e "\033[31m密码错误！\033[0m"
           bash  main.sh
     fi  
}

main(){
    echo -e "\033[33m ----學生系统操作界面----- \033[0m"
    echo "1.新建作业或实验"
    echo "2.上传作业"
    echo "3.查询作业内容"
    echo "4.查询作业或实验完成情况"
    echo "5.退出程序"
    echo "6.返回主界面"
    read -p "输入选项： " choice
    case $choice in
        1) c_hw $1;;
        2) upload_hw $1;;
        3) s_table $1;;
        4) s_hw $1;;
        5) ;;
        6) bash main.sh;;
        *)  echo "未知输入！"
            main;;
     esac
}
#新建作业和实验（文本类型）
c_hw(){
   echo -e "\033[33m----新建作业和实验-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       read -p "输入上传作业名称: " hwname
       #判断作业是否存在
       if [ -e course/$cname/hw_$hwname ]
       then 
            echo "当前作业为$hwname"
            read -p "输入上传答案名称: " ansname
            gedit course/$cname/upload_$hwname/$1_$ansname
       else
             echo "不存在该作业"
       fi
   fi 
   main $1
}
#上传作业（文件）
upload_hw(){
   echo -e "\033[33m----上传作业和实验-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       read -p "输入上传作业名称: " hwname
       #判断课程是否存在
       if [ -e course/$cname/hw_$hwname ]
       then 
            echo "当前作业为$hwname"
            read -p "输入上传答案路径: " hw_path
            t=$(basename $hw_path) 
            cp $hw_path course/$cname/upload_$hwname/$1_$t
       else
             echo "不存在该作业"
       fi
   fi
   main $1 
}
#查询作业内容
s_table(){
   echo -e "\033[33m----查询作业内容-----\033[0m"
   read -p "输入课程名称：" cname
   #判断课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       echo "$cname所有作业如下："
       #遍历作业
       for i in $(ls course/$cname|grep "^hw")
       do 
             echo $i
       done
       read -p "是否需要查看作业详情 Y/N: " choice
       case $choice in
          Y | y) read -p "输入作业名称：" hw_Name
                cat course/$cname/hw_$hw_Name;;
          N | n) main $1;;
          *)   echo "未知字符"
                exit 1;;
       esac
   fi 
   main $1
}
#查询作业和实验完成情况
s_hw(){
   echo -e "\033[33m----查询作业和实验完成情况-----\033[0m"
   read -p "输入课程名称：" cname
   #课程是否存在
   if [ ! -e course/$cname ];then
       echo "课程目录不存在！"
       exit 2
   else
       echo "$cname所有作业如下："
       #遍历作业
       for i in $(ls course/$cname|grep "^hw")
       do 
             echo $i
             echo -e "\033[34m你的提交：\033[0m"
             #提取作业名称
             da=$(basename $i|cut -d "_" -f2)
             echo $da
             for j in $(ls course/$cname/upload_$da | grep "$1")
             do  
                  echo $j
             done  
       done
       read -p "是否需要查看上传作业详情 Y/N: " choice
       case $choice in
          #打印作业
          Y | y) read -p "输入作业名称：" hw_Name
                cat course/$cname/upload_$da/$1_$hw_Name;;
          N | n) main $1;;
          *)   echo "未知字符"
                exit 1;;
       esac
   fi 
   main $1
}
login 
