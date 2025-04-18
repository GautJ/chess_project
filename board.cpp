#include "board.hpp"
#include <iostream>

Board::Board() {
    plateau.resize(8, std::vector<std::optional<Piece>>(8, std::nullopt));
    initialiser();
}

void Board::initialiser() {
    // Pour l’instant, on place juste les pions pour test
    for (int i = 0; i < 8; ++i) {
        plateau[1][i] = Piece(PieceType::Pion, PieceColor::Noir);
        plateau[6][i] = Piece(PieceType::Pion, PieceColor::Blanc);
    }
}

void Board::afficher() const {
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            if (plateau[i][j].has_value()) {
                char c;
                switch (plateau[i][j]->type) {
                    case PieceType::Pion: c = 'P'; break;
                    default: c = '?'; break;
                }
                if (plateau[i][j]->couleur == PieceColor::Noir)
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
