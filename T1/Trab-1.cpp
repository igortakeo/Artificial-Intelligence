#include <bits/stdc++.h>
#define pii pair<int, int>
#define pdi pair<double, int>
using namespace std;

//Declaracoes das funcoes que serao usadas no codigo

double **Manhattan_Plus_Euclidean(char **matrix, int row, int column, pii blue_block, pii red_block);
double **Minkowski_Distance(char **matrix, int row, int column, pii blue_block, pii red_block, int p);
double **Euclidean_Distance(char **matrix, int row, int column, pii blue_block, pii red_block);
double **Manhattan_Distance(char **matrix, int row, int column, pii blue_block, pii red_block);
pdi hill_climbing(char **matrix, int row, int column, pii blue_block, pii red_block, int h);
pdi bestfs(char **matrix, int row, int column, pii blue_block, pii red_block, int h);
pdi A_star(char **matrix, int row, int column, pii blue_block, pii red_block, int h);
void solve_dfs(char **matrix, int row, int column, pii block, pii red_block, vector<pii>&path, map<pii, bool>&vis, vector<pii>&final_path);
pdi dfs(char **matrix, int row, int column, pii blue_block, pii red_block);
pdi bfs(char **matrix, int row, int column, pii blue_block, pii red_block);
void Copy_Matrix(char **a, char **b, int row, int column);
void Read_Matrix(char **matrix, int row, int column);
char **Alloc_Matrix(int row, int column);
pii Get_Blue(char **matrix, int row, int column);
pii Get_Red(char **matrix, int row, int column);
void Free_Matrix(char **matrix, int row);
void Print_Matrix(char **matrix, int row, int column);
void Print_Lines_One();
void Print_Lines();

vector<pii>mov {{0,-1}, {-1,0}, {0,1}, {1,0}};

double **Manhattan_Plus_Euclidean(char **matrix, int row, int column, pii blue_block, pii red_block){

    double **matrix_dist = (double**) malloc((row+1) * sizeof(double*));

    for(int i=1; i<=row; i++){
        matrix_dist[i] = (double*) malloc((column+1) * sizeof(double));
    }

    for(int i=1; i<=row; i++){
       for(int j=1; j<=column; j++){
            matrix_dist[i][j] = abs(red_block.first-i) + abs(red_block.second-j);
            matrix_dist[i][j] += sqrt((double)(red_block.first-i)*(double)(red_block.first-i)+(double)(red_block.second-j)*(double)(red_block.second-j));
        }
    }
    return matrix_dist;
}

double **Minkowski_Distance(char **matrix, int row, int column, pii blue_block, pii red_block, int p){

    double **matrix_dist = (double**) malloc((row+1) * sizeof(double*));

    for(int i=1; i<=row; i++){
        matrix_dist[i] = (double*) malloc((column+1) * sizeof(double));
    }

    for(int i=1; i<=row; i++){
        for(int j=1; j<=column; j++){
            matrix_dist[i][j] = pow(pow((double)abs(red_block.first-i),p)+pow((double)abs(red_block.second-j),p), 1/(double)p);
        }
    }

    return matrix_dist;
}

double **Euclidean_Distance(char **matrix, int row, int column, pii blue_block, pii red_block){
    
    double **matrix_dist = (double**) malloc((row+1) * sizeof(double*));

    for(int i=1; i<=row; i++){
        matrix_dist[i] = (double*) malloc((column+1) * sizeof(double));
    }

    for(int i=1; i<=row; i++){
        for(int j=1; j<=column; j++){
            matrix_dist[i][j] = sqrt((double)(red_block.first-i)*(double)(red_block.first-i)+(double)(red_block.second-j)*(double)(red_block.second-j));
        }
    }

    return matrix_dist;

}

double **Manhattan_Distance(char **matrix, int row, int column, pii blue_block, pii red_block){

    double **matrix_dist = (double**) malloc((row+1) * sizeof(double*));

    for(int i=1; i<=row; i++){
        matrix_dist[i] = (double*) malloc((column+1) * sizeof(double));
    }

    for(int i=1; i<=row; i++){
       for(int j=1; j<=column; j++){
            matrix_dist[i][j] = abs(red_block.first-i) + abs(red_block.second-j);
        }
    }
    return matrix_dist;
}

pdi hill_climbing(char **matrix, int row, int column, pii blue_block, pii red_block, int h){
    
    pdi report;
    report.second = 1;

    clock_t start, end;

    start = clock();

    double **matrix_dist;
    if(h == 0) 
        matrix_dist = Manhattan_Distance(matrix, row, column, blue_block, red_block);
    else if(h == 1) 
        matrix_dist = Euclidean_Distance(matrix, row, column, blue_block, red_block);
    else if(h == 2)
        matrix_dist = Minkowski_Distance(matrix, row, column, blue_block, red_block, 3);
    else if(h == 3)
        matrix_dist = Minkowski_Distance(matrix, row, column, blue_block, red_block, 4); 
    else if(h == 4)
        matrix_dist = Manhattan_Plus_Euclidean(matrix, row, column, blue_block, red_block);

    map<pii, pii>path;
    map<pii, bool>vis;

    pii last;
    queue<pii>q;
    q.push(blue_block);
    vis[blue_block] = true;

    while(!q.empty()){

        pii block = q.front();

        last = block;

        if(block == red_block) break;

        q.pop();

        pii n;
        double weight = -1;

        for(auto a : mov){  
            pii next = make_pair(block.first+a.first, block.second+a.second);
            if(next.first == 0 or next.second == 0 or next.first == row+1 or next.second == column+1){ 
                continue;
            }

            if(weight == -1 and (matrix[next.first][next.second] == '*' or matrix[next.first][next.second] == '$') and !vis[next]){
                n = next;
                weight = matrix_dist[next.first][next.second];
                continue;
            }

            if((matrix[next.first][next.second] == '*' or matrix[next.first][next.second] == '$') and !vis[next] and weight > matrix_dist[next.first][next.second]){  
                weight = matrix_dist[next.first][next.second];
                n = next;   
            }
        }

        if(weight == -1) continue;

        q.push(n);
        path[n] = block;
        vis[n] = true;

    }

    end = clock();

    char **matrix_answer = Alloc_Matrix(row, column);

    Copy_Matrix(matrix_answer, matrix, row, column);

    pii pred = last;
    vector<pii>SetPoints;
    SetPoints.push_back(pred);
    
    if(matrix_answer[pred.first][pred.second] != '$' and matrix_answer[pred.first][pred.second] != '#')
         matrix_answer[pred.first][pred.second] = 'C';
   
    while(true){
        pred = path[pred];
        if(pred == make_pair(0,0)) break;
        SetPoints.push_back(pred);    
        if(pred == blue_block) break;
        matrix_answer[pred.first][pred.second] = 'C';
    }   

    reverse(SetPoints.begin(), SetPoints.end());

    double time = double(end - start) / double(CLOCKS_PER_SEC);
    
    if(last != red_block){ 
        cout << endl <<  "**CAMINHO NAO ENCONTRADO**" << endl << endl;
        report.second = 0;
    }

    report.first = time;

    int cnt = 0;
     if(h == 0) 
        cout << "Heuristica --> Distancia Manhattan" << endl << endl;
    else if(h == 1)
        cout << "Heuristica --> Distancia Euclidiana" << endl << endl;
    else if(h == 2)
        cout << "Heuristica --> Distancia Minkowski com p = 3" << endl;
    else if(h == 3)
        cout << "Heuristica --> Distancia Minkowski com p = 4" << endl;
    else if(h == 4)
        cout << "Heuristica --> Distancia Manhattan mais Euclidiana" << endl;
    
    
    cout << "Caminho percorrido:" << endl;
    for(auto a : SetPoints){
        if(cnt%4 == 0) cout << endl;
        cout << "[" << cnt+1 << "]" << "-->" << "(" << a.first << "," << a.second << ")" << "  ";
        cnt++;
    }
    cout << endl << endl;
    
    cout << "Representacao no tabuleiro:" << endl << endl;

    Print_Matrix(matrix_answer, row, column);
    cout << endl;
   
    cout << "Tempo de execucao: " << time;
    cout << endl << endl;

    Free_Matrix(matrix_answer, row);

    for(int i=1; i<=row; i++){
        free(matrix_dist[i]);
    }
    free(matrix_dist);

    return report;
}


pdi bestfs(char **matrix, int row, int column, pii blue_block, pii red_block, int h){
    pdi report;
    report.second = 1;

    clock_t start, end;

    start = clock();

    double **matrix_dist;
    if(h == 0) 
        matrix_dist = Manhattan_Distance(matrix, row, column, blue_block, red_block);
    else if(h == 1) 
        matrix_dist = Euclidean_Distance(matrix, row, column, blue_block, red_block);
    else if(h == 2)
        matrix_dist = Minkowski_Distance(matrix, row, column, blue_block, red_block, 3);
    else if(h == 3)
        matrix_dist = Minkowski_Distance(matrix, row, column, blue_block, red_block, 4); 
    else if(h == 4)
        matrix_dist = Manhattan_Plus_Euclidean(matrix, row, column, blue_block, red_block);


    map<pii, pii>path;
    map<pii, bool>vis;
    priority_queue<pair<double, pii>>pq;

    vis[blue_block] = true;
    pq.push({-matrix_dist[blue_block.first][blue_block.second], blue_block});
    pii last;

    while(!pq.empty()){

        pii block = pq.top().second;
        last = block;

        if(block == red_block) break;

        pq.pop();

        for(auto a : mov){  
            pii next = make_pair(block.first+a.first, block.second+a.second);
            if(next.first == 0 or next.second == 0 or next.first == row+1 or next.second == column+1){ 
                continue;
            }
            if((matrix[next.first][next.second] == '*' or matrix[next.first][next.second] == '$') and !vis[next]){  
                pq.push({-matrix_dist[next.first][next.second],next});
                vis[next] = true;
                path[next] = block;
            }
        }
        
    }

    end = clock();

    char **matrix_answer = Alloc_Matrix(row, column);

    Copy_Matrix(matrix_answer, matrix, row, column);

    pii pred = last;
    vector<pii>SetPoints;
    SetPoints.push_back(pred);
    
    if(matrix_answer[pred.first][pred.second] != '$' and matrix_answer[pred.first][pred.second] != '#')
         matrix_answer[pred.first][pred.second] = 'C';

    while(true){
        pred = path[pred];
        if(pred == make_pair(0,0)) break;
        SetPoints.push_back(pred);    
        if(pred == blue_block) break;
        matrix_answer[pred.first][pred.second] = 'C';
    }   
    reverse(SetPoints.begin(), SetPoints.end());

    double time = double(end - start) / double(CLOCKS_PER_SEC);

    if(last != red_block){ 
        cout << endl <<  "**CAMINHO NAO ENCONTRADO**" << endl << endl;
        report.second = 0;
    }

    report.first = time;

    int cnt = 0;
    if(h == 0) 
        cout << "Heuristica --> Distancia Manhattan" << endl << endl;
    else if(h == 1)
        cout << "Heuristica --> Distancia Euclidiana" << endl << endl;
    else if(h == 2)
        cout << "Heuristica --> Distancia Minkowski com p = 3" << endl;
    else if(h == 3)
        cout << "Heuristica --> Distancia Minkowski com p = 4" << endl;
    else if(h == 4)
        cout << "Heuristica --> Distancia Manhattan mais Euclidiana" << endl;
    
    
    cout << "Caminho percorrido:" << endl;
    for(auto a : SetPoints){
        if(cnt%4 == 0) cout << endl;
        cout << "[" << cnt+1 << "]" << "-->" << "(" << a.first << "," << a.second << ")" << "  ";
        cnt++;
    }
    cout << endl << endl;
    
    cout << "Representacao no tabuleiro:" << endl << endl;

    Print_Matrix(matrix_answer, row, column);
    cout << endl;
   
    cout << "Tempo de execucao: " << time;
    cout << endl << endl;

    Free_Matrix(matrix_answer, row);

    for(int i=1; i<=row; i++){
        free(matrix_dist[i]);
    }
    free(matrix_dist);

    return report;
}

pdi A_star(char **matrix, int row, int column, pii blue_block, pii red_block, int h){
    pdi report;
    report.second = 1;

    clock_t start, end;

    start = clock();

    double **matrix_dist;
    if(h == 0) 
        matrix_dist = Manhattan_Distance(matrix, row, column, blue_block, red_block);
    else if(h == 1)
        matrix_dist = Euclidean_Distance(matrix, row, column, blue_block, red_block);
    else if(h == 2)
        matrix_dist = Minkowski_Distance(matrix, row, column, blue_block, red_block, 3);
    else if(h == 3)
        matrix_dist = Minkowski_Distance(matrix, row, column, blue_block, red_block, 4); 
    else if(h == 4)
        matrix_dist = Manhattan_Plus_Euclidean(matrix, row, column, blue_block, red_block);

    map<pii, pii>path;
    map<pii, bool>vis;
    priority_queue<pair<pdi, pii>>pq;
    pii last;

    vis[blue_block] = true;
    pq.push({{-matrix_dist[blue_block.first][blue_block.second],0}, blue_block});

    while(!pq.empty()){

        pii block = pq.top().second;
        int depth = pq.top().first.second;

        last = block;
        if(block == red_block) break;

        pq.pop();

        for(auto a : mov){  
            pii next = make_pair(block.first+a.first, block.second+a.second);
            if(next.first == 0 or next.second == 0 or next.first == row+1 or next.second == column+1){ 
                continue;
            }
            if((matrix[next.first][next.second] == '*' or matrix[next.first][next.second] == '$') and !vis[next]){  
                pq.push({{-(matrix_dist[next.first][next.second]+depth+1),depth+1},next});
                vis[next] = true;
                path[next] = block;
            }
        }
        
    }

    end = clock();

    char **matrix_answer = Alloc_Matrix(row, column);

    Copy_Matrix(matrix_answer, matrix, row, column);

    pii pred = last;
    vector<pii>SetPoints;
    SetPoints.push_back(pred);
            
    if(matrix_answer[pred.first][pred.second] != '$' and matrix_answer[pred.first][pred.second] != '#')
         matrix_answer[pred.first][pred.second] = 'C';

    while(true){
        pred = path[pred];
        if(pred == make_pair(0,0)) break;
        SetPoints.push_back(pred);    
        if(pred == blue_block) break;
        matrix_answer[pred.first][pred.second] = 'C';
    }   

    reverse(SetPoints.begin(), SetPoints.end());

    double time = double(end - start) / double(CLOCKS_PER_SEC);

    if(last != red_block){ 
        cout << endl <<  "**CAMINHO NAO ENCONTRADO**" << endl << endl;
        report.second = 0;
    }

    report.first = time;

    int cnt = 0;
    if(h == 0) 
        cout << "Heuristica --> Distancia Manhattan" << endl << endl;
    else if(h == 1)
        cout << "Heuristica --> Distancia Euclidiana" << endl << endl;
    else if(h == 2)
        cout << "Heuristica --> Distancia Minkowski com p = 3" << endl;
    else if(h == 3)
        cout << "Heuristica --> Distancia Minkowski com p = 4" << endl;
    else if(h == 4)
        cout << "Heuristica --> Distancia Manhattan mais Euclidiana" << endl;
    
    cout << "Caminho percorrido: " << endl;
    for(auto a : SetPoints){
        if(cnt%4 == 0) cout << endl;
        cout << "[" << cnt+1 << "]" << "-->" << "(" << a.first << "," << a.second << ")" << "  ";
        cnt++;
    }
    cout << endl << endl;
    
    cout << "Representacao no tabuleiro:" << endl << endl;

    Print_Matrix(matrix_answer, row, column);
    cout << endl;
   
    cout << "Tempo de execucao: " << time;
    cout << endl << endl;

    Free_Matrix(matrix_answer, row);

    for(int i=1; i<=row; i++){
        free(matrix_dist[i]);
    }
    free(matrix_dist);

    return report;
}


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

pdi dfs(char **matrix, int row, int column, pii blue_block, pii red_block){
    
    pdi report;
    report.second = 1;
    
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
    int cnt = 0;

    if(final_path.size() == 0) {
        cout << endl <<  "**CAMINHO NAO ENCONTRADO**" << endl << endl;
        report.second = 0;
    }

    report.first = time;

    cout << "** Depth-First Search **" << endl << endl;
    cout << "Caminho percorrido:" << endl;
    for(auto a : final_path){
        if(cnt%4 == 0) cout << endl;
        cout << "[" << cnt+1 << "]" << "-->" << "(" << a.first << "," << a.second << ")" << "  ";
        cnt++;
    }
    cout << endl << endl;

    cout << "Representacao no tabuleiro:" << endl << endl; 

    Print_Matrix(matrix_answer, row, column);
    cout << endl;

    cout << "Tempo de execucao: " << time;
    cout << endl << endl;

    Print_Lines();

    Free_Matrix(matrix_answer, row);

    return report;
}

pdi bfs(char **matrix, int row, int column, pii blue_block, pii red_block){
    pdi report;
    report.second = 1;

    clock_t start, end;

    start = clock();

    map<pii, pii>path;
    map<pii, bool>vis;

    queue<pii>q;
    q.push(blue_block);
    pii last;
    vis[blue_block] = true;

    while(!q.empty()){

        pii block = q.front();
        last = block;
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

    pii pred = last;
    vector<pii>SetPoints;
    SetPoints.push_back(pred);    
        
    if(matrix_answer[pred.first][pred.second] != '$' and matrix_answer[pred.first][pred.second] != '#')
         matrix_answer[pred.first][pred.second] = 'C';

    while(true){
        pred = path[pred];
        if(pred == make_pair(0,0)) break;
        SetPoints.push_back(pred);    
        if(pred == blue_block) break;
        matrix_answer[pred.first][pred.second] = 'C';
    }   
    reverse(SetPoints.begin(), SetPoints.end());

    double time = double(end - start) / double(CLOCKS_PER_SEC);
    
    Print_Lines();

    if(last != red_block){ 
        cout << endl <<  "**CAMINHO NAO ENCONTRADO**" << endl << endl;
        report.second = 0;
    }

    report.first = time;

    int cnt = 0;
    cout << "** Breadth-First Search **" << endl << endl;
    cout << "Caminho percorrido:" << endl;
    for(auto a : SetPoints){
        if(cnt%4 == 0) cout << endl;
        cout << "[" << cnt+1 << "]" << "-->" << "(" << a.first << "," << a.second << ")" << "  ";
        cnt++;
    }
    cout << endl << endl;
    
    cout << "Representacao no tabuleiro:" << endl << endl;

    Print_Matrix(matrix_answer, row, column);
    cout << endl;
   
    cout << "Tempo de execucao: " << time;
    cout << endl << endl;

    Print_Lines();

    Free_Matrix(matrix_answer, row);

    return report;
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

void Print_Lines_One(){
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}

void Print_Lines(){
    cout << "======================================================================================================================================================" << endl;
    cout << "======================================================================================================================================================" << endl;
}

int main(){

    cout << fixed << setprecision(6);

    int row, column;

    cin >> row >> column;

    char **matrix = Alloc_Matrix(row, column);

    vector<pdi>final_report[3];
    int j=0;

    Read_Matrix(matrix, row, column);

    pii blue_block = Get_Blue(matrix, row, column);
    pii red_block = Get_Red(matrix, row, column);

    cout << "Tabuleiro recebido como parametro:" << endl << endl;
    cout << "Ponto inicial: " << "(" << blue_block.first << "," << blue_block.second << ")" << endl;
    cout << "Ponto final: " << "(" << red_block.first << "," << red_block.second << ")" << endl;
    cout << endl;
    Print_Matrix(matrix, row, column);
    cout << endl << endl;
    
    pdi final_report_bfs = bfs(matrix, row, column, blue_block, red_block);
    
    pdi final_report_dfs = dfs(matrix, row, column, blue_block, red_block);
    
    cout <<"**(A* Search)**" << endl;
    for(int i=0; i<5; i++){
        Print_Lines_One();
        pdi x = A_star(matrix, row, column, blue_block, red_block, i);
        final_report[j].push_back(x);
    }
    Print_Lines();
    j++;
    
    cout << "**(Best First Search)**" << endl;
    for(int i=0; i<5; i++){
        Print_Lines_One();
        pdi x = bestfs(matrix, row, column, blue_block, red_block, i);
        final_report[j].push_back(x);
    }
    Print_Lines();
    j++;
    
    cout << "**(Hill Climbing)**" << endl;
    for(int i=0; i<5; i++){
        Print_Lines_One();
        pdi x = hill_climbing(matrix, row, column, blue_block, red_block, i);
        final_report[j].push_back(x);
    }
    Print_Lines();
    
    cout << endl << "**(Relatório Final)**" << endl << endl;

    if(final_report_bfs.second)
        cout << "BFS: " << final_report_bfs.first << endl;
    else cout << "BFS: " << final_report_bfs.first << ' ' << "(**CAMINHO NAO ENCONTRADO**)" << endl;

    if(final_report_dfs.second)
        cout << "DFS: " << final_report_dfs.first << endl;
    else cout << "DFS: " << final_report_dfs.first << ' ' << "(**CAMINHO NAO ENCONTRADO**)" << endl;

    for(int i=0; i<3; i++){
        string s;
        if(i == 0) s = "A* Search";
        else if(i == 1) s = "Best First Search";
        else if(i == 2) s = "Hill Climbing";
        for(int j=0; j<5; j++){
            string p;
            if(j == 0) p = " (Heuristica --> Distancia Manhattan): ";
            else if (j == 1) p = " (Heuristica --> Distancia Euclidiana): ";
            else if (j == 2) p = " (Heuristica --> Distancia de Minkowski p = 3): ";
            else if (j == 3) p = " (Heuristica --> Distancia de Minkowski p = 4): ";
            else if (j == 4) p = " (Heuristica --> Distancia Manhattan mais Euclidiana): ";

            cout << s+p;
            string e = "";

            if(!final_report[i][j].second) e = "(**CAMINHO NAO ENCONTRADO**)";

            cout << final_report[i][j].first << ' ' << e << endl;
        }
    }
    
    cout << endl;
    
    Free_Matrix(matrix, row);

    return 0;
}