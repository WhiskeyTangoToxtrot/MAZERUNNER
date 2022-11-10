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
void verifie_input(vector<int> &input);

int main()
{
    vector<int> input = get_input();
    verifie_input(input);

    vector<vector<int>> maze;

    initialize_maze(maze, input);
    display_maze(maze);
    return 0;
}

void initialize_maze(vector<vector<int>> &maze, vector<int> input)
{
    int rows = input[0];
    int cols = input[1];

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
    /*int Ax = input[2];
    int Ay = input[3];
    int Bx = input[4];
    int By = input[5];

    maze[Ax][beginy] = 3;
    maze[endx][endy] = 4;

    int
    // init obstacle
    for (int i = 0; i < nbP; i++)
    {
        int x = input[i * 2 + 7];
        int y = input[i * 2 + 8];

        maze[x][y] = -1;
    }
    // init begin end
    */
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
    int nbP;
    cin >> nbP;
    input.push_back(nbP);
    for (int i = 0; i < nbP * 2; i++)
    {
        cin >> a;
        input.push_back(a);
    }

    return input;
}

void verifie_input(vector<int> &input)
{
    int nbC = input[0];
    int nbL = input[1];
    // TOO_SMALL
    if (nbL <= 2 || nbC <= 2)
    {
        print_error(TOO_SMALL);
    }
    // BAD_LOCATION
    int nbP = input[6];
    for (int i = 0; i < nbP; i++)
    {
        int x = input[i * 2 + 7];
        int y = input[i * 2 + 8];
        if (!((x < nbC - 1) && (x > 0) && (y < nbL - 1) && (y > 0)))
        {
            print_error(BAD_LOCATION, true, x, y);
        }
    }
    // OVERLAP_AB
    int Ax = input[2];
    int Ay = input[3];
    int Bx = input[4];
    int By = input[5];
    if ((Ax == By) && (Ay == By))
    {
        print_error(OVERLAP_AB);
    }
    // OVERLAP_FULL
    for (int i = 0; i < nbP; i++)
    {
        int x1 = input[i * 2 + 7];
        int y1 = input[i * 2 + 8];

        for (int j = 0; j < nbP; j++)
        {
            if (i == j)
            {
                continue;
            }

            int x2 = input[j * 2 + 7];
            int y2 = input[j * 2 + 8];
            if ((x1 == x2) && (y1 == y2))
            {
                print_error(OVERLAP_FULL, true, x1, y1);
            }
        }
    }
    // OVERLAP_AB_FULL
    for (int i = 0; i < nbP; i++)
    {
        int x = input[i * 2 + 7];
        int y = input[i * 2 + 8];
        if ((Ax == x) && (Ay == y) || (Bx == x) && (By == y))
        {
            print_error(OVERLAP_AB_FULL, true, x, y);
        }
    }
}