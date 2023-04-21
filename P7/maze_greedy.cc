//Iván Álvarez García DNI: 49623492A
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


int maze_greedy(vector<vector<int>>& maze, int n, int m){

    int length = 0;

    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){

            if(inside_matrix(i+1, j+1, n, m) && maze[i+1][j+1] == 1){
                length++;
                maze[i+1][j+1] = -1;
            }else if(inside_matrix(i,j+1,n,m) && maze[i][j+1] == 1){
                length++;
                maze[i][j+1] = -1;
            }else if(inside_matrix(i+1,j,n,m) && maze[i+1][j] == 1){
                length++;
                maze[i+1][j];
            }else{
                break;
            }
        }
    }

    return length;
}

void output(bool p, vector<vector<int>>& maze, int r, int c){

    int shortest_path_naive = maze_greedy(maze,r,c); //camino más corto calculado por recursión sin almacenamiento
    
    cout<< shortest_path_naive<<endl;
    
    if(p){
        
        
    }
}

int main(int argc, char* argv[]) {
    string filename;
    bool p = false;
    
    // Recorrer los argumentos de entrada
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-p") {
            p = true;

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

    output(p,maze,rows,cols);

    return 0;
}