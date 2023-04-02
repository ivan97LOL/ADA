#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std ;

void usage() {
    cerr << "Usage:" << endl;
    cerr << " [-p] [-t] [--ignore-naive] -f file" << endl;
}


vector<vector<int>> matrix(string file_name, int &r, int &c){
    ifstream is ( file_name );
    if( ! is ) {
        cerr << " ERROR : can ’t open file ’" << file_name << " ’." << endl ;
        exit ( EXIT_FAILURE );
    }

    is >> r >> c ;
    vector < vector <int > > mat ( r , vector <int >( c ));

    for ( int i = 0; i < r ; i ++ ){
        for ( int j = 0; j < c ; j ++ ){
            is >> mat [ i ][ j ];
        }
    }
    
    return mat;
}


// Función que devuelve true si una casilla (i, j) está dentro de la matriz
bool inside_matrix(int i, int j, int n, int m) {
    return i >= 0 && i < n && j >= 0 && j < m;
}

// Función recursiva que calcula el camino más corto desde (i, j) hasta (n-1, m-1)
int naive_method(vector<vector<int>> matrix, int i, int j, int n, int m) {
    if (!inside_matrix(i, j, n, m) || matrix[i][j] == 0) {
        return -1; // no hay un camino válido desde la celda actual
    }

    if (i == n-1 && j == m-1) {
        return matrix[i][j];
    }
    int right = naive_method(matrix, i, j+1, n, m);
    int down = naive_method(matrix, i+1, j, n, m);
    int diagonal = naive_method(matrix, i+1, j+1, n, m);
    int shortest_path = -1; // valor especial para indicar que no hay un camino válido

    if (right != -1) {
        shortest_path = right;
    }
    if (down != -1) {
        if (shortest_path == -1 || down < shortest_path) {
            shortest_path = down;
        }
    }
    if (diagonal != -1) {
        if (shortest_path == -1 || diagonal < shortest_path) {
            shortest_path = diagonal;
        }
    }
    if (shortest_path == -1) {
        return -1; // no hay un camino válido desde la posición actual
    } else {
        return shortest_path + matrix[i][j];
    }
}

int memoized_method(vector<vector<int>>& matrix, vector<vector<int>>& memo, vector<vector<string>>& path, int i, int j, int n, int m) {

    if (!inside_matrix(i, j, n, m) || matrix[i][j] == 0) {
        return -1; // no hay un camino válido desde la celda actual
    }

    if(i == 0 && j == 0 && matrix[i][j] == 1){
        path[i][j] = "*"; // marca la primera posición con un asterisco
    }
    
    if (i == n - 1 && j == m - 1) {
        if (matrix[i][j] == 1) {
            path[i][j] = "*"; // marca la última posición con un asterisco
            return 1;
        }
        return -1;
    }


    if (memo[i][j] != -1) {
        return memo[i][j];
    }

    int right = -1, down = -1, diagonal = -1;

    if (j+1 < m) {
        if(memo[i][j+1] != -1){
            right = memo[i][j+1];
        }else{
            right = memoized_method(matrix, memo, path, i, j+1, n, m);
        }
    }

    if (i+1 < n) {
        if(memo[i+1][j] != -1){
            down = memo[i+1][j];
        }else{
            down = memoized_method(matrix, memo, path, i+1, j, n, m);
        }
    }

    if (i+1 < n && j+1 < m) {
        if(memo[i+1][j+1] != -1){
            diagonal = memo[i+1][j+1];
        }else{
            diagonal = memoized_method(matrix, memo, path, i+1, j+1, n, m);
        }
    }

    int res = -1;
    if (right != -1) {
        path[i][j+1] = "*";
        res = right;
    }

    if (down != -1) {
        if (res == -1 || down < res) {
            path[i+1][j] = "*";
            res = down;
        }
    }

    if (diagonal != -1) {
        if (res == -1 || diagonal < res) {
            path[i+1][j+1] = "*";
            res = diagonal;
        }
    }

    if (res != -1) {
        res++;
    }

    memo[i][j] = res;

    return res;
}

int memo_path(vector<vector<string>>& path, int n, int m){
    int len = 0;
    for(int i = 0; i<n; i++){
        for(int j = 1; j<m; j++){
            if(path[i][j] == "*"){
                len++;
            }
        }
    }

    return len;
}

void output(bool naive, bool p, bool t, vector<vector<int>> maze, int r, int c,vector<vector<int>>& memo, vector<vector<string>>& path){

    if(naive){
        cout<<"- ";
    }else{
        int shortest_path_naive = naive_method(maze,0,0,r,c);
    
        if(shortest_path_naive == -1){
            cout<<"0 ";
        }
        else{
            cout<< shortest_path_naive<< " ";
        }
    }
    int shortest_path_memo = memoized_method(maze,memo,path,0,0,r,c);

    if(shortest_path_memo == -1){
        cout<<"0 ";
    }else{
        cout<< shortest_path_memo<< " ";
    }
    cout<<"? ?"<<endl;
    cout<<"Memoization table: "<<endl;
    cout<<"?"<<endl;

    cout<<"Iterative table: "<<endl;
    cout<<"?"<<endl;

    if(p){
        int len = memo_path(path,r,c);

        if(len > 0){
            for(int i = 0; i<r; i++){
                for(int j = 0; j<c; j++){
                    if(path[i][j] == " "){
                        path[i][j] = to_string(maze[i][j]);
                    }else if(path[i][j] == "*"){
                        if(memo[i+1][j] > memo[i][j+1] || memo[i+1][j] > memo[i+1][j+1]){
                            path[i+1][j] = to_string(maze[i+1][j]);
                        }else if(memo[i][j+1] > memo[i+1][j] || memo[i][j+1] > memo[i+1][j+1]){
                            path[i][j+1] = to_string(maze[i][j+1]);
                        }else if(memo[i+1][j+1] > memo[i+1][j] || memo[i+1][j+1] > memo[i][j+1]){
                            path[i+1][j+1] = to_string(maze[i+1][j+1]);
                        }
                    }
                    cout<<path[i][j];
                }
                cout<<endl;
            }
        }else{
            cout<<"NO EXIT"<<endl;
        }
    }

    if(t){
        cout<<"?"<<endl;
    }
}

int main(int argc, char* argv[]) {
    string filename;
    bool t = false;
    bool p = false;
    bool ignore_naive = false;
    
    // Recorrer los argumentos de entrada
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-t") {
            t = true;

        } else if (arg == "-p") {
            p = true;

        } else if (arg == "--ignore-naive") {
            ignore_naive = true;

        } else if (arg == "-f") {
            // Verificar que el siguiente argumento no sea una opción
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                filename = argv[i + 1];
                i++;

            } else {
                cerr << "ERROR: missing filename." << endl;
                usage();
                return 1;
            }

        } else {
            cerr << "ERROR: unknown option " << arg << endl;
            usage();
            return 1;
        }
    }
    // Verificar que se especificó el fichero de entrada
    if (filename.empty()) {
        cerr << "ERROR: no file found." << endl;
        usage();
        return 1;
    }
    int rows, cols;
    vector<vector<int>> maze = matrix(filename, rows, cols);
    vector<vector<int>> memo(rows, vector<int>(cols, -1));
    vector<vector<string>> path(rows, vector<string>(cols, " "));


    output(ignore_naive,p,t,maze,rows,cols,memo, path);

    for(int i = 0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            cout<<memo[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}