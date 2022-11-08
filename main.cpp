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

void initialize_maze(vector<vector<int>> &maze);
void display_maze(vector<vector<int>> maze);

int main(int argc, char **argv)
{
    vector<vector<int>> maze;
    initialize_maze(maze);
    display_maze(maze);
    return 0;
}

void initialize_maze(vector<vector<int>> &maze)
{
    int rows = 5;
    int cols = 10;

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