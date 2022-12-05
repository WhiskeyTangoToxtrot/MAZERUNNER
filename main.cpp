#include <iostream>
#include <vector>
#include <string>

#define INT_MAX 2147483647

using namespace std;

//errors
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

//global variable
int A_x, A_y, B_x, B_y;
int nb_lines, nb_cols;
vector<vector<int>> maze;
vector<vector<int>> mazeWithPath;
vector<string> paths = {""};

void initialize_maze(vector<int> &input);

void check_too_small();

void check_overlap_ab();

void check_overlap_ab_full_border();

void init_border_walls();

void check_overlap_full(int &current_value, int &i_line, int &i_row);

void check_overlap_ab_full_inner(int &i_line, int &i_row);

void check_bad_location(int &i_line, int &i_row);

void display_maze(vector<vector<int>> &maze_to_display);

void solve_dist();

void display_paths();

void rec_path(pair<int, int> current_node);

void sub_path(pair<int, int> neighbour_index_pair, int &min_dist, int &nb_sub_path,
              string &current_path, char direction);

pair<int, int> extract_min(vector<pair<int, int>> &queue);

vector<string> find_shortest_paths();

vector<int> get_input();

vector<pair<int, int>> get_neighbours(pair<int, int> &current_index_pair);

////////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////////

int main() {

    vector<int> input = get_input();

    //init global variable
    nb_lines = input[0], nb_cols = input[1];
    A_x = input[3], A_y = input[2];
    B_x = input[5], B_y = input[4];

    vector<vector<int>> temp(nb_lines, vector<int>(nb_cols, INT_MAX));
    maze = temp;

    initialize_maze(input);

    mazeWithPath = maze;

    display_maze(maze);

    solve_dist();

    find_shortest_paths();

    display_maze(mazeWithPath);

    display_paths();

    return 0;
}

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

void initialize_maze(vector<int> &input) {

    check_too_small();
    check_overlap_ab();
    check_overlap_ab_full_border();

    init_border_walls();

    size_t nb_obstacle = input[6];
    for (size_t i = 7; i < 7 + nb_obstacle * 2; i += 2) {
        int i_line = input[i];
        int i_row = input[i + 1];

        int current_value = maze[i_line][i_row];

        check_overlap_full(current_value, i_line, i_row);
        check_overlap_ab_full_inner(i_line, i_row);
        check_bad_location(i_line, i_row);

        //init inner walls
        maze[i_line][i_row] = -1;
    }
}

void check_too_small() {
    if (nb_cols < 3 || nb_lines < 3) {
        print_error(TOO_SMALL);
    }
}

void check_overlap_ab() {
    if (A_x == B_x && A_y == B_y) {
        print_error(OVERLAP_AB);
    }
}

void check_overlap_ab_full_border() {
    if (A_x == 0 || A_x == nb_cols - 1 || A_y == 0 || A_y == nb_lines - 1) {
        print_error(OVERLAP_AB_FULL, true, A_y, A_x);
    } else if (B_x == 0 || B_x == nb_cols - 1 || B_y == 0 || B_y == nb_lines - 1) {
        print_error(OVERLAP_AB_FULL, true, B_y, B_x);
    }
}

void check_overlap_full(int &current_value, int &i_line, int &i_row) {
    if (current_value < 0) {
        print_error(OVERLAP_FULL, true, i_line, i_row);
    }
}

void check_overlap_ab_full_inner(int &i_line, int &i_row) {
    if (i_line == A_y && i_row == A_x || i_line == B_y && i_row == B_x) {
        print_error(OVERLAP_AB_FULL, true, i_line, i_row);
    }
}

void check_bad_location(int &i_line, int &i_row) {
    if (i_row <= 0 || i_row >= nb_cols - 1 || i_line <= 0 || i_line >= nb_lines - 1) {
        print_error(BAD_LOCATION, true, i_row, i_line);
    }
}


void init_border_walls() {
    for (size_t i_line = 0; i_line < nb_lines; i_line++) {
        maze[i_line][0] = -1;
        maze[i_line][nb_cols - 1] = -1;
    }

    for (size_t i_row = 0; i_row < nb_cols; i_row++) {
        maze[0][i_row] = -1;
        maze[nb_lines - 1][i_row] = -1;
    }
}

void display_maze(vector<vector<int>> &maze_to_display) {
    string maze_str;

    for (auto &row: maze_to_display) {
        for (auto &cell: row) {
            if (cell == -1) {
                maze_str += '#';
            } else if (cell == -2) {
                maze_str += '.';
            } else {
                maze_str += ' ';
            }
        }

        maze_str.append("\n");
    }

    maze_str[A_y * (nb_cols + 1) + A_x] = 'A';
    maze_str[B_y * (nb_cols + 1) + B_x] = 'B';

    cout << maze_str << endl;
}

void solve_dist() {
    vector<pair<int, int>> pq;
    maze[B_y][B_x] = 0;
    //add starting node
    pq.push_back({B_y, B_x});
    pair<int, int> current_node;

    while (pq.size() > 0) {
        //get current node neighbours
        current_node = extract_min(pq);
        int c_dist = maze[current_node.first][current_node.second];
        vector<pair<int, int>> neighbours_index_pair = get_neighbours(current_node);

        for (auto &pair: neighbours_index_pair) {
            int n_y = pair.first, n_x = pair.second;

            // check if faster way exists
            if (maze[n_y][n_x] > c_dist + 1) {
                //add neighbour to priority queue
                pq.push_back({n_y, n_x});
                //set distance from end
                maze[n_y][n_x] = c_dist + 1;
            }
        }
    }

    if (maze[A_y][A_x] == INT_MAX) {
        print_error(NO_SOLUTION);
    }
}

vector<string> find_shortest_paths() {
    pair<int, int> current_node = {A_y, A_x};

    rec_path(current_node);

    return paths;
}

void rec_path(pair<int, int> current_node) {

    int c_y = current_node.first;
    int c_x = current_node.second;

    //stopping condition
    if (c_y == B_y && c_x == B_x) {
        return;
    }

    //draw dot on mazeWithPath
    mazeWithPath[c_y][c_x] = -2;

    vector<pair<int, int>> neighbours_index_pair = get_neighbours(current_node);

    //find mind_dist in neighbours_index_pair
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

    sub_path({c_y - 1, c_x}, min_dist,
             nb_sub_path, current_path, 'D');

    sub_path({c_y, c_x - 1}, min_dist,
             nb_sub_path, current_path, 'L');

    sub_path({c_y, c_x + 1}, min_dist,
             nb_sub_path, current_path, 'R');

    sub_path({c_y + 1, c_x}, min_dist,
             nb_sub_path, current_path, 'U');
}

void sub_path(pair<int, int> neighbour_index_pair, int &min_dist, int &nb_sub_path,
              string &current_path, char direction) {
    if (maze[neighbour_index_pair.first][neighbour_index_pair.second] == min_dist) {

        nb_sub_path++;

        if (nb_sub_path > 1) {
            paths.push_back(current_path + direction);
        } else {
            paths.back() += direction;
        }

        rec_path({neighbour_index_pair.first, neighbour_index_pair.second});
    }
}

vector<pair<int, int>> get_neighbours(pair<int, int> &current_index_pair) {
    vector<pair<int, int>> neighbours_index_pair;
    int c_y = current_index_pair.first, c_x = current_index_pair.second;

    if (maze[c_y + 1][c_x] != -1) {
        neighbours_index_pair.push_back({c_y + 1, c_x});
    }

    if (maze[c_y][c_x - 1] != -1) {
        neighbours_index_pair.push_back({c_y, c_x - 1});
    }

    if (maze[c_y][c_x + 1] != -1) {
        neighbours_index_pair.push_back({c_y, c_x + 1});
    }

    if (maze[c_y - 1][c_x] != -1) {
        neighbours_index_pair.push_back({c_y - 1, c_x});
    }

    return neighbours_index_pair;
}


void display_paths() {
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

pair<int, int> extract_min(vector<pair<int, int>> &queue) {
    vector<pair<int, int>> queue_copy = queue;
    size_t length = queue.size();
    pair<int, int> node = queue_copy[0];

    for (size_t i = 0; i < length - 1; i++) {
        queue[i] = queue_copy[i + 1];
    }

    queue.pop_back();

    return node;
}
