#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

void initialize_maze(vector<vector<int>> &maze, vector<int> input);
void display_maze(vector<vector<int>> maze);

vector<int> get_input();

int main()
{
    vector<int> input = get_input();
    for (auto elem : input)
    {
        cout << elem << " ";
    }
    cout << endl;

    vector<vector<int>> maze;
    initialize_maze(maze, input);
    display_maze(maze);
    return 0;
}

void initialize_maze(vector<vector<int>> &maze, vector<int> input)
{
    int rows = input[0];
    int cols = input[1];

    if (rows < 2 || cols < 2)
    {
        print_error(TOO_SMALL);
        exit(EXIT_FAILURE);
    }

    // init border
    for (int i = 0; i < rows; i++)
    {
        vector<int> v;

        for (int j = 0; j < cols; j++)
        {
            bool border = ((i == 0) || (j == 0) || (i == rows - 1) || (j == cols - 1));
            int value = 0;
            if (border)
            {
                value = -1;
            }

            v.push_back(value);
        }
        maze.push_back(v);
    }
    /*
     int beginx = input[2];
    int beginy = input[3];
    int endx = input[4];
    int endy = input[5];

    maze[beginx][beginy] = 3;
    maze[endx][endy] = 4;

    // init obstacle
    for (int i = 7; i < 7 + input[6] * 2; i += 2)
    {
        int x = input[i];
        int y = input[i + 1];

        maze[x][y] = -1;
    }
    */
    // init begin end
}

void display_maze(vector<vector<int>> maze)
{
    for (auto row : maze)
    {
        for (auto cell : row)
        {
            if (cell < 0)
            {
                cout << "#";
            }
            else if (cell == 3)
            {
                cout << "A";
            }
            else if (cell == 4)
            {
                cout << "B";
            }
            else
            {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void print_error(string message, bool with_cell, unsigned i, unsigned j)
{
    cout << message;

    if (with_cell)
    {
        cout << " : ligne = " << i << " colonne = " << j;
    }

    cout << endl;

    exit(0);
}

vector<int> get_input()
{
    vector<int> input;
    int a;
    for (int i = 0; i < 6; i++)
    {

        cin >> a;
        input.push_back(a);
    }
    int nobstacle;
    cin >> nobstacle;
    input.push_back(nobstacle);
    for (int i = 0; i < nobstacle; i++)
    {
        cin >> a;
        input.push_back(a);
    }

    return input;
}