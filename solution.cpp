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

/*int puzzle[5][6] = {
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 10, 11, 12},
    {13, 14, 15, 16, 0, 18},
    {19, 20, 21, 22, 17, 24},
    {25, 26, 27, 28, 23, 29}};
*/
int puzzle[5][6] = {
    {3, 7, 10, 0, 6, 16},
    {1, 21, 13, 9, 17, 11},
    {8, 19, 15, 5, 4, 22},
    {2, 27, 20, 28, 24, 12},
    {14, 25, 26, 29, 23, 18},
};
struct Node
{
    std::string state;
    std::string trail;
    std::map<std::string, bool> visited;
    int x;
    int y;
    int heuristic;
    int depth;
    int hd;
};
class myGreater
{
public:
    bool operator()(Node a, Node b)
    {
        return a.hd > b.hd;
    }
};
const std::string end = "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]@";
std::string makePuzzle(int puzzle[5][6]);
inline void push_slide(
    std::priority_queue<Node, std::vector<Node>, myGreater> &q, const Node &n, int dir);
void Solver(int puzzle[5][6]);
void Solver(std::string state);
bool Solvable(const std::string &);
std::string makeRandomPuzzle();
std::string makeRandomPuzzle(int);
void printPuzzle(std::string state);

int main()
{
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
    Solver(makeRandomPuzzle(i));
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
    std::chrono::milliseconds millisecond =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Time Cost: " << millisecond.count() << " ms\n";

    // std::string puzzleStr = makePuzzle(puzzle);
    // std::cout << puzzleStr << std::endl;
    //  Solver(puzzle);
    //   printPuzzle(makeRandomPuzzle(23));
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
inline void push_slide(
    std::priority_queue<Node, std::vector<Node>, myGreater> &q, const Node &n, int dir)
{
    Node volunteer(n);
    const int pos = volunteer.x + volunteer.y * 6;
    switch (dir)
    {
    case LEFT:
        if (volunteer.x == 0)
            return;
        volunteer.state[pos] = volunteer.state[pos - 1];
        volunteer.state[pos - 1] = '@';
        if ((volunteer.state[pos] - 65) % 6 >= volunteer.x)
        {
            volunteer.heuristic--;
        }
        else
        {
            volunteer.heuristic++;
        }
        volunteer.x--;
        volunteer.trail.push_back('L');
        break;
    case RIGHT:
        if (volunteer.x == 5)
            return;
        volunteer.state[pos] = volunteer.state[pos + 1];
        volunteer.state[pos + 1] = '@';
        if ((volunteer.state[pos] - 65) % 6 <= volunteer.x)
        {
            volunteer.heuristic--;
        }
        else
        {
            volunteer.heuristic++;
        }
        volunteer.x++;
        volunteer.trail.push_back('R');
        break;
    case UP:
        if (volunteer.y == 0)
            return;
        volunteer.state[pos] = volunteer.state[pos - 6];
        volunteer.state[pos - 6] = '@';
        if ((volunteer.state[pos] - 65) / 6 >= volunteer.y)
        {
            volunteer.heuristic--;
        }
        else
        {
            volunteer.heuristic++;
        }
        volunteer.y--;
        volunteer.trail.push_back('U');
        break;
    case DOWN:
        if (volunteer.y == 4)
            return;
        volunteer.state[pos] = volunteer.state[pos + 6];
        volunteer.state[pos + 6] = '@';
        if ((volunteer.state[pos] - 65) / 6 <= volunteer.y)
        {
            volunteer.heuristic--;
        }
        else
        {
            volunteer.heuristic++;
        }
        volunteer.y++;
        volunteer.trail.push_back('D');
        break;
    }
    if (volunteer.visited.find(volunteer.state) != volunteer.visited.end())
        return;
    volunteer.visited.insert({volunteer.state, true});
    volunteer.hd = volunteer.heuristic + volunteer.depth;
    volunteer.depth += 1;
    q.push(volunteer);
    // std::cout << "heuristic: " << volunteer.heuristic << std::endl;
    // printPuzzle(volunteer.state);
}

// 퍼즐 푸는 알고리즘
void Solver(int puzzle[5][6])
{
    Solver(makePuzzle(puzzle));
}
void Solver(std::string state)
{
    std::cout << state << std::endl;
    //  std::string state = makePuzzle(puzzle);
    if (!Solvable(state))
    {
        std::cout << "this puzzle is unsolvable." << std::endl;
        return;
    }
    std::priority_queue<Node, std::vector<Node>, myGreater> q;
    int pos;
    // int half;
    bool flag = true;

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
        int pivot = (start.state[i] - 65);
        int myX = i % 6, myY = i / 6, deX = pivot % 6, deY = pivot / 6;
        start.heuristic += abs(myX - deX) + abs(myY - deY);
    }
    q.push(start);

    while (!q.empty())
    {
        Node n = q.top();
        // std::cout << q.size() << ',' << n.depth << ',' << n.heuristic << std::endl;
        if (!n.heuristic)
        {
            std::cout << "clear! \n trail: " << n.trail << "\n movements: " << n.depth << "\n origin\'s heuristic: " << start.heuristic << std::endl;
            return;
        }
        q.pop();
        push_slide(q, n, LEFT);
        push_slide(q, n, RIGHT);
        push_slide(q, n, UP);
        push_slide(q, n, DOWN);
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
        if (pivot == 64)
        {
            inversion += i / 6;
            continue;
        }
        for (int j = i + 1; j < size; j++)
        {
            if (puzzle[j] != 64 && puzzle[j] < pivot)
                inversion++;
        }
    }
    return !(inversion & 1);
}

// 랜덤 퍼즐 생성
std::string makeRandomPuzzle()
{
    std::string output = end;
    std::random_device rd;
    std::default_random_engine rng(rd());
    shuffle(output.begin(), output.end(), rng);
    return output;
}

// 랜덤 퍼즐 생성2
std::string makeRandomPuzzle(int n)
{
    std::string state = end;
    int x = 5, y = 4, pos = 29;
    for (int i = 0; i < n;)
    {
        int dir = rand() % 4;
        switch (dir)
        {
        case LEFT:
            if (x == 0)
                continue;
            state[pos] = state[pos - 1];
            state[pos - 1] = '@';
            x--;
            pos--;
            break;
        case RIGHT:
            if (x == 5)
                continue;
            state[pos] = state[pos + 1];
            state[pos + 1] = '@';
            x++;
            pos++;
            break;
        case UP:
            if (y == 0)
                continue;
            state[pos] = state[pos - 6];
            state[pos - 6] = '@';
            y--;
            pos -= 6;
            break;
        case DOWN:
            if (y == 4)
                continue;
            state[pos] = state[pos + 6];
            state[pos + 6] = '@';
            y++;
            pos += 6;
            break;
        }
        i++;
        // printPuzzle(state);
    }
    return state;
}

// 출력
void printPuzzle(std::string state)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            std::cout << state[6 * i + j] - 64 << '\t';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}