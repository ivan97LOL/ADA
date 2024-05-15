// Iván Álvarez García DNI: 49623492A
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

using namespace std;

void usage() {
    cerr << "Usage:" << endl;
    cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file" << endl;
}

vector<vector<int>> matrix(string file_name, int &r, int &c){
    ifstream is (file_name);
    if (!is) {
        cerr << "ERROR: Can't open file: " << file_name << "." << endl;
        exit(EXIT_FAILURE);
    }

    is >> r >> c;
    vector<vector<int>> mat(r, vector<int>(c));

    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            is >> mat[i][j];
        }
    }
    
    return mat;
}

void mcp_greedy_parser(const vector< vector<string>> &parser, int min){
    int n = parser.size();
    int m = parser[0].size();
    for (int i=0; i<=n; i++){
        for(int j=0; j<=m; j++)
            cout<<parser[i][j];
        cout<<endl;
    }

    cout<<min<<endl;

}

int mcp_greedy(const vector< vector<int>> &matriz, int rows, int cols, vector< vector<string>> &parser ,bool origen){
    
    
    if(origen){
        
        int sol=matriz[0][0], aux, r=0, c=0;
         parser[0][0] = "x";

        while(r<=rows && c<=cols){

            if(c==cols && r==rows) break;  
            
            if(c==cols){ 
                sol+= matriz[r+1][c];  
                 parser[r+1][c] = "x";
                r++;
            }
            else if(r==rows){ 
                sol+= matriz[r][c+1];  
                 parser[r][c+1] = "x";
                c++;
            }
            else{   
                aux = min(matriz[r+1][c+1], min(
                        matriz[r+1][c],
                        matriz[r][c+1]
                    )
                );
                sol += aux;

                if(aux==matriz[r+1][c+1]){
                     parser[r+1][c+1] = "x";
                    r++; c++;
                }
                else if(aux==matriz[r+1][c]){
                     parser[r+1][c] = "x";
                    r++;
                }
                else{
                     parser[r][c+1] = "x";
                    c++;
                }
            }
        }

        return sol;   
    } 




    else{
        
    int sol=matriz[rows][cols], aux, r=rows, c=cols;
    parser[rows][cols] = "x";

    while(r>=0 && c>=0){

        if(c==0 && r==0) break;   
        if(c==0){ 
            sol+= matriz[r-1][c];   
             parser[r-1][c] = "x";
            r--;
        }
        else if(r==0){ 
            sol+= matriz[r][c-1];  
             parser[r][c-1] = "x";
            c--;
        }
        else{    
            aux = min(matriz[r-1][c-1], min(
                    matriz[r-1][c],
                    matriz[r][c-1]
                )
            );
            sol += aux;

             if(aux==matriz[r-1][c-1]){
                 parser[r-1][c-1] = "x";
                r--; c--;
            }
            else if(aux==matriz[r][c-1]){
                 parser[r][c-1] = "x";
                c--;
            }
            else{
                 parser[r-1][c] = "x";
                r--;
            }
        }
    }

    return sol;    

    }

}

void output(bool p, const vector<vector<int>> &map, int r, int c){

    vector<vector<string>> parser(map.size(), vector<string>(map[0].size(), "."));
    vector<vector<string>> x(r, vector<string>(c, "."));
    parser = x;


    int orig = mcp_greedy(map, r-1, c-1, parser, true);
    parser = x;
    int dst = mcp_greedy(map, r-1, c-1, parser ,false);
    int min = dst;

    if(orig<=dst){
        parser = x;
        mcp_greedy(map, r-1, c-1,parser ,true);  
        min = orig;
    }

    cout<<orig<<" "<<dst<<endl;


    if(p){
        mcp_greedy_parser(parser, min);
    } 
}


int main(int argc, char* argv[]) {
    string filename;
    bool p = false;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--p2D") {
            p = true;
        } 
        else if (arg == "-f") {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                filename = argv[i + 1];
                i++;
            } else {
                cerr << "ERROR: missing filename." << endl;
                usage();
                return 1;
            }
        } 
        else {
            cerr << "ERROR: unknown option " << arg << "." << endl;
            usage();
            return 1;
        }
    }

    if (filename.empty()) {
        cerr << "ERROR: missing filename." << endl;
        usage();
        return 1;
    }

    int rows, cols;
    vector<vector<int>> map = matrix(filename, rows, cols);
    output(p, map, rows, cols);

    return 0;
}
