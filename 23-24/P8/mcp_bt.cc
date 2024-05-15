//Iván Álvarez García DNI: 49623492A
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <map>
using namespace std;

const int INF = INTMAX_MAX;
int visit, explored, leaf, unfeasible, not_promising;

void usage() {
    cerr << "Usage:" << endl;
    cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file" << endl;
}

vector<vector<int>> matrix(string file_name, int &r, int &c){
    ifstream is (file_name);
    if(!is){
        cerr << " ERROR : can ’t open file :" << file_name << "." << endl;
        exit (EXIT_FAILURE);
    }

    is >> r >> c ;
    vector <vector<int>> mat ( r, vector<int>(c));

    for(int i = 0; i<r ; i++){
        for(int j = 0; j<c ; j++){
            is >> mat [i][j];
        }
    }
    
    return mat;
}

// Función que devuelve true si una casilla (i, j) está dentro de la matriz
bool inside_matrix(int i, int j, int n, int m) {
    return i >= 0 && i < n && j >= 0 && j < m;
}

int mcp_it_matrix(const vector<vector<int>> &map, vector<vector<int>> &iterative) {
    int n = map.size();
    int m = map[0].size();

    iterative[0][0] = map[0][0];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i > 0 && j > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i-1][j-1] + map[i][j]); // movimiento diagonal
            }
            if (i > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i-1][j] + map[i][j]); // movimiento arriba
            }
            if (j > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i][j-1] + map[i][j]); // movimiento izquierda
            }   
        }
    }

    return iterative[n-1][m-1];

}

//Función backtracking
int mcp_bt(const vector<vector<int>> &maze, vector<vector<int>> &iterative, vector<vector<bool>> &visited, int x, int y){
    enum Step {SE, E, S, N, NE, SW, W, NW};
    map<Step, tuple<int, int>> steps_inc;

    steps_inc[N] = make_tuple(-1,0);
    steps_inc[NE] = make_tuple(-1,1);
    steps_inc[E] = make_tuple(0,1);
    steps_inc[SE] = make_tuple(1,1);
    steps_inc[S] = make_tuple(1,0);
    steps_inc[SW] = make_tuple(1,-1);
    steps_inc[W] = make_tuple(0,-1);
    steps_inc[NW] = make_tuple(-1,-1);

    int min_cost = iterative[x][y];

    for(auto it = steps_inc.begin(); it != steps_inc.end(); it++){
        int incx, incy;

        tie(incx, incy) = it->second;
        int newx = x + incx;
        int newy = y + incy;

        //check factible
        if(inside_matrix(newx,newy,maze.size(), maze[0].size()) && !visited[newx][newy]){
            visit++;
            visited[newx][newy] = true;
            int cost = maze[newx][newy] + mcp_bt(maze, iterative, visited, newx, newy);

            //check promising
            if(cost < min_cost){
                explored++;
                min_cost = cost;
                iterative[x][y] = min_cost;
            }
            visited[newx][newy] = false;
        }    
    }
    return iterative[x][y];
}

//Función que muestra la salida según parámetros
void output(bool p, bool p2D, vector<vector<int>> map, int r, int c){

    
    
    vector<vector<bool>> path(r, vector<bool>(c, false));
    vector<vector<int>> iterative(r, vector<int>(c, INF));
    int shortest_path_iterative = mcp_it_matrix(map, iterative);

    int bt = mcp_bt(map, iterative, path, 0, 0);

    cout<<bt<<endl;
    cout<<visit<<" "<<explored<<" "<<leaf<<" "<<unfeasible<<" "<<not_promising<<endl;

}

int main(int argc, char* argv[]) {
    string filename;
    bool p2D = false;
    bool p = false;
    
    // Recorrer los argumentos de entrada
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if(arg == "-p"){
            p = true;
        }
        else if (arg == "--p2D") {
            p2D = true;
        }
        else if (arg == "-f") {
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
            cerr << "ERROR: unknown option " << arg <<"."<< endl;
            usage();
            return 1;
        }
    }
    // Verificar que se especificó el fichero de entrada
    if (filename.empty()) {
        cerr << "ERROR: missing filename." << endl;
        usage();
        return 1;
    }
    int rows, cols;

    vector<vector<int>> map = matrix(filename, rows, cols);
    output(p,p2D,map,rows,cols);

    return 0;
}