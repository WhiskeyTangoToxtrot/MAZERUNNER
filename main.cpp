//////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
//////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>

#define INT_MAX 2147483647

////////////////////////////////////////////////////////////////////////////////////
// NAMESPACE
////////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////////
// ERROR MESSAGES
////////////////////////////////////////////////////////////////////////////////////

const string TOO_SMALL("Error: the number of lines/columns must be greater than 2");
const string BAD_LOCATION("Error: the cell is not in the inner space of the grid");
const string OVERLAP_AB("Error: self overlap of start and end cells is not allowed");
const string OVERLAP_FULL("Error: this cell is already full");
const string OVERLAP_AB_FULL("Error: overlap of start or end cells is not allowed");

// special case
const string NO_SOLUTION("No path from start to end cells");

// prototype de la fonction d'affichage de message d'error
void print_error(const string &message, bool with_cell = false,
                 unsigned i = 0, unsigned j = 0);

////////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLE
////////////////////////////////////////////////////////////////////////////////////

//global variable
int A_x;
int A_y;
int B_x;
int B_y;
vector<vector<int>> mazeWithPath;
////////////////////////////////////////////////////////////////////////////////////
// CLASS
////////////////////////////////////////////////////////////////////////////////////

class Priority_queue {
public:
    size_t get_pq_size() {
        return queue.size();
    }

    pair<int, int> extract_min() {
        vector<pair<int, int>> queue_copy = queue;
        size_t length = queue.size();
        pair<int, int> node = queue_copy[0];

        for (size_t i = 0; i < length - 1; i++) {
            queue[i] = queue_copy[i + 1];
        }
        queue.pop_back();
        return node;
    }

    void insert(pair<int, int> pair) {
        queue.push_back(pair);
    }

private:
    vector<pair<int, int>> queue;
};

////////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
////////////////////////////////////////////////////////////////////////////////////

void initialize_maze(vector<vector<int>> &maze, vector<int> &input);

void display_maze(vector<vector<int>> &maze);


void solve_dist(vector<vector<int>> &maze);

void display_paths(vector<string> &paths);

vector<string> find_shortest_paths(vector<vector<int>> &maze);

void rec_path(vector<vector<int>> &maze, pair<int, int> current_node,
              vector<string> &paths);

vector<pair<int, int>> get_neighbours(pair<int, int> &current_index_pair,
                                      vector<vector<int>> &maze);

vector<int> get_input();

////////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////////

int main() {

    vector<int> input = get_input();

    const int nb_lines = input[0], nb_cols = input[1];
    vector<vector<int>> maze(nb_lines, vector<int>(nb_cols, INT_MAX));
    initialize_maze(maze, input);
    mazeWithPath = maze;
    display_maze(maze);

    solve_dist(maze);

    vector<string> paths = find_shortest_paths(maze);

    display_maze(mazeWithPath);

    display_paths(paths);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// FUNCTION BODY
////////////////////////////////////////////////////////////////////////////////////

vector<int> get_input() {
    vector<int> input;
    int a;
    for (int i = 0; i < 6; i++) {

        cin >> a;
        input.push_back(a);
    }
    int nbP;
    cin >> nbP;
    input.push_back(nbP);
    for (int i = 0; i < nbP * 2; i++) {
        cin >> a;
        input.push_back(a);
    }

    return input;
}

void initialize_maze(vector<vector<int>> &maze, vector<int> &input) {
    size_t nb_lines = maze.size();
    size_t nb_cols = maze[0].size();

    //TO_SMALL
    if (nb_cols < 3 || nb_lines < 3) {
        print_error(TOO_SMALL);
    }

    //OVERLAP_AB
    A_x = input[3];
    A_y = input[2];
    B_x = input[5];
    B_y = input[4];

    if (A_x == B_x && A_y == B_y) {

        print_error(OVERLAP_AB);

    }
    if (A_x == 0 || A_x == nb_cols - 1 || A_y == 0 || A_y == nb_lines - 1) {
        print_error(OVERLAP_AB_FULL, true, A_y, A_x);
    } else if (B_x == 0 || B_x == nb_cols - 1 || B_y == 0 || B_y == nb_lines - 1) {
        print_error(OVERLAP_AB_FULL, true, B_y, B_x);
    }

    // init border walls
    for (size_t i_line = 0; i_line < nb_lines; i_line++) {
        maze[i_line][0] = -1;
        maze[i_line][nb_cols - 1] = -1;
    }

    for (size_t i_row = 0; i_row < nb_cols; i_row++) {
        maze[0][i_row] = -1;
        maze[nb_lines - 1][i_row] = -1;
    }

    //init inner walls (obstacle)
    size_t nb_obstacle = input[6];
    for (size_t i = 7; i < 7 + nb_obstacle * 2; i += 2) {
        int i_line = input[i];
        int i_row = input[i + 1];


        //CHECK INPUT
        int current_value = maze[i_line][i_row];

        //OVERLAP_FULL
        if (current_value < 0) {
            print_error(OVERLAP_FULL, true, i_line, i_row);
        }
        //OVERLAP_AB_FULL inner cells
        if (i_line == A_y && i_row == A_x || i_line == B_y && i_row == B_x) {
            print_error(OVERLAP_AB_FULL, true, i_line, i_row);
        }

        //BAD_LOCATION
        if (i_row < 0 || i_row > nb_cols - 1 || i_line < 0 || i_line > nb_lines - 1) {
            print_error(BAD_LOCATION, true, i_line, i_row);
        }
        maze[i_line][i_row] = -1;
    }
}

void display_maze(vector<vector<int>> &maze) {
    size_t nb_cols = maze[0].size();
    string maze_str;
    for (auto &row: maze) {
        for (auto &cell: row) {
            if (cell == -1) {
                maze_str += '#';
            } else if (cell == -2) {
                maze_str += '*';
            } else {
                maze_str += ' ';
            }

        }
        maze_str.append("\n");
    }
    maze_str[A_y * (nb_cols + 1) + A_x] = 'A';
    maze_str[B_y * (nb_cols + 1) + B_x] = 'B';
    cout << maze_str<<endl;
}

void solve_dist(vector<vector<int>> &maze) {
    Priority_queue pq;
    maze[B_y][B_x] = 0;
    pq.insert({B_y, B_x});
    pair<int, int> current_node;
    while (pq.get_pq_size() > 0) {
        current_node = pq.extract_min();
        int c_dist = maze[current_node.first][current_node.second];
        vector<pair<int, int>> neighbours_index_pair = get_neighbours(current_node,
                                                                      maze);
        for (auto &pair: neighbours_index_pair) {
            int n_y = pair.first, n_x = pair.second;
            if (maze[n_y][n_x] > c_dist + 1) {
                pq.insert({n_y, n_x});
                maze[n_y][n_x] = c_dist + 1;
            }
        }
    }
    if (maze[A_y][A_x] == INT_MAX) {
        print_error(NO_SOLUTION);
    }
}

vector<string> find_shortest_paths(vector<vector<int>> &maze) {
    vector<string> paths = {""};
    pair<int, int> current_node = {A_y, A_x};
    rec_path(maze, current_node, paths);
    return paths;
}

void rec_path(vector<vector<int>> &maze, pair<int, int> current_node,
              vector<string> &paths) {
    int c_y = current_node.first;
    int c_x = current_node.second;
    mazeWithPath[c_y][c_x] = -2;
    if (c_y == B_y && c_x == B_x) {
        return;
    }
    vector<pair<int, int>> neighbours_index_pair = get_neighbours(current_node, maze);

    int min_dist = INT_MAX;
    for (auto &pair: neighbours_index_pair) {
        int n_y = pair.first;
        int n_x = pair.second;
        if ((maze[n_y][n_x] >= 0) && (maze[n_y][n_x] < min_dist)) {
            min_dist = maze[n_y][n_x];
        }
    }
    string current_path = paths.back();
    int nb_sub_path = 0;
    if (maze[c_y + 1][c_x] == min_dist) {
        nb_sub_path++;
        paths.back() += 'D';
        rec_path(maze, {c_y + 1, c_x}, paths);
    }

    if (maze[c_y][c_x - 1] == min_dist) {
        nb_sub_path++;
        if (nb_sub_path > 1) {
            paths.push_back(current_path + 'L');

        } else {
            paths.back() += 'L';
        }
        rec_path(maze, {c_y, c_x - 1}, paths);
    }

    if (maze[c_y][c_x + 1] == min_dist) {
        nb_sub_path++;

        if (nb_sub_path > 1) {
            paths.push_back(current_path + 'R');

        } else {
            paths.back() += 'R';
        }
        rec_path(maze, {c_y, c_x + 1}, paths);
    }

    if (maze[c_y - 1][c_x] == min_dist) {
        nb_sub_path++;
        if (nb_sub_path > 1) {
            paths.push_back(current_path + 'U');
        } else {
            paths.back() += 'U';
        }
        rec_path(maze, {c_y - 1, c_x}, paths);
    }
}

vector<pair<int, int>> get_neighbours(pair<int, int> &current_index_pair,
                                      vector<vector<int>> &maze) {
    vector<pair<int, int>> neighbours_index_pair;
    if (maze[current_index_pair.first + 1][current_index_pair.second] != -1) {
        neighbours_index_pair.push_back({current_index_pair.first + 1,
                                         current_index_pair.second});
    }
    if (maze[current_index_pair.first][current_index_pair.second - 1] != -1) {
        neighbours_index_pair.push_back({current_index_pair.first,
                                         current_index_pair.second - 1});
    }
    if (maze[current_index_pair.first][current_index_pair.second + 1] != -1) {
        neighbours_index_pair.push_back({current_index_pair.first,
                                         current_index_pair.second + 1});
    }
    if (maze[current_index_pair.first - 1][current_index_pair.second] != -1) {
        neighbours_index_pair.push_back({current_index_pair.first - 1,
                                         current_index_pair.second});
    }
    return neighbours_index_pair;
}


void display_paths(vector<string> &paths) {
    string paths_str;
    for (auto &path: paths) {
        paths_str += path + "\n";
    }
    cout << paths_str;
}


void print_error(const string &message, bool with_cell, unsigned i, unsigned j) {
    cout << message;

    if (with_cell) {
        cout << " : ligne = " << i << " colonne = " << j;
    }

    cout << endl;

    exit(0);
}

