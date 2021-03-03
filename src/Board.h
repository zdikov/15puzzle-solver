#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <functional>

enum Direction {
    LEFT, RIGHT, UP, DOWN
};

class Board {
public:
    Board() :
        board_{{
                   {1, 2, 3, 4},
                   {5, 6, 7, 8},
                   {9, 10, 11, 12},
                   {13, 14, 15, 0}
               }} {}
    Board(const Board &other) : board_(other.board_), zero_index_(other.zero_index_){};

    Board &operator=(const Board &other) {
        board_ = other.board_;
        zero_index_ = other.zero_index_;
        return *this;
    }

    static constexpr std::pair<int, int> size = {4, 4};

    bool can_move(Direction direction) const {
        auto position = moved_zero_index(direction);
        return position.first >= 0 &&
               position.first < Board::size.first &&
               position.second >= 0 &&
               position.second < Board::size.second;
    }

    Board move(Direction direction) const {
        auto [new_i, new_j] = moved_zero_index(direction);
        auto [i, j] = zero_index_;
        Board new_board(*this);
        std::swap(new_board.board_[i][j], new_board.board_[new_i][new_j]);
        new_board.zero_index_ = {new_i, new_j};
        return new_board;
    }

    const std::array<uint8_t, size.second> &operator[](size_t index) const {
        return board_[index];
    }
    std::array<uint8_t, size.second> &operator[](size_t index) {
        return board_[index];
    }

    std::array<uint8_t, size.first * size.second> flatten() const;

    friend std::istream &operator>>(std::istream &in, Board &board);

    bool solvable() const;

    bool operator==(const Board &other) const;

    bool operator!=(const Board &other) const {
        return !((*this) == other);
    }

    struct Hash {
        size_t operator()(const Board &board) const {
            auto flattened = board.flatten();
            std::string str(flattened.begin(), flattened.end());
            return std::hash<std::string>{}(str);
        }
    };

    int heuristic() const;
private:
    std::array<std::array<uint8_t, size.second>, size.first> board_;
    std::pair<int, int> zero_index_ = {size.first - 1, size.second - 1};
    std::pair<int, int> moved_zero_index(Direction direction) const;
};

std::ostream &operator<<(std::ostream &out, const Board &board);