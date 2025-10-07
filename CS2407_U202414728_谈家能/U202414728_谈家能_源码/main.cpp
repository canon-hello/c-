#include "all.h"
extern int FunNum;
int zeroNum;
int isBetter;
int main() {
    display();
    int cod = 0;
    string filename;
    cin >> cod;
    while (cod){
        if(cod == 1){//cnf�������
            cout << "��������Ҫ������.cnf�ļ��������磺.\\src\\Sat\\S\\problem1-20.cnf��" <<endl;
            cin >> filename;
            cout << "��ѡ���Ƿ�ʹ���Ż����㷨" << endl;
            cout << "0--δ�Ż�" << endl; cout << "1--�Ż�" << endl;
            cin >> isBetter;
            HeadNode* L = CnfParser(filename);
            int temp[9][9];
            CoreFun(L, filename, FunNum, cod, temp);
        }
        else if(cod == 2){//�����������
            cout << "�����������������趨���ֵĸ�������������17��������81��" << endl;
            int preNum; cin >> preNum; zeroNum = 81-preNum;
            int array[ROW][COL] = {0};
            filename = createSudokuToFile(zeroNum, array);
            HeadNode* L = CnfParser(filename);
            //printList(L);
            CoreFun(L, filename, 1000, cod, array);
            /*
            ����û������Ĳ���
            �����û�������һ���գ���ָ����д�����ָ�ʽ�Լ���Ӧ����
                ���������Ϊ�㣬ֱ���˳�����
                ���û������������book�е������бȶԣ��鿴�Ƿ���ȷ
                �������ȷ�Ļ�����ʾ������������
                ��ȷ�Ļ����Ͷ������֣�Ȼ����û�����ĵط����Ǹ������޸�֮����������ӡ������ʾ���û�
            ��������п�Ԫ�ĸ���Ϊ�㣬֪ͨ�û��ɹ����أ���ѯ���Ƿ�����ٴ����¿�ʼ��Ϸ
            */
        }
        else if(cod == 3){
            cout << "�������������ļ��������磺.\\src\\Sat\\S\\problem1-20.cnf��" << endl;
            cin >> filename;
            if(isCorrect(filename)) {cout << "����Ľ��û����" << endl;}
            else {cout << "����Ľ������" << endl;}
        }
        display();
        TestNum = 0;
        cin >> cod;
    }
}

void CoreFun(HeadNode* L, string& filename, int FunNum, int cod, int array[ROW][COL]){

    //�ļ����׼������
    string _PATH = filename.replace(filename.end()-4, filename.end(), ".res");
    bool suc;
    int *book = new int[FunNum];
    memset(book, 0, sizeof(int)*FunNum);
    int begin = 0, end = 0;

    //DPLL()���Ĵ�����
    begin = clock();
    bool isTrue;
    if(!isBetter) {isTrue = DPLL(L, book);}
    else {isTrue = NewDPLL(L, book);}
    if(isTrue) {suc = TRUE;}
    else {suc = FALSE;}
    if(cod == 1){
        if(isTrue) {cout << "Ϊ��" << endl;}
        else {cout << "Ϊ��" << endl;}
        end = clock();

        cout << "DPLL()�������е�ʱ��Ϊ" << end-begin << "ms" << endl;
    }
    else{
        end = clock();
        cout << "DPLL()�������е�ʱ��Ϊ" << end-begin << "ms" << endl;
        cout << "������Ϸ��ʼ����������ʱ����0����ֹ��Ϸ" << endl;
        while(zeroNum){//������п�Ԫ
            cout << "���������������һ�����֣�111��ʾ�ڵ�һ�е�һ����������1" << endl;
            int opeNum; cin >> opeNum;
            int x = opeNum/100 - 1; int y = (opeNum/10)%10 - 1;
            if(!opeNum){exit(0);}
            if(book[opeNum] > 0 && array[x][y] == 0){
                array[x][y] = (opeNum%100)%10;
                zeroNum--;
                print(array);
                cout << "����ɹ�������д��һ��" << endl; 
            }
            else{
                cout << "��������ֲ���ȷ��" << endl;
                continue;
            }
        }
        cout << "��Ϸ��������ϲ���ɹ����أ�" << endl;
    }
    //���.res�ļ�
    OutFileFun1(_PATH, suc, book, end-begin, FunNum, cod);
}