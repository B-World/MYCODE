//#include"ByteView.h"
#include<stdio.h>
#include<Windows.h>
#define MaxColumn 20//�����ʾ����
#define MaxLine 100//�����ʾ����



FILE* fp;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("#��������\n");
		return -1;
	}
	fp = fopen(argv[1],"rb+");
	if (!fp) {
		printf("#�ļ�������\n");
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	int counter = 0;
	for (int a; (a = fgetc(fp)) != EOF; counter++) {
		if (counter == MaxColumn) {
			counter = 0;
			printf("\n");
		}
		if (a < 17)printf("0");
		printf("%x,", a);
	}
	printf("\n#�ļ�����:%d\n", len);
	printf("#�ļ���ʾ���");
	
}