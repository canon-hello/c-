//SudokuSover.cpp
#include "all.h"

#define CORRECT 0
#define WRONG -1
static int T = 0;

//����holes���ڶ�
//�˺�������������̣�ͬʱ���ؽ������.cnf�ļ�
string createSudokuToFile(int holes, int array[ROW][COL]) {
    int sudoku[ROW][COL]={0};
    int starting_grid[ROW][COL]={0};
    createSudoku(sudoku);//������������
    createStartinggrid(sudoku,starting_grid,holes);//���ɳ���
    memcpy(array, starting_grid, ROW*COL*sizeof(int));
    cout << "��ʼ������������Ϊ��" << endl;
    print(starting_grid);//�������
    //ת��Ϊcnf�ļ�
    string filename = ToCnf(starting_grid,holes);
    return filename;
}

int Digit(int a[][COL], int i, int j) {//�ݹ��������Ԫ��
    if (i < ROW && j < COL) {
        int x,y,k;
        int check[COL+1]={CORRECT};//�����ų��Ѿ�ʹ�ù��ĵ�����
        for(x = 0 ; x < i ; x++)
            check[a[x][j]] = WRONG;//����ʹ�õ�������ΪWRONG
        for(x = 0 ; x < j ; x++)
            check[a[i][x]] = WRONG;//��ʹ�ù���������ΪWRONG
        for(x = i/3*3 ; x <= i; x++) {
            if(x == i)
                for(y = j/3*3 ; y < j; y++)
                    check[a[x][y]] = WRONG;
            else
                for(y = j/3*3 ; y < j/3*3 + 3; y++)
                    check[a[x][y]] = WRONG;
        }
        if(i+j==8)
        {
            for(x=0,y=8;x<9;x++,y--)
                check[a[x][y]]=WRONG;//б�Խ���ʹ�ù���������ΪWRONG
        }
        if(i>0&&j>0&&i<4&&j<4)
        {
            for(x=1;x<4;x++)
                for(y=1;y<4;y++)
                    check[a[x][y]]=WRONG;//���Ͻ�3*3����ʹ�õ�������ΪWRONG
        }
                if(i>4&&j>4&&i<8&&j<8)
        {
            for(x=5;x<8;x++)
                for(y=5;y<8;y++)
                    check[a[x][y]]=WRONG;//���½�3*3����ʹ�õ�������ΪWRONG
        }
        int flag = 0;
        for(k = 1; k <= COL && flag == 0 ; k++){//��check�����в��Ұ�ȫ������
            if(check[k] == CORRECT){
                flag = 1;
                a[i][j] = k;
                if(j == COL-1 && i != ROW-1){
                    if(Digit(a,i+1,0) == CORRECT) return CORRECT;
                    else flag = 0;
                }
                else if(j != COL-1){
                    if(Digit(a,i,j+1) == CORRECT) return CORRECT;
                    else flag = 0;
                }
            }
        }
        if( flag == 0 ) {
            a[i][j] = 0;
            return WRONG;
        }
    }
    return CORRECT;
}

void randomFirstRow(int a0[], int n) {//������ɵ�һ��
    int i,j;
    srand((unsigned)time(nullptr));
    for( i = 0 ; i < n ; i++){
        a0[i] = rand()%9 + 1;
        j = 0 ;
        while(j < i){
            if(a0[i] == a0[j]){
                a0[i] = rand()%9 + 1;
                j = 0;
            }
            else j++;
        }
    }
}

void createSudoku(int a[][COL]){ //��������
    randomFirstRow(a[0],COL);//������ɵ�һ��
    Digit(a,1,0);//�ݹ����ɺ�i��
}

void createStartinggrid(const int a[][COL], int b[][COL], int numDigits) {//������ɳ���
    int i,j,k;
    srand((unsigned)time(nullptr));
    for( i = 0; i < ROW ; i ++)
        for( j = 0; j < COL ; j++)
            b[i][j] = a[i][j];

    //int c[numDigits][2];//�˴����Բ���c++�е�new��̬Ϊ��ά��������ڴ�
    int** c = new int* [numDigits];
    for(int i = 0; i < numDigits; i++){
        c[i] = new int [2];
    }
    int m,flag = 0;
    
    int d[81]={0},d_count=0;
    for( i = 0; i < numDigits ; i++) {
        j = rand()%9;
        k = rand()%9;

        flag = 0;
        for(m = 0; m < i ; m++)
            if( j == c[m][0] && k == c[m][1])
                flag = 1;
        for(m=0;m<d_count;m++)
            if(j*9+k==d[m])
                flag=1;
        if(flag == 0){
            int tempp = b[j][k];
            int flag2=0;
            int book[1000]={0};
            book[tempp]=1;
            for(int num=1;num<=9;num++){
                if(tempp==num) continue;
                b[j][k]=num;
                string testCnf = ToCnf(b,81-i-1);
                HeadNode* L=CnfParser(testCnf);
                if(NewDPLL(L,book)){
                    flag2=1;
                    break;
                }
            }
            if(flag2==0){
            b[j][k] = 0;
            c[i][0] = j;
            c[i][1] = k;
            }
            else d[d_count++]=j*9+k;
        }
        else
            i--;
    }
    for(int i = 0; i < numDigits; i++){
    delete[] c[i];  // ���ͷ�ÿ�е��ڴ�
    }
    delete[] c;  // ���ͷ�ָ��������ڴ�
}

void print(const int a[][COL]){//��ӡ��������
    int i,j;
    for( i = 0 ; i < ROW ; i++){
        for( j = 0 ; j < COL ; j++)
            printf("%d ", a[i][j]);
        cout<<endl;
    }
}

string ToCnf(int a[][COL],int holes) {
    ofstream in (".\\sudoku.cnf");//���������ļ�
    if(!in.is_open())
        cout<<"can't open!\n";
    in<<"p"<<" "<<"cnf"<<" "<<729<<" "<<8829+81-holes<<" "<<endl;
    //single clause
    for (int x = 0; x < ROW; ++x) {
        for (int y = 0; y < COL; ++y)
            if(a[x][y] != 0)
                in<<(x+1)*100 + (y+1)*10 + a[x][y]<<" "<<0<<endl;
    }
    //entry
    for (int x = 1; x <= 9; ++x) {
        for (int y = 1; y <= 9; ++y) {
            for (int z = 1; z <= 9; ++z)
                in << x * 100 + y * 10 + z << " ";
            in<<0;
            in<<endl;
        }
    }
    //row
    for (int y = 1; y <= 9; ++y) {
        for (int z = 1; z <= 9; ++z)
            for (int x = 1; x <= 8; ++x)
                for (int i = x+1; i <= 9; ++i)
                    in<<0 - (x*100 + y*10 + z)<<" "
                      <<0 - (i*100 + y*10 + z)<<" "<<0<<endl;
    }
    //column
    for (int x = 1; x <= 9; ++x) {
        for (int z = 1; z <=9 ; ++z)
            for (int y = 1; y <= 8; ++y)
                for (int i = y+1; i <= 9; ++i)
                    in<<0-(x*100 + y*10 + z)<<" "
                      <<0-(x*100 + i*10 + z)<<" "<<0<<endl;
    }
    //3*3 sub-grids
    for (int z = 1; z <= 9 ; ++z) {
        for (int i = 0; i <=2 ; ++i)
            for (int j = 0; j <=2 ; ++j)
                for (int x = 1; x <= 3 ; ++x)
                    for (int y = 1; y <= 3; ++y)
                        for (int k = y+1; k <= 3; ++k)
                            in<<0 - ((3*i+x)*100 + (3*j+y)*10 + z)<<" "
                              <<0-((3*i+x)*100 + (3*j+k)*10 + z)<<" "<<0<<endl;
    }
    for (int z = 1; z <= 9; z++) {
        for (int i = 0; i <= 2; i++)
            for (int j = 0; j <= 2; j++)
                for (int x = 1; x <= 3; x++)
                    for (int y = 1; y <= 3; y++)
                        for (int k = x + 1; k <= 3; k++)
                            for (int l = 1; l <= 3; l++)
                                in << 0 - ((3*i+x)*100 + (3*j+y)*10 + z) << ' '
                                   << 0 - ((3*i+k)*100 + (3*j+l)*10 + z) << ' ' << 0 <<endl;
    }
    for(int z=1;z<=9;z++){
        for(int y=9;y>=1;y--)
            for(int i=y-1;i>=1;i--)
                in<<0-((10-y)*100+y*10+z)<<" "<<0-((10-i)*100+i*10+z)<<" "<<0<<endl;
    }//б�Խ���

    for(int z=1;z<=9;z++){
        for(int x=2;x<=4;x++)
            for(int y=2;y<=4;y++)
                for(int i=2;i<=4;i++)
                    for(int j=2;j<=4;j++)
                        if(!(x==i&&y==j))
                            in<<0-((x)*100+y*10+z)<<" "<<0-((i)*100+j*10+z)<<" "<<0<<endl;
    }//���Ͻ�3*3��
        for(int z=1;z<=9;z++){
            for(int x=6;x<=8;x++)
                for(int y=6;y<=8;y++)
                    for(int i=6;i<=8;i++)
                        for(int j=6;j<=8;j++)
                            if(!(x==i&&y==j))
                                in<<0-((x)*100+y*10+z)<<" "<<0-((i)*100+j*10+z)<<" "<<0<<endl;
    }//���½�3*3��
    in.close();
    return ".\\sudoku.cnf";//����һ��string���͵Ķ���
}