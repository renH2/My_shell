#main.sh
#黄仁泓 3180101969
#!/bin/bash

echo -e "\033[33m----作业管理系统界面----\033[0m"
echo "选择账户类型："
echo "1.管理员(admin)"
echo "2.教师(teacher)"
echo "3.学生(student)"
echo "4.退出(exit)"
read -p "输入你的选择: " choice
case $choice in
    1)  bash admin.sh ;;
    2)  bash teacher.sh ;;
    3)  bash student.sh ;;
    4)  exit 0;;
    *)  echo "未知输入！"
        exit 1;;
 esac
