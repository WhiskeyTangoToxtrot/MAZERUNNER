#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
//########################################################################################
// Error messages
const string TOO_SMALL("Error: the number of lines/columns must be greater than 2");
const string BAD_LOCATION("Error: the cell is not in the inner space of the grid");
const string OVERLAP_AB("Error: self overlap of start and end cells is not allowed");
const string OVERLAP_FULL("Error: this cell is already full");
const string OVERLAP_AB_FULL("Error: overlap of start or end cells is not allowed");

// special case
const string NO_SOLUTION("No path from start to end cells");

// prototype de la fonction d'affichage de message d'error
void print_error(string message, bool with_cell = false, unsigned i = 0, unsigned j = 0);
//########################################################################################

//global variable
int A_x;
int A_y;
int B_x;
int B_y;

//prototype
void initialize_maze(vector<vector<char>> &maze, vector<int> &input);
void step_rec(vector<vector<char>> &maze,vector<vector<int>> &priority_queue, int &current_y, int &current_x);
void display_maze(vector<vector<char>> &maze);
void solve(vector<vector<char>> maze);

vector<int> get_input();

int main() {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //########################################################################################

    vector<int> input = get_input();
    const int nb_lines = input[0], nb_cols = input[1];
    vector<vector<char>> maze(nb_lines, vector<char>(nb_cols, ' '));

    initialize_maze(maze, input);

    display_maze(maze);

    solve(maze);
    //########################################################################################

    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";


    return 0;
}


void solve(vector<vector<char>> maze) {
    maze[A_y][A_x] = 0;
    vector<vector<int>> priority_queue = {{A_y,A_x}};
    step_rec(maze,priority_queue,A_y,A_x);
}

void step_rec(vector<vector<char>> &maze,vector<vector<int>> &priority_queue,int &current_y,int &current_x){
    int current_value = maze[priority_queue[0]]
    if(maze[current_y+1][current_x] = ' '){
        priority_queue.push_back({current_y+1,current_x,1})
    }
}

/*
vector<vector<unsigned int>> get_empty_cells(vector<vector<char>> &maze) {
    vector<vector<unsigned int>> empty_cells;
    unsigned int nb_row = maze.size();
    unsigned int nb_col = maze[0].size();

    for (unsigned int i_col = 0; i_col < nb_row; i_col++) {
        for (unsigned int i_row = 0; i_row < nb_col; i_row++) {
            if (maze[i_col][i_row] == ' ') {
                empty_cells.push_back({i_col, i_row, UINT_MAX});
            }
        }
    }
    return empty_cells;
}*/


void initialize_maze(vector<vector<char>> &maze, vector<int> &input) {
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

    maze[A_y][A_x] = 'A';
    maze[B_y][B_x] = 'B';

    // init border walls
    for (size_t i_line = 0; i_line < nb_lines; i_line++) {
        maze[i_line][0] = '#';
        maze[i_line][nb_cols - 1] = '#';
    }

    for (size_t i_row = 0; i_row < nb_cols; i_row++) {
        maze[0][i_row] = '#';
        maze[nb_lines - 1][i_row] = '#';
    }

    //init inner walls (obstacle)
    size_t nb_obstacle = input[6];
    for (size_t i = 7; i < 7 + nb_obstacle * 2; i += 2) {
        int i_line = input[i];
        int i_row = input[i + 1];


        //CHECK INPUT
        char current_value = maze[i_line][i_row];
        if (current_value != ' ') {
            //OVERLAP_FULL
            if (current_value == '#') {
                print_error(OVERLAP_FULL, true, i_line, i_row);
            }
            //OVERLAP_AB_FULL inner cells
            if (current_value == 'A' || current_value == 'B') {
                print_error(OVERLAP_AB_FULL, true, i_line, i_row);
            }
        } else {
            //BAD_LOCATION
            if (i_row < 0 || i_row > nb_cols - 1 || i_line < 0 || i_line > nb_lines - 1) {
                print_error(BAD_LOCATION, true, i_line, i_row);
            }
            maze[i_line][i_row] = '#';

        }
    }
}




void display_maze(vector<vector<char>> &maze) {
    string maze_str = " ";
    for (auto &row: maze) {
        for (auto &cell: row) {
            maze_str += (cell);
        }
        maze_str.append("\n");
    }
    cout << maze_str;
}

void print_error(string message, bool with_cell, unsigned i, unsigned j) {
    cout << message;

    if (with_cell) {
        cout << " : ligne = " << i << " colonne = " << j;
    }

    cout << endl;

    exit(0);
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
