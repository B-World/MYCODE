#include<math.h>
#include"BPNetWork.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//������Ĳ���
#define LS network->layernum

//���������ز��������Ĳ���
#define AS network->layernum-1

//�������Ԫ������
#define INNS network->neurons[0]

//�����ĵ�a������
#define INS(a) network->input[a-1]

//�������Ԫ������
#define OUTNS network->neurons[network->layernum-1]

//��n����Ԫ������
#define NS(n) network->neurons[n-1]


//��n���a����Ԫ�ĵ�p��Ȩ��
#define WF(n,a,p) network->layers[n-2].ws[(p-1) + (a-1)*network->neurons[n-2]]

//��n��ĵ�a����Ԫ��ƫ��
#define BF(n,a) network->layers[n-2].bs[a-1]

//��n���a����Ԫ�����
#define OF(n,a) network->layers[n-2].ps[a-1]

//��n���a����Ԫ�����
#define SF(n,a) network->layers[n-2].ss[a-1]

BPNetWork* BPCreate(int* nums, int len,double ln)
{
	BPNetWork* network = malloc(sizeof(BPNetWork));
	network->layernum = len;
	network->neurons = malloc(len * sizeof(int));
	network->ln = ln;
	memcpy(network->neurons, nums, len * sizeof(int));
	//
	network->input = malloc(nums[0] * sizeof(double));
	network->layers = malloc(sizeof(Layer) * (len - 1));
	network->out = malloc(sizeof(double) * nums[len - 1]);
	for (int p = 0; p < len - 1; p++) {
		srand(&__TIME__);
		int lastnum = nums[p];//��һ�����Ԫ����
		int num = nums[p + 1];//��ǰ�����Ԫ����
		network->layers[p].nums = num;
		network->layers[p].bs = malloc(sizeof(double) * num);
		//
		network->layers[p].ws = malloc(sizeof(double) * num * lastnum);
		//
		network->layers[p].ps = malloc(sizeof(double) * num);
		//
		network->layers[p].ss = malloc(sizeof(double) * num);
		for (int pp = 0; pp < num; pp++) {
			network->layers[p].bs[pp] = rand() / RAND_MAX;
			for (int ppp = 0; ppp < lastnum; ppp++) {
				network->layers[p].ws[ppp + pp * lastnum] = rand() / RAND_MAX;
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
		double* putout = &OF(2,a);//��ȡ��2������ֵ
		*putout = 0;
		for (int aa = 1; aa <= INNS; aa++) {
			net += INS(aa) * WF(2, a, aa);
		}
		*putout = f(net + BF(2,a));
	}
	for (int n = 2; n <= LS-1; n++) {
		for (int a = 1; a <= NS(n + 1); a++) {//��һ���������
			double net = 0;
			double* putout = &OF(n+1,a);
			*putout = 0;
			for (int aa = 1; aa <= NS(n); aa++) {//��ǰ���������
				net += OF(n, aa) * WF(n + 1, a, aa);
			}
			*putout = f(net + BF(n + 1, a));
		}
	}
}

//����ѵ����
void LoadIn(BPNetWork* network,double* input,double* putout) {
	memcpy(network->input, input, INNS*sizeof(double));
	memcpy(network->out, putout, OUTNS*sizeof(double));
}

//���򴫲�һ��(ѵ��һ��)
void TrainOnce(BPNetWork* network) {
	//��������������
	for (int a = 1; a <= OUTNS; a++) {
		double* s = &SF(LS,a);//��ȡ��a����Ԫ�����
		*s = 0;
		double* b = &BF(LS, a);//��ȡ��a����Ԫ��ƫ��
		double o = OF(LS, a);//��ȡ��a����Ԫ�����
		*s = 2 * (o - network->out[a]) * f_(o);
		*b = *b - network->ln * (*s);//����ƫ��
		//����Ȩ��
		for (int aa = 1; aa <=NS(LS-1) ; aa++) {
			double* w = &WF(LS, a, aa);
			*w = *w - network->ln * (*s) * OF(LS - 1, aa);
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
			*b = *b - network->ln * (*s);//����ƫ��
			//����Ȩ��
			for (int nn = 1; nn <= NS(a - 1); nn++) {//��һ��
				double* w = &WF(a, n, nn);
				*w = *w - network->ln * (*s) *OF(a - 1, nn);
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
		*b = *b - network->ln * (*s);//����ƫ��
		//����Ȩ��
		for (int nn = 1; nn <= INNS; nn++) {//��һ��
			double* w = &WF(2, n, nn);
			*w = *w - network->ln * (*s) * INS(nn);
		}
	}
}
//��������
double ETotal(BPNetWork* network) {
	double val = 0;
	for (int a = 1; a <= OUTNS; a++) {
		val += (OF(network->layernum, a) - network->out[a]) * (OF(network->layernum, a) - network->out[a]) / OUTNS;
	}
	return val;
}

int main() {
	int a[] = { 5,100,100,5 };
	double in[5] = { 10,10,10,10,10, };
	double out[5] = { 0.1,0.1,0.1,0.1,0.1 };


	double in1[5] = { 3,3,3,3,3, };
	double out1[5] = { 0.5,0.1,0.3,0.1,0.2 };
	BPNetWork* network = BPCreate(a, 4,0.5);
	
	

	while (1)
	{
		int w = 100;
		LoadIn(network, in, out);
		while (w--)
		{
			RunOnce(network);
			TrainOnce(network);
			printf("%g\n", ETotal(network));


			//system("cls");
		}
		w = 100;
		printf("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");
		LoadIn(network, in1, out1);
		while (w--)
		{
			RunOnce(network);
			TrainOnce(network);
			printf("%g\n", ETotal(network));
		}
	}
	
	
	return 0;
}
















