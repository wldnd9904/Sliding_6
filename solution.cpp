#include <iostream>
#include <chrono>
using namespace std;
int puzzle[5][6] = {
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 10, 11, 12},
    {13, 14, 15, 16, 17, 18},
    {19, 20, 21, 22, 23, 24},
    {25, 26, 27, 28, 29, 0}};
string makePuzzle(int puzzle[5][6]);
void Solver(int puzzle[5][6]);
bool Solvable(const string &);

int main()
{
    /*
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
    Solver(puzzle);
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
    std::chrono::milliseconds millisecond =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Time Cost: " << millisecond.count() << " ms\n";*/
    string puzzleStr = makePuzzle(puzzle);
    cout << puzzleStr << endl;
    std::cout << Solvable(puzzleStr) << std::endl;
}

string makePuzzle(int puzzle[5][6])
{
    string ret;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            ret.push_back(puzzle[i][j] + 64);
        }
    }
    return ret;
}
void Solver(int puzzle[5][6]) {}
bool Solvable(const string &puzzle)
{
    int inversion = 0;
    // inversion이 짝수개라면 풀 수 있다.
    const int size = puzzle.size();
    for (int i = 0; i < size; i++)
    {
        int pivot = puzzle[i];
        if (pivot == 0)
            continue;
        for (int j = i + 1; j < size; j++)
        {
            if (puzzle[j] != 64 && puzzle[j] < pivot)
                inversion++;
        }
    }
    return inversion % 2 == 0;
}