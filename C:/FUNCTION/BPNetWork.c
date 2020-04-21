#include<math.h>
#include"BPNetWork.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//������Ĳ���
#define LS network->lns

//�������Ԫ������
#define INNS network->ns[0]

//�����ĵ�a������
#define INS(a) network->is[a-1]

//��a���������
#define TAS(a) network->ts[a-1]

//�������Ԫ������
#define OUTNS network->ns[LS-1]

//��n����Ԫ������
#define NS(n) network->ns[n-1]

//��n���a����Ԫ�ĵ�p��Ȩ��
#define WF(n,a,p) network->las[n-2].ws[(p-1)+(a-1)*NS(n-1)]

//��n��ĵ�a����Ԫ��ƫ��
#define BF(n,a) network->las[n-2].bs[a-1]

//��n���a����Ԫ�����
#define OF(n,a) network->las[n-2].os[a-1]

//��n���a����Ԫ�����
#define SF(n,a) network->las[n-2].ss[a-1]

//ѧϰ��
#define LN network->ln

BPNetWork* BPCreate(int* nums, int len,double ln)
{
	BPNetWork* network = malloc(sizeof(BPNetWork));
	network->lns = len;
	network->ns = malloc(len * sizeof(int));
	network->ln = ln;
	memcpy(network->ns, nums, len * sizeof(int));
	//
	network->is = malloc(nums[0] * sizeof(double));
	network->las = malloc(sizeof(Layer) * (len - 1));
	network->ts = malloc(sizeof(double) * nums[len - 1]);
	for (int p = 0; p < len - 1; p++) {
		srand(&p);
		int lastnum = nums[p];//��һ�����Ԫ����
		int num = nums[p + 1];//��ǰ�����Ԫ����
		network->las[p].bs = malloc(sizeof(double) * num);
		//
		network->las[p].ws = malloc(sizeof(double) * num * lastnum);
		//
		network->las[p].os = malloc(sizeof(double) * num);
		//
		network->las[p].ss = malloc(sizeof(double) * num);
		for (int pp = 0; pp < num; pp++) {
			network->las[p].bs[pp] = 0.2;//rand() / 2.0 / RAND_MAX;
			for (int ppp = 0; ppp < lastnum; ppp++) {
				network->las[p].ws[ppp + pp * lastnum] = 0.5;//rand() / 2.0 / RAND_MAX;
			}
		}
	}
	return network;
}
//����һ��������
void RunOnce(BPNetWork* network) {
	//��������㵽�ڶ���
	for (int a = 1; a <= NS(2); a++) {
		double net = 0;
		double* o = &OF(2,a);//��ȡ��2������ֵ
		*o = 0;
		for (int aa = 1; aa <= INNS; aa++) {
			net += INS(aa) * WF(2, a, aa);//INS(aa) * WF(2, a, aa);
		}
		*o = f(net + BF(2,a));
	}
	for (int n = 2; n <= LS-1; n++) {
		for (int a = 1; a <= NS(n + 1); a++) {//��һ���������
			double net = 0;
			double* o = &OF(n+1,a);
			*o = 0;
			for (int aa = 1; aa <= NS(n); aa++) {//��ǰ���������
				double oo = OF(n, aa);
				double* ww = &WF(n + 1, a, aa);
				net += oo * (*ww);
			}
			*o = f(net + BF(n + 1, a));
			*o = *o;
		}
	}
}

//����ѵ����
void LoadIn(BPNetWork* network,double* input,double* putout) {
	memcpy(network->is, input, INNS*sizeof(double));
	memcpy(network->ts, putout, OUTNS*sizeof(double));
}

//���򴫲�һ��(ѵ��һ��)
void TrainOnce(BPNetWork* network) {
	//��������������
	for (int a = 1; a <= OUTNS; a++) {
		double* s = &SF(LS,a);//��ȡ��a����Ԫ�����
		*s = 0;
		double* b = &BF(LS, a);//��ȡ��a����Ԫ��ƫ��
		double o = OF(LS, a);//��ȡ��a����Ԫ�����
		*s = (2.0 / OUTNS) * (o - TAS(a))* f_(o);
		*b = *b - LN * (*s);//����ƫ��
		//����Ȩ��
		for (int aa = 1; aa <=NS(LS-1) ; aa++) {
			double* w = &WF(LS, a, aa);
			*w = *w - LN * (*s) * OF(LS-1, aa);
		}
	}
	
	//�������ز�����
	for (int a = LS-1; a > 2; a--) {
		//��ʼ�����a��ÿ����Ԫ�����
		for (int n = 1; n <= NS(a); n++) {//��ǰ��
			double* s = &SF(a, n);//��ȡ��a����Ԫ�����
			*s = 0;
			double* b = &BF(a, n);//��ȡ��a����Ԫ��ƫ��
			double o = OF(a, n);//��ȡ��a����Ԫ�����
			for (int nn = 1; nn <= NS(a+1); nn++) {//��һ��
				double lw = WF(a + 1, nn, n);//��ȡ��һ�㵽��ǰ��Ԫ��ƫ��
				double ls = SF(a + 1, nn);//��ȡ��һ���nn����Ԫ�����
				*s += ls * lw * f_(o);
			}
			*b = *b - LN * (*s);//����ƫ��
			//����Ȩ��
			for (int nn = 1; nn <= NS(a - 1); nn++) {//��һ��
				double* w = &WF(a, n, nn);
				*w = *w - LN * (*s) *OF(a - 1, nn);
			}
		}
	}
	
	//�����2�������
	for (int n = 1; n <= NS(2); n++) {//��ǰ��
		double* s = &SF(2, n);//��ȡ��a����Ԫ�����
		*s = 0;
		double* b = &BF(2, n);//��ȡ��a����Ԫ��ƫ��
		double o = OF(2, n);//��ȡ��a����Ԫ�����
		for (int nn = 1; nn <= NS(3); nn++) {//��һ��
			double lw = WF(3, nn, n);//��ȡ��һ�㵽��ǰ��Ԫ��ƫ��
			double ls = SF(3, nn);//��ȡ��һ���nn����Ԫ�����
			*s += ls * lw * f_(o);
		}
		*b = *b - LN * (*s);//����ƫ��
		//����Ȩ��
		for (int nn = 1; nn <= INNS; nn++) {//��һ��
			double* w = &WF(2, n, nn);
			*w = *w - LN * (*s) * INS(nn);
		}
	}
	
}
//��������
double ETotal(BPNetWork* network) {
	double val = 0;
	for (int a = 1; a <= OUTNS; a++) {
		val += ((OF(LS, a) - TAS(a)) * (OF(LS, a) - TAS(a))) / OUTNS;
	}
	return val;
}

int main() {
	int a[] = { 1,20,20,1 };
	double in[1] = { 0.5 };
	double in1[1] = { 0.2 };


	double out[1] = { 0.1 };



	BPNetWork* network = BPCreate(a, 4, 0.5);

	double* l = &BF(3, 1);

	LoadIn(network, in, out);

	while (1)
	{
		RunOnce(network);
		TrainOnce(network);
		double a = OF(4, 1);
		printf("%g\n", a);
	}






	return 0;
}
















