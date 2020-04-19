#ifndef LIST_H
#define LIST_H

#define LIST_END -1

#define OBJ void*
typedef struct {
	union {
		int val;//����ֵ
		OBJ obj;//�����ַ
	} ele;//Ԫ��
	struct ListNode* next;//��һ��node
}ListNode;
typedef struct {
	int eleLen;//������
	int eleSize;//Ԫ�ش�С
	struct ListNode* first;//��ʼԪ��
	struct ListNode* last;//ĩβԪ��
} List;//����ͷ



List* ListCreat(int eleSize);
//�������n��Ԫ�غ����һ��Ԫ��,ĩβΪLIST_END
void ListAdd(List* list, OBJ obj, int n);
//��ȡ��n��Ԫ��
OBJ ListGet(List* list, int n);
//������Ԫ����������ʽ���ز���������
void* ListGetAll(List* list);
//������������Ԫ��
void ListDeleAll(List* list);

#endif
