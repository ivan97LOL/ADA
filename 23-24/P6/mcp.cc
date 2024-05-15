//Iván Álvarez García DNI: 49623492A
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int INF = 1e9; // valor de infinito

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

// Función recursiva que calcula el camino más corto desde (i, j) hasta (n-1, m-1)
int mcp_naive(vector<vector<int>> map, int i, int j, int n, int m) {
    if(!inside_matrix(i,j,n,m)){
        return INF; //se sale del mapa
    }

    if(i == 0 && j == 0){
        return map[i][j];  //llega al final
    }

    int up = mcp_naive(map, i-1, j, n, m); //va hacia arriba
    int left = mcp_naive(map, i, j-1, n, m); //va hacia la izquierda
    int diagonal = mcp_naive(map, i-1, j-1, n, m); //va hacia la diagonal
    int shortest_path = INF;

    if(left != INF){
        shortest_path = left;
    }

    if(up != INF){
        if(shortest_path == INF || up<shortest_path){
            shortest_path = up;
        }
    }

    if(diagonal != INF){
        if(shortest_path == INF || diagonal < shortest_path){
            shortest_path = diagonal;
        }
    }

    if(shortest_path == INF){
        return INF;
    }
    else{
        return shortest_path + map[i][j];
    }

}

int mcp_memo(const vector<vector<int>>& map, vector<vector<int>>& memo, int i, int j) {

    int n = map.size();
    int m = map[0].size();

    if(!inside_matrix(i,j,n,m)){
        return INF; //se sale del mapa
    }
    
    if (i == 0 && j == 0) {
        memo[i][j] = map[i][j];
        return memo[i][j];
    }

    if (memo[i][j] != INF) {
        return memo[i][j];
    }

    int left = mcp_memo(map, memo, i, j-1); // calcular el camino más corto hacia la izquierda
    int up = mcp_memo(map, memo, i-1, j); // calcular el camino más corto hacia arriba
    int diagonal = mcp_memo(map, memo, i-1, j-1); // calcular el camino más corto hacia la diagonal
    int shortest_path = min(left, min(up, diagonal)) + map[i][j]; // encontrar el camino más corto de los tres posibles y sumar el valor de la casilla actual


    memo[i][j] = shortest_path; // almacenar el resultado en la matriz de memoización
    return shortest_path;
}


//Función que muestra la salida según parámetros
void output(bool naive, bool p, bool t, vector<vector<int>> map, int r, int c){

    if(naive){
        cout<<"- ";
    }else{
        int shortest_path_naive = mcp_naive(map,r-1,c-1,r,c); //camino más corto calculado por recursión sin almacenamiento
    
        if(shortest_path_naive == INF){
            cout<<"0 ";
        }
        else{
            cout<< shortest_path_naive<< " ";
        }
    }
    vector<vector<int>> memo(r, vector<int>(c, INF));
    int shortest_path_memo = mcp_memo(map,memo,r-1,c-1); //camino más corto calculado por memoización
     if(shortest_path_memo >= INF){
        cout<<"0 ";
    }else{
        cout<< shortest_path_memo<< " ";
    }
    cout<<"? ?"<<endl;
    if(p){
        cout<<"?"<<endl;
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

        } else if (arg == "--p2D") {
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
    output(ignore_naive,p,t,map,rows,cols);

    return 0;
}