#!/bin/bash
#3180101969
#管理密码模块

type=$1
case $type in
   1) read -p "输入管理员密码" input_pw
      right=$(cat data/passwd |grep "root" |awk '{print $2}')
      if [ $right = $input_pw ]
      then
            return 0
      else
            echo "密码错误！"
            return 1
      fi ;; 
esac
