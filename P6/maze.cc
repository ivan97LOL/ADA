//Iván Álvarez García DNI: 49623492A
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std ;

const int INF = 1e9; // valor de infinito

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
int maze_naive(vector<vector<int>> matrix, int i, int j, int n, int m) {
    if (!inside_matrix(i, j, n, m) || matrix[i][j] == 0) {
        return -1; // no hay un camino válido desde la celda actual
    }

    if (i == n-1 && j == m-1) {
        return matrix[i][j];
    }
    int right = maze_naive(matrix, i, j+1, n, m);
    int down = maze_naive(matrix, i+1, j, n, m);
    int diagonal = maze_naive(matrix, i+1, j+1, n, m);
    int shortest_path = -1; // valor especial para indicar que no hay un camino válido

    if (right != -1) {
        shortest_path = right;  //pasamos a la casilla de la derecha
    }
    if (down != -1) {
        if (shortest_path == -1 || down < shortest_path) {
            shortest_path = down;   //pasamos a la casilla de abajo
        }
    }
    if (diagonal != -1) {
        if (shortest_path == -1 || diagonal < shortest_path) {
            shortest_path = diagonal;   //pasamos a la casilla de la diagonal
        }
    }
    if (shortest_path == -1) {
        return -1; // no hay un camino válido desde la posición actual
    } else {
        return shortest_path + matrix[i][j];
    }
}

int maze_memo(const vector<vector<int>>& matrix, vector<vector<int>>& memo, int i, int j) {

    int n = matrix.size();
    int m = matrix[0].size();

    if (!inside_matrix(i, j, n, m) || matrix[i][j] == 0) {
        return INF; // no hay un camino válido desde la celda actual
    }
    
    if (i == n - 1 && j == m - 1) {
        return matrix[i][j];
    }


    if (memo[i][j] != -1) {
        return memo[i][j];
    }

    int right = maze_memo(matrix, memo, i, j+1); // calcular el camino más corto hacia la derecha
    int down = maze_memo(matrix, memo, i+1, j); // calcular el camino más corto hacia abajo
    int diagonal = maze_memo(matrix, memo, i+1, j+1); // calcular el camino más corto hacia abajo y a la derecha
    int shortest = min(right, min(down, diagonal)) + matrix[i][j]; // encontrar el camino más corto de los tres posibles y sumar el valor de la casilla actual

    memo[i][j] = shortest; // almacenar el resultado en la matriz de memoización
    return shortest;
}

int maze_it_matrix(const vector<vector<int>> &matrix) {
    int n = matrix.size();
    int m = matrix[0].size();

    vector<vector<int>> iterative(n, vector<int>(m, INF));
    iterative[0][0] = matrix[0][0];

    if(iterative[0][0] == 0){
        return 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] == 0){ // invalid cell
                continue;
            } 

            if (i > 0 && j > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i-1][j-1] + matrix[i][j]); // diagonal move
            }
            if (i > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i-1][j] + matrix[i][j]); // up move
            }
            if (j > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i][j-1] + matrix[i][j]); // left move
            }
        }
    }

    return iterative[n-1][m-1];

}


void maze_parser(vector<vector<string>>& path, const vector<vector<int>>& memo, const vector<vector<int>>& maze, int n, int m){

    for(int i = 0; i < n-1; i++){
        for(int j = 0; j < m-1; j++){
            path[i][j] = maze[i][j];
        }
    }
}

int memo_path(vector<vector<string>> path, int n, int m){
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

void output(bool naive, bool p, bool t, const vector<vector<int>>& maze, int r, int c,vector<vector<int>>& memo, vector<vector<string>>& path){

    if(naive){
        cout<<"- ";
    }else{
        int shortest_path_naive = maze_naive(maze,0,0,r,c); //camino más corto calculado por recursión sin almacenamiento
    
        if(shortest_path_naive == -1){
            cout<<"0 ";
        }
        else{
            cout<< shortest_path_naive<< " ";
        }
    }
    int shortest_path_memo = maze_memo(maze,memo,0,0); //camino más corto calculado por memoización

    if(shortest_path_memo >= INF){
        cout<<"0 ";
    }else{
        cout<< shortest_path_memo<< " ";
    }

    int shortest_path_iterative = maze_it_matrix(maze);

    if(shortest_path_iterative >= INF){
        cout<<"0 ";
    }else{
        cout<< shortest_path_iterative<<" ";
    }
    cout<<"?"<<endl;
    

    if(p){
        maze_parser(path,memo,maze,r,c);
        //int len = memo_path(path,r,c); //obtiene la longitud del camino

        //if(len > 2){
            for(int i = 0; i<r; i++){
                for(int j = 0; j<c; j++){
                    cout<<path[i][j];
                }
                cout<<endl;
            }
        //}else{
         //   cout<<"NO EXIT"<<endl;
        //}
    }

    if(t){
        cout<<"Memoization table: "<<endl;
        cout<<"?"<<endl;

        cout<<"Iterative table: "<<endl;
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

    return 0;
}