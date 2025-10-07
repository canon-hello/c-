#include"all.h"
extern int FunNum;
bool DPLL(HeadNode* L,int* book){
    int SC;//SingleClause?
    while(SC=isHaveSingleClause(L)){
        if(SC>0) book[SC]=1;
        else if(SC<0) book[0-SC]=-1;
        SimplifyCnf(L,SC);
        if(isemptyCnf(L)) return TRUE;
        else if(isHaveEmptyClause(L)) return FALSE;
    }
    int v = SelectWord2(L);
    // int v = SelectWord2(L);//采用最短子句优先选取规则进行选取变元
    //cout << "这次选取的变元为：" << v << endl;
    HeadNode* temL = CopyList(L);
    if(DPLL(merge(L, v), book)) return TRUE;
    bool result = DPLL(merge(temL, 0-v), book);
    //删除temL的内容空间
    FreeList(temL);
    return result;
}

// bool NewDPLL(HeadNode* L, int* book){//非递归版本DPLL算法
//     stack<HeadNode*> S;
//     HeadNode* ListL[10000] = {nullptr}; int i = 0;//ListL用来记录每次改变的链表结点内容
//     HeadNode* temL = nullptr;
//     int v[10000]; int val;
//     S.push(L);
//     while(!S.empty()){
//         while((L = S.top()) && (val = SingleSpread(L, book)) == NOMAL){
//             cout << "循环开始" << endl; printList(L);
//             temL = CopyList(L);
//             ListL[i] = temL;
//             v[i] = SelectWord2(L);
//             cout << "选取的变元为：" << v[i] << endl;
//             L = merge(L, v[i]);
//             S.push(L);i++;
//         }
//         //printList(L); 
//         cout << "循环结束" << endl;
//         if(val == TRUE) {
//             cout<<"真"<<endl;
//             HeadNode* temp = S.top();
//             while(!S.empty()) {
//             temp = S.top();
//             S.pop();
//             cout<<"马上释放一个栈"<<endl;
//             FreeList(temp);
//             }
//             cout<<"栈已经全部释放"<<endl;
//             i--;
//             while(i>=0)
//             {
//                 FreeList(ListL[i]);
//                 i--;
//             }
        
//         return TRUE;
//         }// 提前返回TRUE前，释放栈中所有剩余元素
//         S.pop();i--;
//         cout << "此处的变元为：" << v[i] << endl;
//         FreeList(L);
//         cout << "栈的大小为" << S.size() << endl;
//         if(!S.empty()){
//             L = ListL[i]; 
//             S.push(L=merge(L, -v[i]));
//         }
//     }
//     while(i>=0)
//             {
//                 FreeList(ListL[i]);
//                 i--;
//             }
//     return FALSE;
// }
bool NewDPLL(HeadNode* L, int* book){//非递归版本DPLL算法
    stack<HeadNode*> S;
    HeadNode* ListL[10000] = {nullptr}; int i = 0;//ListL用来记录每次改变的链表结点内容
    HeadNode* temL = nullptr;
    int v[10000]; int val;
    S.push(L);
    while(!S.empty()){
        while((L = S.top()) && (val = SingleSpread(L, book)) == NOMAL){
            //cout << "循环开始" << endl; printList(L);
            temL = CopyList(L);
            ListL[i] = temL;
            v[i] = SelectWord2(L);
            //cout << "选取的变元为：" << v[i] << endl;
            L = merge(L, v[i]);
            S.push(L);i++;
        }
        //printList(L); cout << "循环结束" << endl;
        if(val == TRUE) return TRUE;
        S.pop();i--;
        //cout << "此处的变元为：" << v[i] << endl;
        FreeList(L);
        //cout << "栈的大小为" << S.size() << endl;
        if(!S.empty()){
            L = ListL[i]; S.pop();
            S.push(merge(L, -v[i]));
        }
    }
    return FALSE;
}

int SingleSpread(HeadNode* L, int* book){
    int SC;//存储单子句(SingleClause的缩写)
    //cout << ++TestNum << endl;
    //printList(L);
    if(!(SC=isHaveSingleClause(L)))
    {
        SC=isHavePlainText(L);
    }
    while(SC){
        //cout << "2-" << TestNum << " 本次循环要删除的为:" << SC << endl;
        if(SC > 0) book[SC] = 1;
        else if(SC < 0) book[0-SC] = -1;//例如：book[-(-2)] = -1表示2代表的变元取负值，即-2
        SimplifyCnf(L, SC);//根据单子句传播策略简化单子句
       //printList(L);
        if(isemptyCnf(L)) return TRUE;
        else if(isHaveEmptyClause(L)) return FALSE;

        if(!(SC=isHaveSingleClause(L)))
        {
            SC=isHavePlainText(L);
        }
    }
    return NOMAL;
}
int isHavePlainText(HeadNode* L){//判断L中是否含有纯文字
    int* mark1 = new int[FunNum + 1]();//标记数组，标记每个变元出现的正负情况
    int* mark2 = new int[FunNum + 1]();
    HeadNode* _pH = L->down;
    while(_pH){
        DataNode* _p = _pH->right;
        while(_p){
            if(_p->value > 0) mark1[_p->value] = 1;
            else if(_p->value < 0) mark2[0 - _p->value] = 1;
            _p = _p->next; 
        }
        _pH = _pH->down;
    }
    for(int i = 1; i <= FunNum; i++){
        if(mark1[i]==1&&mark2[i]==0){ delete[] mark1;delete[] mark2;return i;}
        else if(mark1[i]==0&&mark2[i]==1) {delete[] mark1;delete[] mark2;return 0-i;}
    }
    delete[] mark1;delete[] mark2;
    return 0;
}
//L中存储的合区范式是否含有单子句（只含有一个文字的子句叫做单子句）
//如果含有单子句，则返回单子句的值
//如果不含有单子句，则返回0
int isHaveSingleClause(HeadNode* L){
    HeadNode* _pH = L->down;
    while(_pH){
        if(_pH->num == 1) return _pH->right->value;
        _pH = _pH->down;
    }
    return 0;
}

//根据单子句SC进行化简范式L——含有SC的子句直接去掉，含有-SC的子句中去掉-SC这个文字
void SimplifyCnf(HeadNode* L, int SC){
    HeadNode* _pH = L->down;
    HeadNode* _fpH = L;//记录之前遍历过的结点
    int flag;
    while(_pH){
        DataNode* _p = _pH->right;
        DataNode* _fp = _p;
        flag = 0;
        while(_p){
            flag = 0;
            if(_p->value == SC){//去掉这个子句（删除一行）
                _fpH->down = _pH->down;
                _p = _pH->right;
                while(_p){
                    _fp = _p;
                    _p = _p->next;
                    delete _fp;
                }
                L->num--;
                flag = 1;
                break;//跳过这一行
            }
            else if(_p->value == 0-SC){//去掉这个文字（删除一个点，需要考虑第一个节点的特殊情况）
                if(_pH->right == _p){
                    flag = 2;//第一个结点就出现目标数据，需要删除
                    _pH->right = _p->next;
                    _pH->num--;
                    //cout << "第一个结点处地文字删除后num的值为：" << _pH->num << endl;
                }
                else{
                    flag = 3;
                    _fp->next = _p->next;
                    _pH->num--;
                }
            }
            if(!flag){//没有需要删除的节点
                if(_fp != _p) _fp = _p;
                _p = _p->next;
            }
            else if(flag == 2){//删除的结点为第一个节点的情况处理
                _p = _pH->right;
                _fp = _p;
            }
            else if(flag == 3){//删除的节点不是第一个，在中间
                delete _p;
                _p = _fp->next;
            } 
        }
        if(!flag){
            _fpH = _pH;
            _pH = _pH->down;
        }
        else if(flag == 1){
            delete _pH;
            _pH = _fpH->down;
        } 
    }
}

//L中存储的范式是否为空范式
bool isemptyCnf(HeadNode* L){
    if(L->num) return FALSE;
    else return TRUE;
}

//判断范式中是否有空子句
bool isHaveEmptyClause(HeadNode* L){
    HeadNode* _pH = L->down;
    while(_pH){
        if(_pH->num == 0) return TRUE;
        _pH = _pH->down;
    }
    return FALSE;
}

//将v作为一个单子句合并到范式L中，将合并后的范式作为返回值
HeadNode* merge(HeadNode* L, int v){
    HeadNode* pH = new HeadNode; pH->num++;
    pH->down = L->down;
    L->down = pH;
    DataNode* p = new DataNode; p->value = v;
    pH->right = p; p->next = nullptr;
    L->num++;
    //cout << "merge完之后添加的v为：" << L->down->right->value << endl;
    return L;
}

//直接返回第一个文字即可
int SelectWord(HeadNode* L){
    HeadNode* _pH = L->down;
    int ans;
    while(_pH){
        DataNode* _p = _pH->right;
        while(_p){
            ans = _p->value;
            return ans;
        }
        _pH = _pH->down;
    }
    cout << "范式为空集，无法选择文字返回" << endl;
    return 0;
}

int SelectWord2(HeadNode* L){//最短子句优先
    HeadNode* _pH = L->down;
    int _min, ans = 0;
    if(!_pH){cout << "范式为空集，无法选择文字返回" << endl; return 0;}
    _min = _pH->num; ans = _pH->right->value;
    while(_pH){
        if(_pH->num != 0 && _pH->num < _min) ans = _pH->right->value;
        _pH = _pH->down;
    }
    return ans;
}

//仅用于测试使用
void printList(HeadNode* L){
    HeadNode* _pH = L->down;
    int i = 1;
    while(_pH){
        cout << "第" << i++ << "行：" << _pH->num;
        DataNode* _p = _pH->right;
        while(_p){
            cout << "->" << _p->value;
            _p = _p->next; 
        }
        cout << endl;
        _pH = _pH->down;
    }
}

//将L中全部内容都拷贝到temL中，函数结构与cnfparser函数基本相同
HeadNode* CopyList(HeadNode* L){
    HeadNode* temL = new HeadNode;
    temL->num = L->num;
    HeadNode* pH = L->down;
    HeadNode* _pH = new HeadNode; temL->down = _pH; _pH->down = nullptr;
    for(int i = 1; i <= L->num; i++){
        _pH->num = pH->num;
        int tep = pH->right->value;
        DataNode* p = pH->right;
        DataNode* _p = new DataNode;
        _p->value = tep; _pH->right = _p; _p->next = nullptr;
        while(p->next){
            p = p->next; tep = p->value;
            _p = new DataNode;
            _p->value = tep;//采用头插法建立链表
            _p->next = _pH->right;
            _pH->right = _p;
        }
        if(i != L->num){
            _pH = new HeadNode;
            _pH->down = temL->down;
            temL->down = _pH;
        }
        pH = pH->down;
    }
    return temL;
}