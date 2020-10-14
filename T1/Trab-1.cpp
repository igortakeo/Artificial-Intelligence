#include <bits/stdc++.h>
#define pii pair<int, int>
using namespace std;

//Declaracoes das funcoes que serao usadas no codigo

void solve_dfs(char **matrix, int row, int column, pii block, pii red_block, vector<pii>&path, map<pii, bool>&vis, vector<pii>&final_path);
void dfs(char **matrix, int row, int column, pii blue_block, pii red_block);
void bfs(char **matrix, int row, int column, pii blue_block, pii red_block);
void Copy_Matrix(char **a, char **b, int row, int column);
void Read_Matrix(char **matrix, int row, int column);
char **Alloc_Matrix(int row, int column);
pii Get_Blue(char **matrix, int row, int column);
pii Get_Red(char **matrix, int row, int column);
void Free_Matrix(char **matrix, int row);
void Print_Matrix(char **matrix, int row, int column);
void Print_Lines();

vector<pii>mov {{0,1},{0,-1},{1,0},{-1,0}};

void solve_dfs(char **matrix, int row, int column, pii block, pii red_block, vector<pii>&path, map<pii, bool>&vis, vector<pii>&final_path){
    vis[block] = true;
    path.push_back(block);
    if(block == red_block){
        final_path = path;
        return;
    }

    for(auto a : mov){
        pii next = make_pair(block.first+a.first, block.second+a.second);
        if(next.first == 0 or next.second == 0 or next.first == row+1 or next.second == column+1){ 
            continue;
        }
        if((matrix[next.first][next.second] == '*' or matrix[next.first][next.second] == '$') and !vis[next]){      
            solve_dfs(matrix, row, column, next, red_block, path, vis, final_path);
            path.pop_back();
        }

    }
}

void dfs(char **matrix, int row, int column, pii blue_block, pii red_block){
    
    vector<pii>path, final_path;
    map<pii, bool>vis;
    
    clock_t start, end;

    start = clock();
    
    solve_dfs(matrix, row, column, blue_block, red_block, path, vis, final_path);   
    
    end = clock();

    char **matrix_answer = Alloc_Matrix(row, column);

    Copy_Matrix(matrix_answer, matrix, row, column);


    for(auto a : final_path){
        if(a == blue_block or a == red_block) continue;
        matrix_answer[a.first][a.second] = 'C';
    }

    double time = double(end - start) / double(CLOCKS_PER_SEC);
    
    cout << "**Depth-First Search**" << endl;
    cout << "Caminho encontrado:" << endl;
    for(auto a : final_path){
        cout << "(" << a.first << "," << a.second << ")" << ' ';
    }
    cout << endl;
    cout << "Tempo de Execucao: " << time;
    cout << endl << endl;

    Print_Matrix(matrix_answer, row, column);
    cout << endl;
   
    Print_Lines();

    Free_Matrix(matrix_answer, row);
}

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

    end = clock();

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

    double time = double(end - start) / double(CLOCKS_PER_SEC);
    
    Print_Lines();
    cout << endl;

    cout << "**Breadth-First Search**" << endl;
    cout << "Caminho encontrado:" << endl;
    for(auto a : SetPoints){
        cout << "(" << a.first << "," << a.second << ")" << ' ';
    }
    cout << endl;
    cout << fixed << setprecision(6);
    cout << "Tempo de Execucao: " << time;
    cout << endl << endl;

    Print_Matrix(matrix_answer, row, column);
    cout << endl;
   
    Print_Lines();

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

void Print_Lines(){
    cout << "======================================================================================================================================================" << endl;
    cout << "======================================================================================================================================================" << endl;
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
    
    dfs(matrix, row, column, blue_block, red_block);

    Free_Matrix(matrix, row);

    return 0;
}
