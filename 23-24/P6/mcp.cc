//Iván Álvarez García DNI: 49623492A
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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

int mcp_it_vector(vector<vector<int>> &map) {
    int n = map.size();
    int m = map[0].size();
    
    // Usamos solo dos vectores para almacenar los costes mínimos
    vector<int> prev(m, INF); // Vector para la iteración anterior
    vector<int> curr(m, INF); // Vector para la iteración actual
    
    // Inicialización del primer elemento
    prev[0] = map[0][0];
    
    // Iteración sobre las filas
    for (int i = 0; i < n; i++) {
        // Iteración sobre las columnas
        for (int j = 0; j < m; j++) {
            // Calculamos el costo mínimo para llegar a la casilla actual
            curr[j] = map[i][j];
            if (i > 0 && j > 0) {
                curr[j] += min({prev[j], prev[j - 1], curr[j - 1]});
            } else if (i > 0) {
                curr[j] += prev[j];
            } else if (j > 0) {
                curr[j] += curr[j - 1];
            }
        }
        // Actualizamos el vector previo para la siguiente iteración
        swap(prev, curr);
    }
    
    // Devolvemos el coste mínimo para llegar a la última casilla
    return prev[m - 1];
}

int mcp_parser(vector<vector<string>> &path, const vector<vector<int>> map, const vector<vector<int>> memo) {
    int n = path.size();
    int m = path[0].size();
    int path_size = 0;

    // Rellenamos la matriz con puntos
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            path[i][j] = ".";
        }
    }

    // Marcamos el punto de inicio y el de final
    path[0][0] = ".";
    path[n - 1][m - 1] = ".";

    // Marcamos el camino
    int i = n - 1;
    int j = m - 1;
    while (i != 0 || j != 0) {
        path[i][j] = "x";
        if (i > 0 && j > 0) {
            int left = memo[i][j - 1];
            int diagonal = memo[i - 1][j - 1];
            int up = memo[i - 1][j];
            if (diagonal <= left && diagonal <= up) {
                i--;
                j--;
            } else if (left <= diagonal && left <= up) {
                j--;
            } else {
                i--;
            }
        } else if (i > 0) {
            i--;
        } else {
            j--;
        }
    }
    path[0][0] = "x";  // Marcar el punto de inicio

    // Calcular el tamaño del camino
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(path[i][j] == "x") {
                path_size += map[i][j];
            }
        }
    }
    return path_size;
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
    
    vector<vector<string>> path(r, vector<string>(c));
    vector<vector<int>> memo(r, vector<int>(c, INF));
    int shortest_path_memo = mcp_memo(map,memo,r-1,c-1); //camino más corto calculado por memoización
     if(shortest_path_memo >= INF){
        cout<<"0 ";
    }else{
        cout<< shortest_path_memo<< " ";
    }
    vector<vector<int>> iterative(r, vector<int>(c, INF));
    int shortest_path_iterative = mcp_it_matrix(map, iterative);
    
    if(shortest_path_iterative >= INF){
        cout<<"0 ";
    }else{
        cout<< shortest_path_iterative<<" ";
    }

    int shortest_path_vector = mcp_it_vector(map);

    if(shortest_path_vector >= INF){
        cout<<"0"<<endl;
    }else{
        cout<< shortest_path_vector<<endl;
    }

    if(p){
        int mcp = mcp_parser(path, map, memo);
        for(int i = 0; i < r; i++){
            for(int j = 0; j < c; j++){
                cout<<path[i][j];
            }
            cout<< endl;
        }
        cout<<mcp<<endl;
    }

    if(t){
        for(int i = 0; i<r; i++){
            for(int j = 0; j<c; j++){
                cout<<iterative[i][j]<<" ";
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