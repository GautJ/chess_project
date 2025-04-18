#pragma once
#include <vector>
#include <optional>
#include "piece.hpp"

class Board {
private:
    std::vector<std::vector<std::optional<Piece>>> grid;

public:
    Board();

    void display() const;
    void initialize();
};
