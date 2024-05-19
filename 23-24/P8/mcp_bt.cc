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

int mcp_bt(const vector<vector<int>> &maze, vector<vector<bool>> &visited, int x, int y, int current_cost, int &best_cost) {
    if (x == maze.size() - 1 && y == maze[0].size() - 1) {
        leaf++;
        return current_cost + maze[x][y];
    }

    visit++;
    current_cost += maze[x][y];

    // Si el coste actual ya supera el mejor coste, no continuar explorando esta rama
    if (current_cost >= best_cost) {
        not_promising++;
        return INF;
    }

    int n = maze.size();
    int m = maze[0].size();

    enum Step {SE, E, S, N, NE, SW, W, NW};
    map<Step, tuple<int, int>> steps_inc = {
        {N, make_tuple(-1, 0)},
        {NE, make_tuple(-1, 1)},
        {E, make_tuple(0, 1)},
        {SE, make_tuple(1, 1)},
        {S, make_tuple(1, 0)},
        {SW, make_tuple(1, -1)},
        {W, make_tuple(0, -1)},
        {NW, make_tuple(-1, -1)}
    };

    for (const auto &it : steps_inc) {
        int incx, incy;
        tie(incx, incy) = it.second;
        int newx = x + incx;
        int newy = y + incy;

        if (inside_matrix(newx, newy, n, m) && !visited[newx][newy]) {
            visited[newx][newy] = true;
            explored++;

            int cost = mcp_bt(maze, visited, newx, newy, current_cost, best_cost);

            if (cost < best_cost) {
                best_cost = cost;
            }

            visited[newx][newy] = false;
        }
    }

    return best_cost;
}

void output(bool p, bool p2D, const vector<vector<int>> &map, int r, int c) {
    auto start = std::chrono::steady_clock::now();
    vector<vector<bool>> visited(r, vector<bool>(c, false));
    vector<vector<int>> iterative(r, vector<int>(c, INF));
    int shortest_path_iterative = mcp_it_matrix(map, iterative);

    visited[0][0] = true; // Empezamos desde el punto inicial
    int best_cost = shortest_path_iterative;
    int bt = mcp_bt(map, visited, 0, 0, 0, best_cost);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    cout << bt << endl;
    cout << visit << " " << explored << " " << leaf << " " << unfeasible << " " << not_promising << endl;
    cout<< fixed << setprecision(3) << seconds <<endl;
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
