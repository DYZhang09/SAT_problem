/*****************************************************/
//* Author: Zhang DY                                             
//*Date: 2020/1/21                                                
//*Desription: 配置文件                                          
/****************************************************/

#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<time.h>
#include<conio.h>
#include<string>
using namespace std;

//数独棋盘大小
constexpr int puzzle_size = 8;

//cnf求解模块版本, 0为最初始版本, 1为策略优化版本, 2为内存优化版本
constexpr int cnf_solver_version = 1; 