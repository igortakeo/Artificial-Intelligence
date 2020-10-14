#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;

//Declaracoes das funcoes que serao usadas no codigo

void bfs(char **matrix, int row, int column, pii blue_block, pii red_block);
void Copy_Matrix(char **a, char **b, int row, int column);
void Read_Matrix(char **matrix, int row, int column);
char **Alloc_Matrix(int row, int column);
pii Get_Blue(char **matrix, int row, int column);
pii Get_Red(char **matrix, int row, int column);
void Free_Matrix(char **matrix, int row);
void Print_Matrix(char **matrix, int row, int column);


vector<pii>mov {{0,1},{0,-1},{1,0},{-1,0}};

void bfs(char **matrix, int row, int column, pii blue_block, pii red_block){

    clock_t start, end;

    start = clock();

    map<pii, pii>path;
    map<pii, bool>vis;

    queue<pii>q;
    q.push(blue_block);
    vis[blue_block] = true;

    while(!q.empty()){

        pii block = q.front();
        
        if(block == red_block) break;

        q.pop();

        for(auto a : mov){  
            pii next = make_pair(block.first+a.first, block.second+a.second);
            if(next.first == 0 or next.second == 0 or next.first == row+1 or next.second == column+1){ 
                continue;
            }

            if((matrix[next.first][next.second] == '*' or matrix[next.first][next.second] == '$') and !vis[next]){  
                q.push(next);
                path[next] = block;
                vis[next] = true;
            }
        }
        
    }

    char **matrix_answer = Alloc_Matrix(row, column);

    Copy_Matrix(matrix_answer, matrix, row, column);

    pii pred = red_block;
    vector<pii>SetPoints;
    SetPoints.push_back(pred);
    
    while(true){
        pred = path[pred];
        SetPoints.push_back(pred);    
        if(pred == blue_block) break;
        matrix_answer[pred.first][pred.second] = 'C';
    }   
    reverse(SetPoints.begin(), SetPoints.end());

    end = clock();

    double time = double(end - start) / double(CLOCKS_PER_SEC);

    cout << "Breadth-First-Search:" << endl;
    cout << "Caminho encontrado: ";
    for(auto a : SetPoints){
        cout << "(" << a.first << "," << a.second << ")" << ' ';
    }
    cout << endl;
    cout << "Tempo de Execucao: " << fixed << time << setprecision(9);
    cout << endl << endl;

    Print_Matrix(matrix_answer, row, column);

    Free_Matrix(matrix_answer, row);
}

void Copy_Matrix(char **a, char **b, int row, int column){
    
    for(int i=1; i<=row; i++){
       for(int j=1; j<=column; j++){
            a[i][j] = b[i][j];
        }
    }

}


void Read_Matrix(char **matrix, int row, int column){

    for(int i=1; i<=row; i++){
       for(int j=1; j<=column; j++){
            cin >> matrix[i][j];
        }
    }
    
}

char **Alloc_Matrix(int row, int column){

    char **matrix = (char**) malloc((row+1) * sizeof(char*));

    for(int i=1; i<=row; i++){
        matrix[i] = (char*) malloc((column+1) * sizeof(char));
    }

    return matrix;
}

pii Get_Blue(char **matrix, int row, int column){
    
    pii blue_block;

    for(int i=1; i<=row; i++){
        for(int j=1; j<=column; j++){
            if(matrix[i][j] == '#'){
                blue_block.first = i;
                blue_block.second = j;
                return blue_block;
            }
        }
    }

    return blue_block;
}


pii Get_Red(char **matrix, int row, int column){
    
    pii red_block;

    for(int i=1; i<=row; i++){
        for(int j=1; j<=column; j++){
            if(matrix[i][j] == '$'){
                red_block.first = i;
                red_block.second = j;
                return red_block;
            }
        }
    }

    return red_block;
}

void Free_Matrix(char **matrix, int row){

    for(int i=1; i<=row; i++) free(matrix[i]);
    free(matrix);

}

void Print_Matrix(char **matrix, int row, int column){
    
    for(int i=1; i<=row; i++){
       for(int j=1; j<=column; j++){
            cout << matrix[i][j];
        }
        cout << endl;
    }

}

int main(){

    int row, column;
    cin >> row >> column;

    char **matrix = Alloc_Matrix(row, column);

    Read_Matrix(matrix, row, column);

    cout << "Tabuleiro recebido como parametro:" << endl << endl;
    Print_Matrix(matrix, row, column);
    cout << endl << endl;
    
    pii blue_block = Get_Blue(matrix, row, column);
    pii red_block = Get_Red(matrix, row, column);
    
    bfs(matrix, row, column, blue_block, red_block);
    
    Free_Matrix(matrix, row);

    return 0;
}
