#include "Board.h"

#include <iomanip>
#include <cmath>

using namespace std;

const std::array<std::pair<int, int>, Board::size.first * Board::size.second> right_position = {{
              {3, 3},
              {0, 0}, {0, 1}, {0, 2}, {0, 3},
              {1, 0}, {1, 1}, {1, 2}, {1, 3},
              {2, 0}, {2, 1}, {2, 2}, {2, 3},
              {3, 0}, {3, 1}, {3, 2}
          }};

int Board::heuristic() const {
    int result = 0;
    for (int i = 0; i < size.first; ++i) {
        for (int j = 0; j < size.second; ++j) {
            result += abs(i - right_position[board_[i][j]].first) +
                abs(j - right_position[board_[i][j]].second);
        }
    }
    return result;
}

bool Board::operator==(const Board &other) const {
    for (int i = 0; i < size.first; ++i) {
        for (int j = 0; j < size.second; ++j) {
            if (board_[i][j] != other[i][j]) {
                return false;
            }
        }
    }
    return true;
}

ostream &operator<<(ostream &out, const Board &board) {
    for (int i = 0; i < Board::size.first; ++i) {
        for (const auto &item : board[i]) {
            out << setw(4) << static_cast<int>(item);
        }
        out << '\n';
    }
    return out;
}

istream &operator>>(istream &in, Board &board) {
    for (int i = 0; i < Board::size.first; ++i) {
        for (int j = 0; j < Board::size.second; ++j) {
            int number;
            in >> number;
            if (number == 0) {
                board.zero_index_ = {i, j};
            }
            board[i][j] = number;
        }
    }
    return in;
}

bool Board::solvable() const {
    int inversions = 0;
    auto flattened = flatten();
    for (size_t i = 0; i < flattened.size(); ++i) {
        if (flattened[i] != 0) {
            for (size_t j = 0; j < i; ++j) {
                if (flattened[j] > flattened[i]) {
                    ++inversions;
                }
            }
        }
    }
    return (inversions + zero_index_.first + 1) % 2 == 0;
}

array<uint8_t, Board::size.first * Board::size.second> Board::flatten() const {
    array<uint8_t, Board::size.first * Board::size.second> result;
    int index = 0;
    for (const auto &row : board_) {
        for (auto item : row) {
            result[index++] = item;
        }
    }
    return result;
}

pair<int, int> Board::moved_zero_index(Direction direction) const {
    switch (direction) {
        case Direction::UP:
            return {zero_index_.first - 1, zero_index_.second};
        case Direction::DOWN:
            return {zero_index_.first + 1, zero_index_.second};
        case Direction::LEFT:
            return {zero_index_.first, zero_index_.second - 1};
        case Direction::RIGHT:
            return {zero_index_.first, zero_index_.second + 1};
    }
    return {-1, -1};
}