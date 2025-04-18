#pragma once
#include <vector>
#include <optional>
#include "piece.hpp"

class Board {
private:
    std::vector<std::vector<std::optional<Piece>>> plateau;

public:
    Board();

    void afficher() const;
    void initialiser();
};
