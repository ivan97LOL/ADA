//Iván Álvarez García DNI: 49623492A
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Stats {
    int visited;
    int explored;
    int leaf;
    int unfeasible;
    int not_promising;
};

void usage() {
    cerr << "Usage:" << endl;
    cerr << "[-p] [--p2D] -f file" << endl;
}

vector<vector<int>> matrix(string file_name) {
    int r, c;
    ifstream is(file_name);
    if (!is) {
        cerr << "ERROR: can't open file '" << file_name << "'." << endl;
        exit(EXIT_FAILURE);
    }

    is >> r >> c;
    vector<vector<int>> mat(r, vector<int>(c));

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            is >> mat[i][j];
        }
    }

    return mat;
}

void maze_bt(const vector<vector<int>>& maze, vector<vector<int>>& memory, int r, int c, int& shortest_path, string& path_sequence, Stats& stats) {
    int n = maze.size();
    int m = maze[0].size();

    stats.visited++;

    if (r == n - 1 && c == m - 1) {
        shortest_path = 0;
        return;
    }

    int direction_x[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int direction_y[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++) {
        int next_r = r + direction_x[i];
        int next_c = c + direction_y[i];

        if (next_r >= 0 && next_r < n && next_c >= 0 && next_c < m && maze[next_r][next_c] == 1 && memory[next_r][next_c] == 0) {
            stats.visited++;
            memory[next_r][next_c] = 1;

            if (shortest_path == INT_MAX) {
                maze_bt(maze, memory, next_r, next_c, shortest_path, path_sequence, stats);
            }

            if (shortest_path != INT_MAX && shortest_path + 1 < path_sequence.length()) {
                path_sequence = to_string(i + 1) + path_sequence;
            }

            memory[next_r][next_c] = 0;
        } else {
            stats.unfeasible++;
        }
    }

    stats.explored++;
    if (shortest_path == INT_MAX) {
        stats.leaf++;
    } else {
        stats.not_promising++;
    }
}

void reconstructPath(const vector<vector<int>>& maze, vector<vector<int>>& path) {
    int n = maze.size();
    int m = maze[0].size();

    int curr_r = n - 1;
    int curr_c = m - 1;

    while (curr_r != 0 || curr_c != 0) {

        path[curr_r][curr_c] = 1; // Marcar la celda en el camino

        int move = maze[curr_r][curr_c];

        if (move == 1) { // Norte
            curr_r++;
        } else if (move == 2) { // Noreste
            curr_r++;
            curr_c--;
        } else if (move == 3) { // Este
            curr_c--;
        } else if (move == 4) { // Sureste
            curr_r--;
            curr_c--;
        } else if (move == 5) { // Sur
            curr_r--;
        } else if (move == 6) { // Suroeste
            curr_r--;
            curr_c++;
        } else if (move == 7) { // Oeste
            curr_c++;
        } else if (move == 8) { // Noroeste
            curr_r++;
            curr_c++;
        }
    }

    path[0][0] = 1; // Marcar la posición inicial en el camino
}

void output(bool p, bool p2D, const vector<vector<int>>& maze, const string& path_sequence, const Stats& stats, double cpu_time) {
    int shortest_path_bt = path_sequence.length();

    cout << shortest_path_bt << endl;
    cout << stats.visited << " " << stats.explored << " " << stats.leaf << " " << stats.unfeasible << " " << stats.not_promising << endl;
    cout << cpu_time << endl;

    if(shortest_path_bt == 0){
        cout<<"NO EXIT"<<endl;
    }

    if (p) {
        if (path_sequence.empty()) {
            cout << "<NO EXIT>";
        } else {
            cout << "<" << path_sequence << ">";
        }
        cout << endl;
    }

    if (p2D) {
        for (const auto& row : maze) {
            for (int cell : row) {
                if (cell == -1) {
                    cout << '*';
                } else {
                    cout << cell;
                }
            }
            cout << endl;
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
    
    auto start_time = high_resolution_clock::now();

    vector<vector<int>> maze = matrix(filename);
    vector<vector<int>> memory(maze.size(), vector<int>(maze[0].size(), 0));
    vector<vector<int>> path(maze.size(), vector<int>(maze[0].size(), 0));

    int shortest_path = INT_MAX;
    string path_sequence;
    Stats stats;

    maze_bt(maze, memory, 0, 0, shortest_path, path_sequence, stats);
    reconstructPath(maze, path);

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    double cpu_time = static_cast<double>(duration.count());

    output(p, p2D, maze, path_sequence, stats, cpu_time);

    return 0;
}
