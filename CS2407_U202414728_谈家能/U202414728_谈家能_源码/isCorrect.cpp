//isCorrect.cpp
#include"all.h"
extern int FunNum;
bool verification(HeadNode* ordL, int* book);
//��֤����DPLL�㷨���ɵ��ļ��Ƿ���ȷ
bool isCorrect(string& filename){
    HeadNode* L = CnfParser(filename);
    HeadNode* ordL = CopyList(L);

    int _FunNum = FunNum>1000? FunNum: 1000;
    int *book = new int[_FunNum];
    memset(book, 0, sizeof(int)*_FunNum);

    int isTrue = NewDPLL(L, book);
    if(!isTrue){cout << ".cnf�ļ���������"; delete []book;exit(0);}
    else{
        if(verification(ordL, book)){ delete []book;return TRUE;}
        else {delete []book;return FALSE;}
    }
    
}
/*
�����ļ�����������L��ͬʱ����һ�ݲ���ı��ordL;������ı��L��
ִ��DPLL�㷨������.res�ļ�
���DPLL�㷨����Ϊ�٣�ֱ���˳����У�Ϊ�������һ��
ֱ�ӱ���ordL�ṹ������book��Ӧ�����ֵ�ֵ
�����һ���Ӿ�����е����ֶ��������㣬��ô�ý����������
*/
bool verification(HeadNode* ordL, int* book){
    HeadNode* _pH = ordL->down;
    while(_pH){
        DataNode* _p = _pH->right;
        while(_p){
            if(book[abs(_p->value)]*_p->value > 0) break;
            _p = _p->next;
        }
        if(!_p) return FALSE;
        _pH = _pH->down;
    }
    return TRUE;
}