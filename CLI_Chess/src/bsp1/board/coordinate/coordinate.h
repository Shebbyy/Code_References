#pragma once

constexpr int COL_WIDTH  = 8;
constexpr int ROW_HEIGHT = 8;

struct boardcoordinate
{
    int x;
    int y;

    // != Operator wird nicht benötigt, da default einfach !(lhs == rhs) verwendet wird.
    bool operator ==(const boardcoordinate& value) const {
        return x == value.x && y == value.y;
    };

    boardcoordinate operator + (const boardcoordinate& rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    boardcoordinate &operator += (const boardcoordinate& rhs) {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    [[nodiscard]] bool is_in_board() const {
        return x >= 0
            && y >= 0
            && x < COL_WIDTH
            && y < ROW_HEIGHT;
    }
};
