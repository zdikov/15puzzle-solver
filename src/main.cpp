#include "Board.h"

#include <chrono>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

unordered_map<Board, Board, Board::Hash> solve(const Board &start) {
    unordered_map<Board, Board, Board::Hash> parent;
    unordered_set<Board, Board::Hash> to_visit;
    unordered_map<Board, int, Board::Hash> dist_from_start;
    // score = moves number from the start board + heuristic
    unordered_map<Board, int, Board::Hash> score;
    priority_queue<Board, vector<Board>, function<bool(const Board &lhs, const Board &rhs)>> queue(
        [&score](const Board &lhs, const Board &rhs) {
            return score[lhs] > score[rhs];
        });
    Board goal;
    dist_from_start[start] = 0;
    queue.push(start);
    to_visit.insert(start);
    while (!queue.empty()) {
        auto current_board = queue.top();
        if (current_board == goal) {
            return parent;
        }
        queue.pop();
        to_visit.erase(current_board);
        for (auto direction : {Direction::LEFT, Direction::RIGHT, Direction::UP, Direction::DOWN}) {
            if (!current_board.can_move(direction)) {
                continue;
            }
            auto next_board = current_board.move(direction);
            int distance = dist_from_start[current_board] + 1;
            if (dist_from_start.find(next_board) != dist_from_start.end() &&
                dist_from_start[next_board] <= distance) {
                continue;
            }
            parent[next_board] = current_board;
            dist_from_start[next_board] = distance;
            score[next_board] = distance + next_board.heuristic();
            if (to_visit.find(next_board) == to_visit.end()) {
                queue.push(next_board);
                to_visit.insert(next_board);
            }
        }
    }
    return parent;
}

int main() {
    Board start_board;
    ios::sync_with_stdio(false);
    cin >> start_board;
    if (!start_board.solvable()) {
        cout << "Solution does not exist\n";
        return 0;
    }
    auto start_time = chrono::steady_clock::now();
    auto parent = solve(start_board);
    auto finish_time = chrono::steady_clock::now();
    Board goal;
    cout << goal;
    auto prev = parent[goal];
    int count = 1;
    while (prev != start_board) {
        cout << "-------------------\n";
        cout << prev;
        prev = parent[prev];
        ++count;
    }
    cout << "-------------------\n";
    cout << start_board;
    cout << "-------------------\n";
    cout << "Solved in " << count << " moves\n";
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(finish_time - start_time).count() << " ms" << endl;
}
