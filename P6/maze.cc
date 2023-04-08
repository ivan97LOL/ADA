//Iván Álvarez García DNI: 49623492A
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>


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

int maze_memo(const vector<vector<int>>& matrix, vector<vector<int>>& memo, vector<vector<string>>& memo_table, int i, int j) {

    int n = matrix.size();
    int m = matrix[0].size();

    if (!inside_matrix(i, j, n, m)) {
        int memo_table_n = memo_table.size();
        int memo_table_m = memo_table[0].size();
        if (i >= 0 && i < memo_table_n && j >= 0 && j < memo_table_m) {
            memo_table[i][j] = "X";
        }
        return INF; // no hay un camino válido desde la celda actual
    }


    if(matrix[i][j] == 0){
        memo_table[i][j] = "X";
        return INF; //celda inválida
    }
    
    if (i == n - 1 && j == m - 1) {
        memo[i][j] = matrix[i][j];
        memo_table[i][j] = to_string(memo[i][j]);
        return memo[i][j];
    }

    if (memo[i][j] != -1) {
        memo_table[i][j] = to_string(memo[i][j]);
        return memo[i][j];
    }

    int right = maze_memo(matrix, memo, memo_table, i, j+1); // calcular el camino más corto hacia la derecha
    int down = maze_memo(matrix, memo, memo_table, i+1, j); // calcular el camino más corto hacia abajo
    int diagonal = maze_memo(matrix, memo, memo_table, i+1, j+1); // calcular el camino más corto hacia abajo y a la derecha
    int shortest = min(right, min(down, diagonal)) + matrix[i][j]; // encontrar el camino más corto de los tres posibles y sumar el valor de la casilla actual


    memo[i][j] = shortest; // almacenar el resultado en la matriz de memoización
    memo_table[i][j] = to_string(memo[i][j]); // actualizar la matriz de memoización con el valor de la solución encontrada
    return shortest;
}


int maze_it_matrix(const vector<vector<int>> &matrix, vector<vector<string>> &iterative_table) {
    int n = matrix.size();
    int m = matrix[0].size();

    vector<vector<int>> iterative(n, vector<int>(m, INF));
    iterative[0][0] = matrix[0][0];

    iterative_table = vector<vector<string>>(n, vector<string>(m, ""));

    if(iterative[0][0] == 0){
        iterative_table[0][0] = "X";
        return 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] == 0){ // celda inválida
                iterative_table[i][j] = "X";
                continue;
            } 

            if (i > 0 && j > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i-1][j-1] + matrix[i][j]); // movimiento diagonal
            }
            if (i > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i-1][j] + matrix[i][j]); // movimiento arriba
            }
            if (j > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i][j-1] + matrix[i][j]); // movimiento izquierda
            }

            if(iterative[i][j] == INF){
                iterative_table[i][j] = "X";
            }else{
                iterative_table[i][j] = to_string(iterative[i][j]);
            }    
        }
    }

    return iterative[n-1][m-1];

}


int maze_it_vector(vector<vector<int>> &matrix) {

    int n = matrix.size();
    int m = matrix[0].size();
    vector<int> v0(m + 1, INF);
    vector<int> v1(m + 1, INF);
    v0[1] = 0;
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (matrix[i - 1][j - 1] == 1) {
                v1[j] = min({v0[j - 1], v0[j], v1[j - 1]}) + 1;
            } else {
                v1[j] = INF;
            }
        }
        swap(v0, v1);
    }
    
    return v0[m];
}



void output(bool naive, bool p, bool t, vector<vector<int>> maze, int r, int c){

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
    vector<vector<string>> memo_table(r, vector<string>(c, "-"));
    vector<vector<int>> memo(r, vector<int>(c, -1));
    vector<vector<string>> path(r, vector<string>(c, " "));
    int shortest_path_memo = maze_memo(maze,memo,memo_table,0,0); //camino más corto calculado por memoización


    if(shortest_path_memo >= INF){
        cout<<"0 ";
    }else{
        cout<< shortest_path_memo<< " ";
    }
    vector<vector<string>> iterative_table;
    int shortest_path_iterative = maze_it_matrix(maze, iterative_table);

    if(shortest_path_iterative >= INF){
        cout<<"0 ";
    }else{
        cout<< shortest_path_iterative<<" ";
    }

    int shortest_path_vector = maze_it_vector(maze);

    if(shortest_path_vector >= INF){
        cout<<"0"<<endl;
    }else{
        cout<< shortest_path_vector<<endl;
    }
    

    if(p){
        cout<<"?"<<endl;
    }

    if(t){
/*        cout<<"Memoization table: "<<endl;
        for(int i = 0; i<r; i++){
            for(int j = 0; j<c; j++){
                cout<<memo_table[i][j]<<" ";
            }
            cout<<endl;
        }*/

        cout<<"Iterative table: "<<endl;
        for(int i = 0; i<r; i++){
            for(int j = 0; j<c; j++){
                cout<<iterative_table[i][j]<<" ";
            }
            cout<<endl;
        }
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
    output(ignore_naive,p,t,maze,rows,cols);

    return 0;
}