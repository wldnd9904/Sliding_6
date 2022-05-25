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
    {1, 8, 2, 4, 5, 6},
    {7, 14, 3, 9, 10, 12},
    {13, 20, 15, 16, 11, 18},
    {25, 19, 21, 22, 17, 24},
    {26, 27, 28, 23, 29, 0}};

int inv[30] = {0};
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
    bool forward;
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
inline void push_slide_backward(
    std::priority_queue<Node, std::vector<Node>, myGreater> &q, const Node &n, int dir);
void Solver(int puzzle[5][6]);
void Solver(std::string state);
bool Solvable(const std::string &);

int main()
{
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
    Solver(puzzle);
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
    std::chrono::milliseconds millisecond =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Time Cost: " << millisecond.count() << " ms\n";
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
    if (volunteer.visited.count(volunteer.state))
        return;
    volunteer.visited.insert({volunteer.state, true});
    volunteer.hd = volunteer.heuristic + volunteer.depth;
    volunteer.depth += 1;
    q.push(volunteer);
}

// 퍼즐 슬라이드해서 우선순위 큐에 넣어줌
inline void push_slide_backward(
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
        if (inv[volunteer.state[pos] - 64] % 6 >= volunteer.x)
        {
            volunteer.heuristic--;
        }
        else
        {
            volunteer.heuristic++;
        }
        volunteer.x--;
        volunteer.trail.insert(volunteer.trail.begin(), 'R');
        break;
    case RIGHT:
        if (volunteer.x == 5)
            return;
        volunteer.state[pos] = volunteer.state[pos + 1];
        volunteer.state[pos + 1] = '@';
        if (inv[volunteer.state[pos] - 64] % 6 <= volunteer.x)
        {
            volunteer.heuristic--;
        }
        else
        {
            volunteer.heuristic++;
        }
        volunteer.x++;
        volunteer.trail.insert(volunteer.trail.begin(), 'L');
        break;
    case UP:
        if (volunteer.y == 0)
            return;
        volunteer.state[pos] = volunteer.state[pos - 6];
        volunteer.state[pos - 6] = '@';
        if (inv[volunteer.state[pos] - 64] / 6 >= volunteer.y)
        {
            volunteer.heuristic--;
        }
        else
        {
            volunteer.heuristic++;
        }
        volunteer.y--;
        volunteer.trail.insert(volunteer.trail.begin(), 'D');
        break;
    case DOWN:
        if (volunteer.y == 4)
            return;
        volunteer.state[pos] = volunteer.state[pos + 6];
        volunteer.state[pos + 6] = '@';
        if (inv[volunteer.state[pos] - 64] / 6 <= volunteer.y)
        {
            volunteer.heuristic--;
        }
        else
        {
            volunteer.heuristic++;
        }
        volunteer.y++;
        volunteer.trail.insert(volunteer.trail.begin(), 'U');
        break;
    }
    if (volunteer.visited.count(volunteer.state))
        return;
    volunteer.visited.insert({volunteer.state, true});
    volunteer.hd = volunteer.heuristic + volunteer.depth;
    volunteer.depth += 1;
    q.push(volunteer);
}

// 퍼즐 푸는 함수
void Solver(int puzzle[5][6])
{
    Solver(makePuzzle(puzzle));
}
void Solver(std::string state)
{
    if (!Solvable(state))
    {
        std::cout << "this puzzle is unsolvable." << std::endl;
        return;
    }
    std::priority_queue<Node, std::vector<Node>, myGreater> qF, qB;
    std::map<std::string, Node> visited;
    int pos;

    // start, dest node
    Node start, dest;
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
    start.forward = true;
    qF.push(start);

    dest.state = end;
    dest.trail = "";
    dest.x = 5;
    dest.y = 4;
    dest.heuristic = start.heuristic;
    dest.depth = 0;
    dest.forward = false;
    qB.push(dest);

    // make inverse matrix
    for (int i = 0; i < 30; i++)
    {
        inv[state[i] - 64] = i;
    }
    while (true)
    {
        // Forward
        int qFsize = qF.size();
        while (qFsize-- > 0)
        {
            Node n = qF.top();
            qF.pop();
            if (n.heuristic == 0)
            {
                std::cout << "clear! \n trail: " << n.trail << "\n movements: " << n.depth << std::endl;
                return;
            }
            if (visited.count(n.state))
            {
                if (!visited[n.state].forward)
                {
                    std::cout << "bidirectional clear! \n trail: " << n.trail << visited[n.state].trail << "\n movements: " << n.depth + visited[n.state].depth << std::endl;
                    return;
                }
                else if (visited[n.state].depth < n.depth)
                    visited[n.state] = n;
            }
            else
                visited.insert({n.state, n});
            push_slide(qF, n, LEFT);
            push_slide(qF, n, RIGHT);
            push_slide(qF, n, UP);
            push_slide(qF, n, DOWN);
        }
        // Backward
        int qBsize = qB.size();
        while (qBsize-- > 0)
        {
            Node n = qB.top();
            qB.pop();
            if (n.heuristic == 0)
            {
                std::cout << "clear! \n trail: " << n.trail << "\n movements: " << n.depth << std::endl;
                return;
            }
            if (visited.count(n.state))
            {
                if (visited[n.state].forward)
                {
                    std::cout << "bidirectional clear! \n trail: " << n.trail << visited[n.state].trail << "\n movements: " << n.depth + visited[n.state].depth << std::endl;
                    return;
                }
                else if (visited[n.state].depth < n.depth)
                    visited[n.state] = n;
            }
            else
                visited.insert({n.state, n});
            push_slide_backward(qB, n, LEFT);
            push_slide_backward(qB, n, RIGHT);
            push_slide_backward(qB, n, UP);
            push_slide_backward(qB, n, DOWN);
        }
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
    return inversion % 2 == 0;
}