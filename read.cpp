#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> get_input();

int main()
{
    vector<int> input;
    input = get_input();

    for (auto elem : input)
    {
        cout << elem;
    }
    return 0;
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