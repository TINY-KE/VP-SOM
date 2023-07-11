/*
 * @Descripttion: 
 * @version: 
 * @Author: Zhang Jiadong
 * @Date: 2021-12-19 20:22:40
 * @LastEditors: GetOverMassif 164567487@qq.com
 * @LastEditTime: 2022-08-28 16:42:35
 */

#ifndef COMMON_INCLUDE_H
#define COMMON_INCLUDE_H

#include <sstream>
// #include "arm_control/Arms.h"
#include <thread>
#include <atomic>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <functional>
#include <pthread.h>
#include <cstdio>
#include <string>

/**
 * @brief 如果想要终端输出的颜色不同，就需要用到printf的控制命令：\033[m
 * 控制命令以 \033[ 开头，以 m 结尾，中间是属性码，属性代码之间用分号";"隔开
 *  -   属性：重置所有属性-0，高亮/加粗-1，暗淡-2，下划线-4，闪烁-5，反转-7，隐藏-8
 *  -   颜色：黑色-30，红色-31，绿色-32，黄色-33，蓝色-34，品红-35，青色-36
 *  - 背景色：黑色-40，红色-41，绿色-42，黄色-43，蓝色-44，品红-45，青色-46
 * 
 * 参考博客：https://www.cnblogs.com/lewki/p/14343894.html
 */

namespace FABO_ROBOT
{

enum printColor{
    NONE = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    PINKISH_RED = 5,
    CYAN = 6
    // 红色-31，绿色-32，黄色-33，蓝色-34，品红-35，青色-36
};

void printInColor(const char *content, const printColor color , std::string lv="", std::string rv=""){
    printf("\033[0m");

    if (color == NONE){}
    else if (color == RED)
        printf("\033[1;31m");
    else if (color == GREEN)
        printf("\033[1;32m");
    else if (color == YELLOW)
        printf("\033[1;33m");
    else if (color == BLUE)
        printf("\033[1;34m");
    else if (color == PINKISH_RED)
        printf("\033[1;35m");
    else if (color == CYAN)
        printf("\033[1;36m");
    
    printf("%s", content);
    if( lv!=""  &&  rv!="")
        printf("左轮速度%s, 右轮速度%s \n", lv, rv);
    printf("\033[0m ");
}

};

#endif