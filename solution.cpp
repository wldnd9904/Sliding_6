#include <iostream>
#include <chrono>

int puzzle[5][6] = {
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 10, 11, 12},
    {13, 14, 15, 16, 17, 18},
    {19, 20, 21, 22, 23, 24},
    {25, 26, 27, 28, 29, 0}};
void Solver(int puzzle[5][6]);
int main()
{
    /*
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
    Solver(puzzle);
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
    std::chrono::milliseconds millisecond =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Time Cost: " << millisecond.count() << " ms\n";*/
    std::cout << "test" << std::endl;
}