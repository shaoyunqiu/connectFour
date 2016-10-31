/********************************************************
*	Strategy.h : 策略接口文件                           *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2010.8                                              *
*********************************************************/

#ifndef STRATEGY_H_
#define	STRATEGY_H_

#include "Point.h"

extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY);

extern "C" __declspec(dllexport) void clearPoint(Point* p);

void clearArray(int M, int N, int** board);

/*
	添加你自己的辅助函数
*/

class Node //蒙特卡洛树节点类
{
	public:
		int col ;//落子的列
		int win ;//胜利场数
		int lose ;//输的场数
		int tie ;//平局场数
		bool isleaf ;//判断是否为叶子
		int child[13] ;//孩子在node中的位置
		int child_num ;//孩子数量
		int user ;//记录该节点的使用者，用户1，本机2，初始为0
		int parent ;//记录父节点位置
		void init_n()
		{
			col = -1 ;
			win = 0 ;
			lose = 0 ;
			tie = 0 ;
			isleaf = 1 ;
			child_num = 0 ;
			user = 0 ;
			parent = -1 ;
		}
} ;

//void init_n(int pos) ;//初始化节点

void init_t(const int *top, int **board) ;//初始化棋盘，恢复棋盘，以及恢复L

int TreePolicy(int pos) ;//选择带扩展的节点，并调用expand进行扩展，返回扩展节点的父节点位置，之后对它新扩展的每一个孩子进行一次模拟

void Default(int pos) ;//对该节点的所有孩子进行模拟，并回溯结果

int BestChild(int pos) ;//返回最好的孩子的位置

void Expand(int pos) ;//一次扩展出所有孩子

int MTKL(int pos) ;//在该节点上进行蒙特卡洛模拟，胜利返回2，平局返回1，失败返回0

double Value(int pos, int p_sum) ;//计算对应的ucb值，其中如果user为2计算胜率，user为1计算败率

void Update(int pos, int val) ;//把结果回溯给祖先

void Update_L() ;//更新课落点列

int put_chess(int y, int u) ;//落子,并返回x坐标

int Check(int x, int y, int u) ;//判断是胜利2， 输0，平局1, 胜负未订-1

void Pre(int &x, int &y, const int *top, int **board) ;//堵

#endif