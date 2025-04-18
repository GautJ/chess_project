#include "board.hpp"
#include <iostream>

Board::Board() {
    grid.resize(8, std::vector<std::optional<Piece>>(8, std::nullopt));
    initialize();
}

void Board::initialize() {
    for (int i = 0; i < 8; ++i) {
        grid[1][i] = Piece(PieceType::Pawn, PieceColor::Black);
        grid[6][i] = Piece(PieceType::Pawn, PieceColor::White);
    }
}

void Board::display() const {
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j].has_value()) {
                char c;
                switch (grid[i][j]->type) {
                    case PieceType::Pawn: c = 'P'; break;
                    default: c = '?'; break;
                }
                if (grid[i][j]->color == PieceColor::Black)
                    c = tolower(c);
                std::cout << c << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
}
