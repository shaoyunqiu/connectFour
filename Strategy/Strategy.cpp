#include <iostream>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <atlstr.h>

using namespace std;
int m, n ;//行，列
const int Max_Num = 8000000 ;//最大节点数
const double C = 0.8 ;//计算公式中的常数值
Node node[Max_Num] ;//存储所有节点
int cnt = 0 ;//记录当前节点个数
int *t_top ;//临时存储列顶情况
int **t_board ; //临时存储棋盘情况
int no_x ;
int no_y ;//记录不可落子点
int L[13] ; //存储0~n-1的序列，为了之后方便随机选子
int l_num = 0;//记录此时L的大小，及可落子的列数
int isEnd = -1 ;//判断胜负是否已定，如果已定，则无需进行模拟 胜利2， 平局1， 败局0
clock_t start_time ;//开始时间
bool ifPre = 0 ;

/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误
	
	input:
		为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
		M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
		top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
		_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
				你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
				board[x][y]表示第x行、第y列的点(从0开始计)
				board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
		lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
				落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
		noX, noY : 棋盘上的不可落子点(注:其实这里给出的top已经替你处理了不可落子点，也就是说如果某一步
				所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
				所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
				当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
		以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
		你的落子点Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY){
	/*
		不要更改这段代码
	*/
	int x = -1, y = -1;//最终将你的落子点存到x,y中
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	start_time = clock();//记录开始时间
	srand(unsigned(time(NULL))) ;//随机数种子
	/*
		根据你自己的策略来返回落子点,也就是根据你的策略完成对x,y的赋值
		该部分对参数使用没有限制，为了方便实现，你可以定义自己新的类、.h文件、.cpp文件
	*/
	m = M ;
	n = N ;
	no_x = noX ;
	no_y = noY ;
	cnt = 0 ;
	ifPre = 0 ;
	t_top = new int[N] ;
	t_board = new int*[M] ;
	for(int i = 0 ; i < M ; i ++)
		t_board[i] = new int[N] ;	//初始化，分配空间
	init_t(top, board) ;//初始化棋盘
	node[0].init_n() ;
	node[0].col = lastY ;//初始化根节点
	node[0].user = 1 ;
	Pre(x, y, top, board) ;

	if(ifPre == 1)
	{
		put_chess(y, 2) ;
	}
	//先堵， 如果没有可堵节点再进行搜索
	else{
	while(double(clock()-start_time) < 4500 && cnt < Max_Num) //不超时且节点数足够
	{
		int now = TreePolicy(0) ;//返回扩展的父节点位置
		if(isEnd != -1) //如果沿着树走的过程中，胜负已定，则无需再进行模拟，直接更新胜负局数即可
		{
			Update(now, isEnd) ;
		}
		else Default(now) ;//如果在选节点时胜负未定，则对新扩展的孩子节点依次进行模拟
		init_t(top, board) ;//恢复棋盘
	}
	int ans = 1;
	int mu = (node[0].win + node[0].tie + node[0].lose)/node[0].child_num ;//计算父节点的平均值
	double pp = -1 ;
	for(int i = 0 ; i < node[0].child_num ; i ++)//遍历根节点的孩子
	{
		int cc = node[0].child[i] ;
		int sum = node[cc].win + node[cc].tie + node[cc].lose ;
		if(sum >= mu)//认为比较次数大于平均值的时候被接受
		{
			double tmp = double(node[cc].win + node[cc].tie)/sum ;//计算胜率
			if(tmp > pp)
			{
				pp = tmp ;
				ans = cc ;
			}
		}
	} //最后一步下棋时选择胜率最大的点
	y = node[ans].col ;
	x = put_chess(y,node[ans].user);}
	/*
		不要更改这段代码
	*/
	clearArray(M, N, board);
	return new Point(x, y);
}


/*
	getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
	函数来释放空间，而不应该在外部直接delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	清除top和board数组
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	添加你自己的辅助函数，你可以声明自己的类、函数，添加新的.h .cpp文件来辅助实现你的想法
*/

void init_t(const int *top, int **board) 
{
	l_num = 0 ;
	isEnd = -1 ;//初始结束标志
	for(int i = 0 ; i < n ; i ++)
	{
		t_top[i] = top[i] ;
		if(top[i] > 0)//可以落子
		{
			L[l_num] = i ;
			++ l_num ;
		}
	}
	for(int i = 0 ; i < m ; i ++)
	{
		for(int j = 0 ; j < n ; j ++)
		{
			t_board[i][j] = board[i][j] ;
		}
	}	
}

int put_chess(int y, int u) 
{
	-- t_top[y] ;
	int x = t_top[y] ;
	t_board[x][y] = u ;
	if(y == no_y && t_top[y] - 1 == no_x)
	{
		-- t_top[y] ;
	} //处理不可落子点，只能落在不可落子点上方
	Update_L() ;// 更新可落点列
	return x ;
}

void Update_L()
{
	for(int i = 0 ; i < l_num ; i ++)
	{
		if(t_top[L[i]] == 0) //该列已满，不可落子，删除
		{
			swap(L[i], L[l_num-1]) ;
			-- l_num ;//将其和该数列的最后一个交换，并且总数减1
		} 
		else continue ;
	}
}

void Update(int pos, int val)//更新祖先结果值
{
	while(pos != -1)
	{
		if(val == 0) //败局
		{
			++ node[pos].lose ;
			pos = node[pos].parent ;
			continue ;
		}
		else if(val == 1)//平局
		{
			++ node[pos].tie ;
			pos = node[pos].parent ;
			continue ;
		}
		else //胜局
		{
			++ node[pos].win ;
			pos = node[pos].parent ;
			continue ;
		}
	}
}

int TreePolicy(int pos)
{
	while(!(node[pos].isleaf == 0 && node[pos].child_num == 0)) //不是终止节点
	{
		if(node[pos].isleaf == 1)//叶子节点，可扩展
		{
			Expand(pos) ;
			node[pos].isleaf = 0 ;//更改标志
			return pos ;
		}
		else
		{
			pos = BestChild(pos) ;
			put_chess(node[pos].col, node[pos].user) ;//并将选择的孩子放在棋局上
			int nowy = node[pos].col ;
			int nowx = t_top[nowy] ;
			int nowr = Check(nowx, nowy, node[pos].user) ;
			if(nowr != -1) 
			{
				isEnd = nowr ;
				return pos ; // 如果胜负已定，更改结束标志，返回该节点
			}
		}
	}
	return pos ;
}

void Expand(int pos)
{
	int c_n = 0 ;//记录孩子个数
	for(int i = 0 ; i < l_num ; i ++) //依次扩展可落子列
	{
		++ cnt ;
		node[cnt].init_n() ;//先对该节点初始化
		node[cnt].col = L[i] ;
		node[cnt].parent =  pos ;
		node[cnt].user = 3 - node[pos].user ;
		node[pos].child[c_n] = cnt ;
		++ c_n ;
	}
	node[pos].child_num = c_n ;
}

int BestChild(int pos)
{
	double temp = -100000 ;
	double val = 0 ;
	int ans = 0 ;
	int sum = node[pos].win + node[pos].lose + node[pos].tie ;//父亲节点的总局数
	for(int i = 0 ; i < node[pos].child_num ; i ++)
	{
		val = Value(node[pos].child[i], sum) ;
		if(val > temp )
		{
			temp = val ;
			ans = node[pos].child[i] ;
		}
	}
	return ans ;
}

double Value(int pos, int p_s)
{
	if(node[pos].user == 2 ) // 计算胜率
	{
		int m_s = node[pos].win + node[pos].tie + node[pos].lose ;
		double X = double(node[pos].win + node[pos].tie) / m_s ;//将胜利得分与平局得分区分开
		X += C * sqrt(double(double(2*log(double(p_s)))/m_s)) ;
		return X ;
	}
	else // 用户，计算败率
	{
		int m_s = node[pos].win + node[pos].tie + node[pos].lose ;
		double X = double(node[pos].lose + node[pos].tie) / m_s ;
		X += C * sqrt(double(double(2*log(double(p_s)))/m_s)) ;
		return X ;
	}
}

void Default(int pos)
{
	int tt[12] ;
	int tl[12] ;
	int tb[12][12] ;
	int tl_c = l_num ;//临时存储当前局面
	for(int j = 0 ; j < n ; j ++)
	{
		tt[j] = t_top[j] ;
	}
	for(int j = 0 ; j < l_num ; j ++)
	{
		tl[j] = L[j] ;
	}
	for(int j = 0 ; j < m ; j ++)
	{
		for(int k = 0 ; k < n ; k ++)
		{
			tb[j][k] = t_board[j][k] ;
		}
	} // 保存蒙特卡洛之前的棋盘
	for(int i = 0 ; i < node[pos].child_num ; i ++)//对每个孩子进行第一次模拟
	{
		int nchild = node[pos].child[i] ;//当前节点的位置
		put_chess(node[nchild].col, node[nchild].user) ;//把当前的孩子放在棋盘中
		int res = MTKL(nchild) ;//对该孩子进行模拟
		Update(nchild, res) ;
		//恢复棋盘
		l_num = tl_c ;
		for(int j = 0 ; j < n ; j ++)
		{
			t_top[j] = tt[j] ;
		}
		for(int j = 0 ; j < tl_c ; j ++)
		{
			L[j] = tl[j] ;
		}
		for(int j = 0 ; j < m ; j ++)
		{
			for(int k = 0 ; k < n ; k ++)
			{
				t_board[j][k] = tb[j][k] ;
			}
		}
	}
}

int MTKL(int pos)
{
	int n_user = node[pos].user ;
	int y0 = node[pos].col ;
	int x0 = t_top[y0] ;
	int res = -1 ;
	res = Check(x0, y0, n_user) ;
	if(res != -1) return res ;//如果一旦放置该节点胜负已定，则无需再进行模拟，直接返回结果
	while(l_num > 0)
	{
		n_user = 3 - n_user ;
		int z = rand() % l_num ;
		y0 = L[z] ;//随机算一列
		x0 = put_chess(y0, n_user) ;//放置棋子，判断胜负
		/*if(n_user == 2) //电脑方，判断胜负
		{
			if(machineWin(x0, y0, m, n, t_board)) return 2 ;
			else if(isTie(n, t_top)) return 1 ;
			else continue ;
		}
		else // 用户赢，这说明计算机输
		{
			if(userWin(x0, y0, m, n, t_board)) return 0 ;
			else if(isTie(n, t_top)) return 1 ;
			else continue ;
		}*/
		res = Check(x0, y0, n_user) ;
		if(res != -1) return res ;
		else continue ;
	}
	return 1 ;//无子可下，说明平局
}

int Check(int x, int y, int u)
{
	int res = -1 ;
	if(u == 2)
	{
		if(machineWin(x, y, m, n, t_board)) res = 2 ;
		else if(isTie(n, t_top)) res = 1 ;
		else res = -1 ;
	}
	else
	{
		if(userWin(x, y, m, n, t_board)) res = 0 ;
		else if(isTie(n, t_top)) res = 1 ;
		else res = -1 ;
	}
	return res ;
}

void Pre(int &x, int &y, const int *top, int ** board)
{
	for(int i = 0 ; i < l_num ; i ++)
	{
		int y0 = L[i] ;
		int x0 = put_chess(y0, 1) ;
		if(userWin(x0, y0, m, n, t_board))
		{
			ifPre = 1 ;
			x = x0 ;
			y = y0 ;
			init_t(top, board) ;
			return ;
		}
		else init_t(top, board) ;
	}
}


