#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <queue>
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

int puzzle[5][6] = {
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 10, 11, 12},
    {13, 14, 15, 16, 17, 18},
    {19, 20, 21, 22, 23, 24},
    {25, 26, 27, 28, 0, 29}};
struct Node
{
    std::string state;
    std::string trail;
    std::map<std::string, bool> visited;
    int x;
    int y;
    int heuristic;
    int depth;
};
class myLess
{
public:
    bool operator()(Node a, Node b)
    {
        return a.heuristic < b.heuristic;
    }
};
const std::string end = "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]@";
std::string makePuzzle(int puzzle[5][6]);
void push_slide(
    std::priority_queue<Node, std::vector<Node>, myLess> &q, const Node &n, int dir);
void Solver(int puzzle[5][6]);
void Solver(std::string state);
bool Solvable(const std::string &);
std::string makeRandomPuzzle();

int main()
{
    /*
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
    Solver(puzzle);
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
    std::chrono::milliseconds millisecond =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Time Cost: " << millisecond.count() << " ms\n";*/
    std::string puzzleStr = makePuzzle(puzzle);
    std::cout << puzzleStr << std::endl;
    Solver(puzzle);
    while (true)
        Solver(makeRandomPuzzle());
}

// 5*6 int 배열을 문자열로 치환
std::string makePuzzle(int puzzle[5][6])
{
    std::string ret;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            ret.push_back(puzzle[i][j] + 64);
        }
    }
    return ret;
}

// 퍼즐 슬라이드해서 우선순위 큐에 넣어줌
void push_slide(
    std::priority_queue<Node, std::vector<Node>, myLess> &q, const Node &n, int dir)
{
    Node volunteer(n);
    if (n.depth > 5)
        return;
    const int pos = volunteer.x + volunteer.y * 6;
    switch (dir)
    {
    case LEFT:
        if (volunteer.x == 0)
            return;
        volunteer.state[pos] = volunteer.state[pos - 1];
        volunteer.state[pos - 1] = '@';
        volunteer.x--;
        if (volunteer.state[pos] - 65 <= pos)
            volunteer.heuristic--;
        else
            volunteer.heuristic++;
        volunteer.trail.push_back('L');
        break;
    case RIGHT:
        if (volunteer.x == 5)
            return;
        volunteer.state[pos] = volunteer.state[pos + 1];
        volunteer.state[pos + 1] = '@';
        volunteer.x++;
        if (volunteer.state[pos] - 65 >= pos)
            volunteer.heuristic--;
        else
            volunteer.heuristic++;
        volunteer.trail.push_back('R');
        break;
    case UP:
        if (volunteer.y == 0)
            return;
        volunteer.state[pos] = volunteer.state[pos - 6];
        volunteer.state[pos - 6] = '@';
        volunteer.y--;
        if (volunteer.state[pos] - 65 <= pos)
            volunteer.heuristic--;
        else
            volunteer.heuristic++;
        volunteer.trail.push_back('U');
        break;
    case DOWN:
        if (volunteer.y == 4)
            return;
        volunteer.state[pos] = volunteer.state[pos + 6];
        volunteer.state[pos + 6] = '@';
        volunteer.y++;
        if (volunteer.state[pos] - 65 >= pos)
            volunteer.heuristic--;
        else
            volunteer.heuristic++;
        volunteer.trail.push_back('D');
        break;
    }
    if (volunteer.visited.find(volunteer.state) != volunteer.visited.end())
        return;
    volunteer.visited.insert({volunteer.state, true});
    volunteer.heuristic += volunteer.depth;
    volunteer.depth += 1;
    q.push(volunteer);
}

// 퍼즐 푸는 알고리즘
void Solver(int puzzle[5][6])
{
    Solver(makePuzzle(puzzle));
}
void Solver(std::string state)
{
    std::cout << state << std::endl;
    // std::string state = makePuzzle(puzzle);
    if (!Solvable(state))
    {
        std::cout << "this puzzle is unsolvable." << std::endl;
        return;
    }
    std::priority_queue<Node, std::vector<Node>, myLess> q;
    int pos;

    Node start;
    start.state = state;
    start.trail = "";
    pos = state.find('@');
    start.x = pos % 6;
    start.y = pos / 6;
    start.heuristic = 0;
    start.depth = 0;
    for (int i = 0; i < 30; i++)
    {
        if (i == pos)
            continue;
        int tmp = i - (state[i] - 65);
        if (tmp < 0)
            tmp = -tmp;
        start.heuristic += tmp % 6 + tmp / 6;
    }
    q.push(start);

    while (true)
    {
        Node n = q.top();
        /* std::cout << q.size() << ':' << n.state << ',' << n.trail << ',' << n.depth << ", (" << n.x << ',' << n.y << ')' << std::endl
        << std::endl;
        ;*/
        if (n.state == end)
        {
            std::cout << "clear! \n trail: " << n.trail << std::endl;
            return;
        }
        q.pop();
        for (int i = 0; i < 4; i++)
            push_slide(q, n, i);
    }
}

// 주어진 5*6 퍼즐 문자열을 풀 수 있는지의 여부 반환
bool Solvable(const std::string &puzzle)
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

std::string makeRandomPuzzle()
{
    std::string output = end;
    std::random_device rd;
    std::default_random_engine rng(rd());
    shuffle(output.begin(), output.end(), rng);
    return output;
}