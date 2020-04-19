#include "List.h"
#include<stdlib.h>
#include<string.h>
ListNode* ListGetNode(List* list, int n);


List* ListCreat(int eleSize) {
	List* list = malloc(sizeof(List));
	list->eleLen = 0;
	list->eleSize = eleSize;
	list->first = 0;
	list->last = 0;
	return list;
}
//�������n��Ԫ�غ����һ��Ԫ��,ĩβΪLIST_END
void ListAdd(List* list,OBJ obj,int n) {
	ListNode* node = malloc(sizeof(ListNode));
	node->ele.obj = obj;
	if (list->eleLen == 0) {
		list->first = node;
		list->last = node;
		list->eleLen++;
		return;
	}
	if (n == LIST_END) {
		ListNode* p = list->last;
		p->next = node;
		list->last = node;	
		list->eleLen++;
		return;
	}
	ListNode* a = ListGetNode(list, n);
	ListNode* b = ListGetNode(list, n+1);
	a->next = node;
	node->next = b;
	list->eleLen++;
}
//��ȡ��n��Ԫ��
OBJ ListGet(List* list, int n) {
	return ListGetNode(list, n)->ele.obj;
 }
//������Ԫ����������ʽ���ز���������
void* ListGetAll(List* list) {
	void* eles = malloc(list->eleLen * list->eleSize);
	for (int a = 0; a < list->eleLen; a++) {
		memcpy((char*)eles + a * list->eleSize, ListGet(list, a + 1), list->eleSize);
	}
	ListDeleAll(list);
	return eles;
}
//������������Ԫ��
void ListDeleAll(List* list) {
	for (int a = list->eleLen; a > 0; a--) {
		free(ListGet(list, a));
		free(ListGetNode(list, a));
	}
	free(list);
}
//��ȡ��n������ڵ�
ListNode* ListGetNode(List* list,int n) {
	if (n == LIST_END||n>=list->eleLen) {
		return list->last;
	}
	int a = 1;
	ListNode* p = list->first;
	for (; a != n; a++)p = p->next;
	return p;
}