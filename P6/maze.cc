#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std ;

void usage() {
    cerr << "Usage:" << endl;
    cerr << " [-p] [-t] [--ignore-naive] -f file" << endl;
}

void memoization(){

}

vector<vector<int>> matrix(string file_name){
    ifstream is ( file_name );
    if( ! is ) {
        cerr << " ERROR : can ’t open file ’" << file_name << " ’." << endl ;
        exit ( EXIT_FAILURE );
    }

    int rows , cols ;
    is >> rows >> cols ;
    vector < vector <int > > mat ( rows , vector <int >( cols ));

    for ( int i = 0; i < rows ; i ++ ){
        for ( int j = 0; j < cols ; j ++ ){
            is >> mat [ i ][ j ];
        }
    }
    
    return mat;
}


// Función que devuelve true si una casilla (i, j) está dentro de la matriz
bool dentro_de_matriz(int i, int j, int n, int m) {
    return i >= 0 && i < n && j >= 0 && j < m;
}

// Función recursiva que calcula el camino más corto desde (i, j) hasta (n-1, m-1)
int camino_mas_corto(vector<vector<int>>& matriz, int i, int j, int n, int m) {
    // Si la casilla actual está fuera de la matriz o es inaccesible, devolvemos INF
    if (!dentro_de_matriz(i, j, n, m) || matriz[i][j] == 0) {
        cerr<<"Unaccesible"<<endl;
    }
    // Si hemos llegado a la casilla (n-1, m-1), devolvemos el valor de la casilla
    if (i == n-1 && j == m-1) {
        return matriz[i][j];
    }
    // Calculamos el camino más corto tomando el mínimo entre los caminos posibles
    int derecha = camino_mas_corto(matriz, i, j+1, n, m);
    int abajo = camino_mas_corto(matriz, i+1, j, n, m);
    int diagonal = camino_mas_corto(matriz, i+1, j+1, n, m);
    int camino_mas_corto = min(derecha, min(abajo, diagonal));
    // Si el camino más corto es INF, devolvemos INF, si no, sumamos el valor de la casilla actual
    return camino_mas_corto + matriz[i][j];
}

vector<vector<int>> naive( const vector<vector<int>> &m){
    vector<vector<int>> aux();
    
}

void output(bool naive, bool p, bool t){

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

    vector<vector<int>> maze = matrix(filename);



    return 0;
}