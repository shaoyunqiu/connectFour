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
int m, n ;//�У���
const int Max_Num = 8000000 ;//���ڵ���
const double C = 0.8 ;//���㹫ʽ�еĳ���ֵ
Node node[Max_Num] ;//�洢���нڵ�
int cnt = 0 ;//��¼��ǰ�ڵ����
int *t_top ;//��ʱ�洢�ж����
int **t_board ; //��ʱ�洢�������
int no_x ;
int no_y ;//��¼�������ӵ�
int L[13] ; //�洢0~n-1�����У�Ϊ��֮�󷽱����ѡ��
int l_num = 0;//��¼��ʱL�Ĵ�С���������ӵ�����
int isEnd = -1 ;//�ж�ʤ���Ƿ��Ѷ�������Ѷ������������ģ�� ʤ��2�� ƽ��1�� �ܾ�0
clock_t start_time ;//��ʼʱ��
bool ifPre = 0 ;

/*
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������
	
	input:
		Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
		M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
		top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
		_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
				��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
				board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
				board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
		lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
				����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
		noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
				������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
				��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
				��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
		���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
		������ӵ�Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY){
	/*
		��Ҫ������δ���
	*/
	int x = -1, y = -1;//���ս�������ӵ�浽x,y��
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	start_time = clock();//��¼��ʼʱ��
	srand(unsigned(time(NULL))) ;//���������
	/*
		�������Լ��Ĳ������������ӵ�,Ҳ���Ǹ�����Ĳ�����ɶ�x,y�ĸ�ֵ
		�ò��ֶԲ���ʹ��û�����ƣ�Ϊ�˷���ʵ�֣�����Զ����Լ��µ��ࡢ.h�ļ���.cpp�ļ�
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
		t_board[i] = new int[N] ;	//��ʼ��������ռ�
	init_t(top, board) ;//��ʼ������
	node[0].init_n() ;
	node[0].col = lastY ;//��ʼ�����ڵ�
	node[0].user = 1 ;
	Pre(x, y, top, board) ;

	if(ifPre == 1)
	{
		put_chess(y, 2) ;
	}
	//�ȶ£� ���û�пɶ½ڵ��ٽ�������
	else{
	while(double(clock()-start_time) < 4500 && cnt < Max_Num) //����ʱ�ҽڵ����㹻
	{
		int now = TreePolicy(0) ;//������չ�ĸ��ڵ�λ��
		if(isEnd != -1) //����������ߵĹ����У�ʤ���Ѷ����������ٽ���ģ�⣬ֱ�Ӹ���ʤ����������
		{
			Update(now, isEnd) ;
		}
		else Default(now) ;//�����ѡ�ڵ�ʱʤ��δ�����������չ�ĺ��ӽڵ����ν���ģ��
		init_t(top, board) ;//�ָ�����
	}
	int ans = 1;
	int mu = (node[0].win + node[0].tie + node[0].lose)/node[0].child_num ;//���㸸�ڵ��ƽ��ֵ
	double pp = -1 ;
	for(int i = 0 ; i < node[0].child_num ; i ++)//�������ڵ�ĺ���
	{
		int cc = node[0].child[i] ;
		int sum = node[cc].win + node[cc].tie + node[cc].lose ;
		if(sum >= mu)//��Ϊ�Ƚϴ�������ƽ��ֵ��ʱ�򱻽���
		{
			double tmp = double(node[cc].win + node[cc].tie)/sum ;//����ʤ��
			if(tmp > pp)
			{
				pp = tmp ;
				ans = cc ;
			}
		}
	} //���һ������ʱѡ��ʤ�����ĵ�
	y = node[ans].col ;
	x = put_chess(y,node[ans].user);}
	/*
		��Ҫ������δ���
	*/
	clearArray(M, N, board);
	return new Point(x, y);
}


/*
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	���top��board����
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
*/

void init_t(const int *top, int **board) 
{
	l_num = 0 ;
	isEnd = -1 ;//��ʼ������־
	for(int i = 0 ; i < n ; i ++)
	{
		t_top[i] = top[i] ;
		if(top[i] > 0)//��������
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
	} //���������ӵ㣬ֻ�����ڲ������ӵ��Ϸ�
	Update_L() ;// ���¿������
	return x ;
}

void Update_L()
{
	for(int i = 0 ; i < l_num ; i ++)
	{
		if(t_top[L[i]] == 0) //�����������������ӣ�ɾ��
		{
			swap(L[i], L[l_num-1]) ;
			-- l_num ;//����͸����е����һ������������������1
		} 
		else continue ;
	}
}

void Update(int pos, int val)//�������Ƚ��ֵ
{
	while(pos != -1)
	{
		if(val == 0) //�ܾ�
		{
			++ node[pos].lose ;
			pos = node[pos].parent ;
			continue ;
		}
		else if(val == 1)//ƽ��
		{
			++ node[pos].tie ;
			pos = node[pos].parent ;
			continue ;
		}
		else //ʤ��
		{
			++ node[pos].win ;
			pos = node[pos].parent ;
			continue ;
		}
	}
}

int TreePolicy(int pos)
{
	while(!(node[pos].isleaf == 0 && node[pos].child_num == 0)) //������ֹ�ڵ�
	{
		if(node[pos].isleaf == 1)//Ҷ�ӽڵ㣬����չ
		{
			Expand(pos) ;
			node[pos].isleaf = 0 ;//���ı�־
			return pos ;
		}
		else
		{
			pos = BestChild(pos) ;
			put_chess(node[pos].col, node[pos].user) ;//����ѡ��ĺ��ӷ��������
			int nowy = node[pos].col ;
			int nowx = t_top[nowy] ;
			int nowr = Check(nowx, nowy, node[pos].user) ;
			if(nowr != -1) 
			{
				isEnd = nowr ;
				return pos ; // ���ʤ���Ѷ������Ľ�����־�����ظýڵ�
			}
		}
	}
	return pos ;
}

void Expand(int pos)
{
	int c_n = 0 ;//��¼���Ӹ���
	for(int i = 0 ; i < l_num ; i ++) //������չ��������
	{
		++ cnt ;
		node[cnt].init_n() ;//�ȶԸýڵ��ʼ��
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
	int sum = node[pos].win + node[pos].lose + node[pos].tie ;//���׽ڵ���ܾ���
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
	if(node[pos].user == 2 ) // ����ʤ��
	{
		int m_s = node[pos].win + node[pos].tie + node[pos].lose ;
		double X = double(node[pos].win + node[pos].tie) / m_s ;//��ʤ���÷���ƽ�ֵ÷����ֿ�
		X += C * sqrt(double(double(2*log(double(p_s)))/m_s)) ;
		return X ;
	}
	else // �û����������
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
	int tl_c = l_num ;//��ʱ�洢��ǰ����
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
	} // �������ؿ���֮ǰ������
	for(int i = 0 ; i < node[pos].child_num ; i ++)//��ÿ�����ӽ��е�һ��ģ��
	{
		int nchild = node[pos].child[i] ;//��ǰ�ڵ��λ��
		put_chess(node[nchild].col, node[nchild].user) ;//�ѵ�ǰ�ĺ��ӷ���������
		int res = MTKL(nchild) ;//�Ըú��ӽ���ģ��
		Update(nchild, res) ;
		//�ָ�����
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
	if(res != -1) return res ;//���һ�����øýڵ�ʤ���Ѷ����������ٽ���ģ�⣬ֱ�ӷ��ؽ��
	while(l_num > 0)
	{
		n_user = 3 - n_user ;
		int z = rand() % l_num ;
		y0 = L[z] ;//�����һ��
		x0 = put_chess(y0, n_user) ;//�������ӣ��ж�ʤ��
		/*if(n_user == 2) //���Է����ж�ʤ��
		{
			if(machineWin(x0, y0, m, n, t_board)) return 2 ;
			else if(isTie(n, t_top)) return 1 ;
			else continue ;
		}
		else // �û�Ӯ����˵���������
		{
			if(userWin(x0, y0, m, n, t_board)) return 0 ;
			else if(isTie(n, t_top)) return 1 ;
			else continue ;
		}*/
		res = Check(x0, y0, n_user) ;
		if(res != -1) return res ;
		else continue ;
	}
	return 1 ;//���ӿ��£�˵��ƽ��
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


