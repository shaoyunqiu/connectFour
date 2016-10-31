/********************************************************
*	Strategy.h : ���Խӿ��ļ�                           *
*	������                                              *
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
	������Լ��ĸ�������
*/

class Node //���ؿ������ڵ���
{
	public:
		int col ;//���ӵ���
		int win ;//ʤ������
		int lose ;//��ĳ���
		int tie ;//ƽ�ֳ���
		bool isleaf ;//�ж��Ƿ�ΪҶ��
		int child[13] ;//������node�е�λ��
		int child_num ;//��������
		int user ;//��¼�ýڵ��ʹ���ߣ��û�1������2����ʼΪ0
		int parent ;//��¼���ڵ�λ��
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

//void init_n(int pos) ;//��ʼ���ڵ�

void init_t(const int *top, int **board) ;//��ʼ�����̣��ָ����̣��Լ��ָ�L

int TreePolicy(int pos) ;//ѡ�����չ�Ľڵ㣬������expand������չ��������չ�ڵ�ĸ��ڵ�λ�ã�֮���������չ��ÿһ�����ӽ���һ��ģ��

void Default(int pos) ;//�Ըýڵ�����к��ӽ���ģ�⣬�����ݽ��

int BestChild(int pos) ;//������õĺ��ӵ�λ��

void Expand(int pos) ;//һ����չ�����к���

int MTKL(int pos) ;//�ڸýڵ��Ͻ������ؿ���ģ�⣬ʤ������2��ƽ�ַ���1��ʧ�ܷ���0

double Value(int pos, int p_sum) ;//�����Ӧ��ucbֵ���������userΪ2����ʤ�ʣ�userΪ1�������

void Update(int pos, int val) ;//�ѽ�����ݸ�����

void Update_L() ;//���¿������

int put_chess(int y, int u) ;//����,������x����

int Check(int x, int y, int u) ;//�ж���ʤ��2�� ��0��ƽ��1, ʤ��δ��-1

void Pre(int &x, int &y, const int *top, int **board) ;//��

#endif