#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <tuple>
#include <limits>
#include <chrono>
#include <iomanip>

using namespace std;

const int INF = numeric_limits<int>::max();
int visit = 0, explored = 0, leaf = 0, unfeasible = 0, not_promising = 0;

void usage() {
    cerr << "Usage:" << endl;
    cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file" << endl;
}

vector<vector<int>> matrix(const string &file_name, int &r, int &c) {
    ifstream is(file_name);
    if (!is) {
        cerr << "ERROR: can't open file: " << file_name << "." << endl;
        exit(EXIT_FAILURE);
    }

    is >> r >> c;
    vector<vector<int>> mat(r, vector<int>(c));

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            is >> mat[i][j];
        }
    }

    return mat;
}

bool inside_matrix(int i, int j, int n, int m) {
    return i >= 0 && i < n && j >= 0 && j < m;
}

int mcp_it_matrix(const vector<vector<int>> &map, vector<vector<int>> &iterative) {
    int n = map.size();
    int m = map[0].size();

    iterative[0][0] = map[0][0];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i-1][j] + map[i][j]); // movimiento arriba
            }
            if (j > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i][j-1] + map[i][j]); // movimiento izquierda
            }
            if (i > 0 && j > 0) {
                iterative[i][j] = min(iterative[i][j], iterative[i-1][j-1] + map[i][j]); // movimiento diagonal
            }
        }
    }

    return iterative[n-1][m-1];
}

int mcp_bt(const vector<vector<int>> &maze, vector<vector<bool>> &visited, vector<pair<int, int>> &best_path, vector<pair<int, int>> &current_path, vector<string> &best_directions, vector<string> &current_directions, int x, int y, int current_cost, int &best_cost) {
    if (x == maze.size() - 1 && y == maze[0].size() - 1) {
        leaf++;
        current_path.push_back({x, y});
        if (current_cost + maze[x][y] < best_cost) {
            best_cost = current_cost + maze[x][y];
            best_path = current_path;
            best_directions = current_directions;
        }
        current_path.pop_back();
        return current_cost + maze[x][y];
    }

    visit++;
    current_cost += maze[x][y];
    current_path.push_back({x, y});

    // Si el coste actual ya supera el mejor coste, no continuar explorando esta rama
    if (current_cost >= best_cost) {
        not_promising++;
        current_path.pop_back();
        return INF;
    }

    int n = maze.size();
    int m = maze[0].size();

    enum Step {SE, E, S, N, NE, SW, W, NW};
    map<Step, tuple<int, int, string>> steps_inc = {
        {N, make_tuple(-1, 0, "N")},
        {NE, make_tuple(-1, 1, "NE")},
        {E, make_tuple(0, 1, "E")},
        {SE, make_tuple(1, 1, "SE")},
        {S, make_tuple(1, 0, "S")},
        {SW, make_tuple(1, -1, "SW")},
        {W, make_tuple(0, -1, "W")},
        {NW, make_tuple(-1, -1, "NW")}
    };

    for (const auto &it : steps_inc) {
        int incx, incy;
        string direction;
        tie(incx, incy, direction) = it.second;
        int newx = x + incx;
        int newy = y + incy;

        if (inside_matrix(newx, newy, n, m) && !visited[newx][newy]) {
            visited[newx][newy] = true;
            explored++;
            current_directions.push_back(direction);

            int cost = mcp_bt(maze, visited, best_path, current_path, best_directions, current_directions, newx, newy, current_cost, best_cost);

            if (cost < best_cost) {
                best_cost = cost;
            }

            visited[newx][newy] = false;
            current_directions.pop_back();
        }
    }

    current_path.pop_back();
    return best_cost;
}

void parser(const vector<vector<int>> &maze, const vector<vector<bool>> &visited) {
    int cost = 0;
    for(int i = 0; i < visited.size(); i++){
        for(int j = 0; j < visited[0].size(); j++){
            if(visited[i][j] == true){
                cout << "x";
                cost += maze[i][j];
            }
            else{
                cout << ".";
            }
        }
        cout << endl;
    }

    cout << cost << endl;
}

void print_directions(const vector<string> &directions) {
    cout<<"<";
    for (const auto &dir : directions) {
        
        if(dir == "N"){
            cout<<1;
        }
        else if(dir == "NE"){
            cout<<2;
        }
        else if(dir == "E"){
            cout<<3;
        }
        else if(dir == "SE"){
            cout<<4;
        }
        else if(dir == "S"){
            cout<<5;
        }
        else if(dir == "SW"){
            cout<<6;
        }
        else if(dir == "W"){
            cout<<7;
        }
        else if(dir == "NW"){
            cout<<8;
        }
    }
    cout <<">"<< endl;
}

void output(bool p, bool p2D, const vector<vector<int>> &map, int r, int c) {
    auto start = std::chrono::steady_clock::now();
    vector<vector<bool>> visited(r, vector<bool>(c, false));
    vector<vector<int>> iterative(r, vector<int>(c, INF));
    int shortest_path_iterative = mcp_it_matrix(map, iterative);

    visited[0][0] = true; // Empezamos desde el punto inicial
    int best_cost = shortest_path_iterative;

    vector<pair<int, int>> best_path;
    vector<pair<int, int>> current_path;
    vector<string> best_directions;
    vector<string> current_directions;

    int bt = mcp_bt(map, visited, best_path, current_path, best_directions, current_directions, 0, 0, 0, best_cost);

    // Actualizamos visited con el mejor camino encontrado
    for (const auto& coord : best_path) {
        visited[coord.first][coord.second] = true;
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    cout << bt << endl;
    cout << visit << " " << explored << " " << leaf << " " << unfeasible << " " << not_promising << endl;
    cout << fixed << setprecision(3) << seconds << endl;

    if(p2D){
        parser(map, visited);
    }

    if(p){
        print_directions(best_directions);
    }
}

int main(int argc, char* argv[]) {
    string filename;
    bool p2D = false;
    bool p = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg == "-p") {
            p = true;
        } else if (arg == "--p2D") {
            p2D = true;
        } else if (arg == "-f") {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                filename = argv[++i];
            } else {
                cerr << "ERROR: missing filename." << endl;
                usage();
                return 1;
            }
        } else {
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
    output(p, p2D, map, rows, cols);

    return 0;
}
