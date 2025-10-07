#include "all.h"
int FunNum;//��¼cnf�б�Ԫ����
HeadNode* CnfParser(string& filename){
    string PATH=filename;
    ifstream fp(PATH);
    if(!fp){cout << "File can not open"; exit(0);}
    char ch;
    char exp[105];//exp:��cnf�ļ��Ľ���explain
    fp >> ch;
    while(ch != 'p'){
        fp.getline(exp, 100);
        //ע�⣺�˴���expֻ������c-string�����飩��������string
        //��ϸ����:n:Pointer to an array of characters where extracted characters are stored as a c-string.
        fp >> ch;
    }
    string cnf; int ClauseNum;//�ֱ�洢CNF����Ԫ�������Ӿ������
    fp >> cnf >> FunNum >> ClauseNum;
    //����������cout << ClauseNum << " " << FunNum << endl;
    //����ѭ�������е��Ӿ���뵽��ƺõ����ݽṹ��
    HeadNode* L = new HeadNode;//LΪHeadNode����ͷ���
    L->num = ClauseNum;//�־������Ҳ�����ж����У�
    L->right=nullptr;
    HeadNode* pH = new HeadNode; L->down = pH; pH->right = nullptr; pH->down = nullptr;//L��num��ʾ�ж�����
    for(int i = 1; i <= ClauseNum; i++){
        int tep;
        fp >> tep;
        DataNode* p = new DataNode;
        p->value = tep; pH->right = p; pH->num++; p->next = nullptr;
        fp >> tep;
        while(tep){
            p = new DataNode;
            p->value = tep;//����ͷ�巨��������
            p->next = pH->right;
            pH->right = p;
            pH->num++;
            fp >> tep;
        }
        if(i != ClauseNum){
            pH = new HeadNode;
            pH->down = L->down;
            L->down = pH;
        }
        //fp.get();//get�����з�
    }
    fp.close();//�ر��ļ���
    
    //���������cnf�ļ����������ʾ�����ڽ��д�������
    // HeadNode* _pH = L->down;

    // while(_pH){
    //     DataNode* _p = _pH->right;
    //     while(_p->next){
    //         cout << _p->value << " ";
    //         _p = _p->next;
    //     }
    //     cout << _p->value << endl;
    //     _pH = _pH->down;
    // }


    return L;
}

// void FreeList(HeadNode* L){
//     //�ͷŵ�L����������ڴ�ռ�
//     HeadNode* _pH = L->down;
//     while(_pH){
//         DataNode* _p = _pH->right;
//         while(_p){
//             DataNode* _fp = _p;
//             if(_p->next!=NULL)
//             {
//             _p = _p->next; 
//             delete _fp;
//             }
//             else break;
//         }
//         HeadNode* _fpH = _pH;
//         _pH = _pH->down;
//         delete _fpH;
//     }
//     delete L;
// }
void FreeList(HeadNode* L){
    // �ȼ��L�Ƿ�Ϊ�գ�������ʿ�ָ��
    if (L == nullptr) return;

    HeadNode* _pH = L->down;
    while(_pH != nullptr){  // ��nullptr���淶��C++11+��
        // �ͷŵ�ǰHeadNode��Ӧ������DataNode
        DataNode* _p = _pH->right;
        while(_p != nullptr){  // ��������DataNode���������һ��
            DataNode* _fp = _p;  // ���浱ǰ�ڵ�
            _p = _p->next;       // ���ƶ�����һ���ڵ�
            delete _fp;          // ���ͷŵ�ǰ�ڵ㣨����next�Ƿ�Ϊ�գ�
            _fp=nullptr;
        }

        // �ͷŵ�ǰHeadNode
        HeadNode* _fpH = _pH;
        _pH = _pH->down;  // �ƶ�����һ��HeadNode
        delete _fpH;
        _fpH=nullptr;
    }

    // ����ͷ�ͷ�ڵ�L
    delete L;
    L=nullptr;
}
