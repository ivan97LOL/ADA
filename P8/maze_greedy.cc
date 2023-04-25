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

vector<vector<int>> matrix(string file_name){

    int r, c;
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

int maze_bt(const vector<vector<int>>& maze, vector<vector<int>>& path, int i, int j){

    int n = maze.size();
    int m = maze[0].size();
    int length = 0;

    if(maze[0][0] == 0){
        return 0;
    }else{
        length++;
        path[0][0] = -1;
    }

    while(i < n && j < m){

        if(i == n-1 && j == m-1){ //termina el bucle
            maze[i][j] = -1;
            break;
        }
        
        if(inside_matrix(i+1,j+1,n,m) && maze[i+1][j+1] == 1){ //va en diagonal
            maze[i+1][j+1] = -1;
            length++;
            i++;
            j++;

        }else if(inside_matrix(i+1,j,n,m) && maze[i+1][j] == 1){ //va hacia abajo
            maze[i+1][j] = -1;
            length++;
            i++;

        }else if(inside_matrix(i,j+1,n,m) && maze[i][j+1] == 1){ //va hacia la derecha
            maze[i][j+1] = -1;
            length++;
            j++;

        }else{
            return 0;
        }
    }

    return length;
}

void output(bool p, vector<vector<int>>& maze){

    int shortest_path_bt = maze_bt(maze,0,0); //camino más corto calculado por backtracking
    
    cout<< shortest_path_bt <<endl;
    
    if(p){
        
        for(unsigned i = 0; i<maze.size(); i++){
            for(unsigned j = 0; j<maze[0].size(); j++){
                if(maze[i][j] == -1){
                    cout << '*';
                }else{
                    cout << maze[i][j];
                }
            }
            cout << endl;
        }
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

    vector<vector<int>> maze = matrix(filename);
    output(p,maze);

    return 0;
}