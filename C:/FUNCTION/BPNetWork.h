#ifndef BPNETWORK_H
#define BPNETWORK_H

#define f(x) Sigmoid(x)//������趨
#define f_(x) Sigmoidf(x)//������

typedef struct {
	double* ws;//Ȩ�ؾ���
	double* bs;//ƫ������
	double* os;//�������
	double* ss;//���(�������ڼ�Ȩ�͵�ƫ��)
} Layer;
typedef struct {
	int lns;//����
	int* ns;//ÿ����Ԫ������
	double* is;//����������
	double* ts;//�������
	Layer* las;//�����������(�����������)
	double ln;//ѧϰ��
}BPNetWork;








//����������
BPNetWork* BPCreate(int* nums, int len,double ln);
//����һ��������
void RunOnce(BPNetWork* network);





//sigmoid�����
#define Sigmoid(x)  (1 / (1 + exp(-(x))))
//sigmoid������ĵ�����,����Ϊsigmoid���
#define Sigmoidf(f)  ((f) * (1 - (f)))


#define Tanh(x) ((2 / (1 + exp(-2 * (x))))-1)
#define Tanhf(f) ((1+(f))*(1-(f)))




#endif
