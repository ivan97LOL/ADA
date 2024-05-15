//Iván Álvarez García DNI: 49623492A
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <unistd.h>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Stats {
    int visited;
    int explored;
    int leaf;
    int unfeasible;
    int not_promising;
    int promising_but_discarded;
    int best_solution_updated_from_leafs;
    int best_solution_updated_from_bound;

    Stats() : visited(0), explored(0), leaf(0), unfeasible(0), 
            not_promising(0), promising_but_discarded(0), best_solution_updated_from_leafs(0),
            best_solution_updated_from_bound(0) {}
};

void usage() {
    cerr << "Usage:" << endl;
    cerr << "[-p] [--p2D] -f file" << endl;
}

vector<vector<int>> matrix(const string& file_name) {
    ifstream is(file_name);
    if (!is) {
        cerr << "ERROR: can't open file '" << file_name << "'." << endl;
        exit(EXIT_FAILURE);
    }

    int r, c;
    is >> r >> c;

    vector<vector<int>> mat(r, vector<int>(c));

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            is >> mat[i][j];
        }
    }

    return mat;
}

bool inside_matrix(int i, int j, int n, int m) {
    return i >= 0 && i < n && j >= 0 && j < m;
}

void maze_bb(const vector<vector<int>>& maze, vector<vector<int>>& memory, vector<vector<bool>> &visited, int r, int c, int path, int& shortest_path_bt, vector<string>& current_path_traveled, vector<string>& path_traveled, Stats& stats) {
    int n = maze.size();
    int m = maze[0].size();

    stats.visited++;

    if(memory[r][c] <= path){
        stats.unfeasible++;
        return;
    }
    else{
        memory[r][c] = path;
    }

    if(maze[0][0] == 0){
        return;
    }
    
    if (r == n - 1 && c == m - 1) {
        stats.leaf++;
        if (path < shortest_path_bt) {
            shortest_path_bt = path;
            path_traveled = current_path_traveled;
            stats.best_solution_updated_from_leafs++;
        }
        return;
    }

    if(path >= shortest_path_bt){
        stats.not_promising++;
        return;
    }

    int direction_x[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int direction_y[] = {-1, -1, 0, 1, 1, 1, 0, -1};

    for (int i = 0; i < 8; i++) {
        int next_r = r + direction_y[i];
        int next_c = c + direction_x[i];
        int new_path = path + 1;

        if(inside_matrix(next_r, next_c, n, m) && maze[next_r][next_c] == 1 && !visited[next_r][next_c]){
            stats.explored++;
            visited[next_r][next_c] = true;
            current_path_traveled.push_back(to_string(i+1));

            maze_bb(maze, memory, visited, next_r, next_c, new_path, shortest_path_bt, current_path_traveled, path_traveled, stats);

            visited[next_r][next_c] = false;

            current_path_traveled.pop_back();
        }
        else{
            stats.unfeasible++;
        }
    }
}

void reconstruct_path(vector<vector<int>>& mark_path, vector<string> path_traveled){
    int i = 0;
    int j = 0;

    for(string move : path_traveled){
        if(move == "1"){
            i--;
            mark_path[i][j] = 1;
        }
        else if(move == "2"){
            i--;
            j++;
            mark_path[i][j] = 1;
        }
        else if(move == "3"){
            j++;
            mark_path[i][j] = 1;
        }
        else if(move == "4"){
            i++;
            j++;
            mark_path[i][j] = 1;
        }
        else if(move == "5"){
            i++;
            mark_path[i][j] = 1;
        }
        else if(move == "6"){
            i++;
            j--;
            mark_path[i][j] = 1;
        }
        else if(move == "7"){
            j--;
            mark_path[i][j] = 1;
        }
        else if(move == "8"){
            i--;
            j--;
            mark_path[i][j] = 1;
        }
    }
    mark_path[0][0] = 1;
} 


void output(bool p, bool p2D, const vector<vector<int>>& maze) {
    int shortest_path_bb = INF - 1;
    const int MAX_LENGTH = maze.size() * maze[0].size();
    vector<vector<int>> memory(maze.size(), vector<int>(maze[0].size(), INF));
    vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));
    vector<string> current_path_traveled;
    vector<string> path_traveled;
    vector<vector<int>> mark_path(maze.size(), vector<int>(maze[0].size(), 0));
    Stats stats;

    auto start_time = clock();

    visited[0][0] = true;

    maze_bb(maze, memory, visited, 0, 0, 1, shortest_path_bb, current_path_traveled, path_traveled, stats);

    auto end_time = clock();

    if(shortest_path_bb > MAX_LENGTH){
        cout<<0<<endl;
    }
    else{
        cout << shortest_path_bb << endl;
    }

    cout << stats.visited << " " << stats.explored << " " << stats.leaf << " " << stats.unfeasible << " " << stats.not_promising << endl;
    cout << 1000.0 * (end_time-start_time)/CLOCKS_PER_SEC<< endl;

    if (shortest_path_bb > MAX_LENGTH) {
        cout << "NO EXIT" << endl;
    }

    if (p2D) {
        if(shortest_path_bb < MAX_LENGTH){
            reconstruct_path(mark_path, path_traveled);
            for(unsigned long i = 0; i<mark_path.size(); i++){
                for(unsigned long j = 0; j<mark_path[0].size(); j++){
                    if(mark_path[i][j] == 1){
                        cout<<"*";
                    }
                    else{
                        cout<< maze[i][j];
                    }
                }
                cout<<endl;
            }
        }     
    }

    if (p) {
        if (shortest_path_bb > MAX_LENGTH) {
            cout << "<NO EXIT>"<<endl;
        }
        else{
            cout<<"<";
            for(string move : path_traveled){
                cout<< move;
            }
            cout<<">"<<endl;
        }
    }
}

int main(int argc, char* argv[]) {
    string filename;
    bool p = false;
    bool p2D = false;

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

        } else if (arg == "--p2D") {
            p2D = true;
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

    output(p, p2D, maze);

    return 0;
}
