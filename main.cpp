#include <iostream>
#include <string>
#include "board.hpp"

// Convertit une position comme "e2" en indices de tableau (row, col)
std::pair<int, int> parsePosition(const std::string& pos) {
    if (pos.length() != 2) return {-1, -1}; // Format invalide
    int col = pos[0] - 'a';
    int row = 8 - (pos[1] - '0'); // L'échiquier est affiché de haut en bas (8 → 0)
    if (col < 0 || col > 7 || row < 0 || row > 7) return {-1, -1};
    return {row, col};
}

int main() {
    Board board; // Plateau de jeu
    PieceColor currentPlayer = PieceColor::White; // Le joueur qui commence

    while (true) {
        board.display(); // Affiche l’état actuel du plateau

        // Demande le coup au joueur actuel
        std::cout << ((currentPlayer == PieceColor::White) ? "White" : "Black") << "'s move (e.g. e2 e4): ";
        std::string fromStr, toStr;
        std::cin >> fromStr >> toStr;

        // Conversion des positions saisies
        auto [fromX, fromY] = parsePosition(fromStr);
        auto [toX, toY] = parsePosition(toStr);

        if (fromX == -1 || toX == -1) {
            std::cout << "Invalid input format.\n";
            continue;
        }

        Piece* selected = board.getPiece(fromX, fromY);
        if (!selected) {
            std::cout << "No piece at " << fromStr << "\n";
            continue;
        }

        if (selected->getColor() != currentPlayer) {
            std::cout << "That's not your piece!\n";
            continue;
        }

        // Si le coup est valide, on le joue
        if (board.movePiece(fromX, fromY, toX, toY)) {
            PieceColor next = (currentPlayer == PieceColor::White) ? PieceColor::Black : PieceColor::White;

            // Vérifie si le prochain joueur est mat
            if (board.isCheckmate(next)) {
                std::cout << ((next == PieceColor::White) ? "White" : "Black") << " is checkmated. Game over!\n";
                break;
            }

            // Vérifie si le prochain joueur est pat
            if (board.isStalemate(next)) {
                std::cout << "Stalemate! Game is a draw.\n";
                break;
            }

            // Vérifie si le prochain joueur est en échec
            if (board.isInCheck(next)) {
                std::cout << ((next == PieceColor::White) ? "White" : "Black") << " is in check!\n";
            }

            currentPlayer = next;
        } else {
            std::cout << "Illegal move.\n";
        }
    }

    return 0;
}
