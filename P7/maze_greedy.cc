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

int maze_greedy(vector<vector<int>>& maze){

    int n = maze.size();
    int m = maze[0].size();
    int length = 0;

    if(maze[0][0] == 0){
        return 0;
    }

    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){

            if(i == n-1 && j == m-1){
                maze[i][j] = -1;
                length++;
                break;
            }

            if(j == m-1){
                if(maze[i+1][j] == 0){
                    maze[i][j] = -1;
                    return 0;
                }

                maze[i][j] = -1;
                length++;
            }else if(i == n-1){
                if(maze[i][j+1] == 0){
                    maze[i][j] = -1;
                    return 0;
                }

                maze[i][j] = -1;
                length++;
            }else{

                if(maze[i+1][j+1] == 1){
                    length++;
                    maze[i][j] = -1;
                }else if(maze[i][j+1] == 1){
                    length++;
                    maze[i][j] = -1;
                }else if(maze[i+1][j] == 1){
                    length++;
                    maze[i][j] = -1;
                }else{
                    maze[i][j] = -1;
                    return 0;
                }
            }
        }
    }

    return length;
}

void output(bool p, vector<vector<int>>& maze){

    int shortest_path_naive = maze_greedy(maze); //camino más corto calculado por recursión sin almacenamiento
    
    cout<< shortest_path_naive<<endl;
    
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