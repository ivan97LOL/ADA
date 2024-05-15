//Javier Ortega García 48775597A

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

using namespace std;

vector< vector<string>> almacenParser;
void mcp_greedy_parser(int rows, int cols, int min){

    for (int r=0; r<=rows; r++){
        for(int c=0; c<=cols; c++)
            cout<<almacenParser[r][c];
        cout<<"\n";
    }

    cout<<min<<endl;

}

int mcp_greedy(vector< vector<int>> matriz, int rows, int cols, bool origen){
    
    
    if(origen){
        //DESDE EL ORIGEN HASTA EL FINAL
        int sol=matriz[0][0], aux, r=0, c=0;
        almacenParser[0][0] = "x";

        while(r<=rows && c<=cols){

            if(c==cols && r==rows) break;   //Última posición
            
            if(c==cols){ 
                sol+= matriz[r+1][c];   //Última columna
                almacenParser[r+1][c] = "x";
                r++;
            }
            else if(r==rows){ 
                sol+= matriz[r][c+1];  //Última fila
                almacenParser[r][c+1] = "x";
                c++;
            }
            else{    //El mínimo de los tres lados
                aux = min(matriz[r+1][c+1], min(
                        matriz[r+1][c],
                        matriz[r][c+1]
                    )
                );
                sol += aux;

                if(aux==matriz[r+1][c+1]){
                    almacenParser[r+1][c+1] = "x";
                    r++; c++;
                }
                else if(aux==matriz[r+1][c]){
                    almacenParser[r+1][c] = "x";
                    r++;
                }
                else{
                    almacenParser[r][c+1] = "x";
                    c++;
                }
            }
        }

        return sol;   
    } 




    else{
        //DESDE EL FINAL HASTA EL ORIGEN
    int sol=matriz[rows][cols], aux, r=rows, c=cols;
    almacenParser[rows][cols] = "x";

    while(r>=0 && c>=0){

        if(c==0 && r==0) break;   //Primera posición
        
        if(c==0){ 
            sol+= matriz[r-1][c];   //Primera columna
            almacenParser[r-1][c] = "x";
            r--;
        }
        else if(r==0){ 
            sol+= matriz[r][c-1];  //Primera fila
            almacenParser[r][c-1] = "x";
            c--;
        }
        else{    //El mínimo de los tres lados
            aux = min(matriz[r-1][c-1], min(
                    matriz[r-1][c],
                    matriz[r][c-1]
                )
            );
            sol += aux;

             if(aux==matriz[r-1][c-1]){
                almacenParser[r-1][c-1] = "x";
                r--; c--;
            }
            else if(aux==matriz[r][c-1]){
                almacenParser[r][c-1] = "x";
                c--;
            }
            else{
                almacenParser[r-1][c] = "x";
                r--;
            }
        }
    }

    return sol;    

    }

}


void show_usage () {
    cerr << "\t Usage: mcp [--p2D] -f fichero \n"
    " --p2D se muestra un camino de dificultad mínima\n"
    " -f se esperará el nombre del fichero a continuación."<<endl;
}

int main(int argc, char *argv[]){

    bool p2D = false;
    string fichero;

    if(argc==1){ show_usage(); exit(EXIT_FAILURE); }
    //----------------- RECOGIENDO PARÁMETROS -----------------------
    for ( int i = 1; i < argc ; i ++ ) {
        string arg = argv [ i ];
        if( arg == "-f" ) {      //-f
            i ++;
            if( i >= argc ) {
                cerr << "ERROR: missing filename." << endl ;
                show_usage();
                exit ( EXIT_FAILURE );
            }
            //Fichero
            fichero = argv [ i ];
        } 
        else if( arg == "--p2D" ) {   //--p2D
            p2D = true;
        }
        else{
            cerr<<"ERROR: unknown option "<<argv[i]<<endl;
            show_usage();
            exit(EXIT_FAILURE);
        }
    }
    //----------------- !RECOGIENDO PARÁMETROS -----------------------


    //-------------- GUARDA FICHERO ---------------------
    if(fichero.empty()) {
        cerr << "ERROR: missing filename." << endl ;
        show_usage();
        exit ( EXIT_FAILURE );
    }

    ifstream is(fichero);
    if( !is ) {
        cerr << "ERROR: can’t open file: " << fichero << endl ;
        exit ( EXIT_FAILURE );
    }

    int rows , cols ;
    is >> rows >> cols ;
    vector< vector<int>> matriz( rows , vector<int>(cols));
    for ( int i = 0; i < rows ; i ++ )
        for ( int j = 0; j < cols ; j ++ )
            is >> matriz[ i ][ j ];

    //-------------- !GUARDA FICHERO -------------------

    vector<vector<string>> x(rows, vector<string>(cols));
    for ( int i = 0; i < rows ; i ++ )
        for ( int j = 0; j < cols ; j ++ )
                    x[i][j] = ".";
    almacenParser = x;


    int orig = mcp_greedy(matriz, rows-1, cols-1, true);
    almacenParser = x;
    int dst = mcp_greedy(matriz, rows-1, cols-1, false);
    int min = dst;

    if(orig<=dst){
        almacenParser = x;
        mcp_greedy(matriz, rows-1, cols-1, true);  
        min = orig;
    }
    //Si orig es menor o igual que dst ejecutamos de nuevo la función para que el mapa sea suyo

    cout<<orig<<" "<<dst<<endl;


    if(p2D) mcp_greedy_parser(rows-1, cols-1, min);


}