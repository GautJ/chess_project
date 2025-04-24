#include "board.hpp"
#include <iostream>

// Constructeur : initialise un plateau vide (8x8 cases) puis place les pièces
Board::Board() {
    grid.resize(8, std::vector<Piece*>(8, nullptr));
    initialize();
}

// Destructeur : libère la mémoire de toutes les pièces allouées
Board::~Board() {
    for (auto& row : grid)
        for (auto& piece : row)
            delete piece;
}

// Accès sécurisé à une case du plateau
Piece* Board::getPiece(int x, int y) const {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return nullptr;
    return grid[x][y];
}

// Positionne une pièce à une coordonnée donnée
void Board::setPiece(int x, int y, Piece* p) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
        grid[x][y] = p;
}

// Cherche la position du roi d'une couleur donnée
std::pair<int, int> Board::findKing(PieceColor color) const {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (grid[i][j] && dynamic_cast<King*>(grid[i][j]) && grid[i][j]->getColor() == color)
                return {i, j};
    return {-1, -1}; // Roi non trouvé (erreur)
}

// Renvoie vrai si le roi de cette couleur est attaqué
bool Board::isInCheck(PieceColor color) const {
    auto [kingX, kingY] = findKing(color);
    if (kingX == -1) return true; // Sécurité

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (grid[i][j] && grid[i][j]->getColor() != color) {
                auto moves = grid[i][j]->getLegalMoves(i, j, grid);
                for (const auto& move : moves)
                    if (move.first == kingX && move.second == kingY)
                        return true;
            }
    return false;
}

// Retourne tous les coups légaux pour une pièce, en évitant les auto-échecs
std::vector<std::pair<int, int>> Board::getAllValidMoves(int x, int y) {
    std::vector<std::pair<int, int>> validMoves;
    Piece* piece = grid[x][y];
    if (!piece) return validMoves;

    auto possible = piece->getLegalMoves(x, y, grid);

    for (const auto& move : possible) {
        // On simule le déplacement
        Piece* backup = grid[move.first][move.second];
        grid[move.first][move.second] = piece;
        grid[x][y] = nullptr;

        bool stillInCheck = isInCheck(piece->getColor());

        // On annule la simulation
        grid[x][y] = piece;
        grid[move.first][move.second] = backup;

        if (!stillInCheck)
            validMoves.push_back(move);
    }

    return validMoves;
}

// Déplace une pièce si le coup est valide, gère promotion et roque
bool Board::movePiece(int fromX, int fromY, int toX, int toY) {
    if (!grid[fromX][fromY]) return false;

    Piece* movingPiece = grid[fromX][fromY];
    auto validMoves = getAllValidMoves(fromX, fromY);

    for (const auto& move : validMoves) {
        if (move.first == toX && move.second == toY) {
            delete grid[toX][toY]; // capture
            grid[toX][toY] = movingPiece;
            grid[fromX][fromY] = nullptr;

            // Cas du pion : marquer comme ayant bougé et gérer la promotion
            if (Pawn* pawn = dynamic_cast<Pawn*>(movingPiece)) {
                pawn->markMoved();
                if ((pawn->getColor() == PieceColor::White && toX == 0) ||
                    (pawn->getColor() == PieceColor::Black && toX == 7)) {
                    delete grid[toX][toY];
                    grid[toX][toY] = new Queen(pawn->getColor());
                    std::cout << "Promotion to Queen!\n";
                }
            }

            // Cas du roi : marquer comme ayant bougé et gérer le roque
            if (King* king = dynamic_cast<King*>(movingPiece)) {
                king->markMoved();

                // Petit roque
                if (fromY == 4 && toY == 6) {
                    Piece* rook = grid[toX][7];
                    grid[toX][5] = rook;
                    grid[toX][7] = nullptr;
                    if (Rook* r = dynamic_cast<Rook*>(rook)) r->markMoved();
                }

                // Grand roque
                if (fromY == 4 && toY == 2) {
                    Piece* rook = grid[toX][0];
                    grid[toX][3] = rook;
                    grid[toX][0] = nullptr;
                    if (Rook* r = dynamic_cast<Rook*>(rook)) r->markMoved();
                }
            }

            // Cas de la tour : marquer comme ayant bougé (important pour le roque)
            if (Rook* rook = dynamic_cast<Rook*>(movingPiece)) {
                rook->markMoved();
            }

            return true;
        }
    }

    return false;
}

// Renvoie vrai si le joueur est en échec ET ne peut rien faire → échec et mat
bool Board::isCheckmate(PieceColor color) {
    if (!isInCheck(color)) return false;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (grid[i][j] && grid[i][j]->getColor() == color) {
                auto validMoves = getAllValidMoves(i, j);
                if (!validMoves.empty())
                    return false;
            }

    return true;
}

// Renvoie vrai si le joueur N'EST PAS en échec mais ne peut rien faire → pat
bool Board::isStalemate(PieceColor color) {
    if (isInCheck(color)) return false;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (grid[i][j] && grid[i][j]->getColor() == color) {
                auto validMoves = getAllValidMoves(i, j);
                if (!validMoves.empty())
                    return false;
            }

    return true;
}

// Affiche le plateau dans la console (en mode ASCII)
void Board::display() const {
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            if (grid[i][j]) {
                std::cout << grid[i][j]->getSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}

// Place toutes les pièces à leur position initiale
void Board::initialize() {
    for (int i = 0; i < 8; ++i) {
        grid[1][i] = new Pawn(PieceColor::Black);
        grid[6][i] = new Pawn(PieceColor::White);
    }

    grid[0][0] = new Rook(PieceColor::Black); grid[0][7] = new Rook(PieceColor::Black);
    grid[7][0] = new Rook(PieceColor::White); grid[7][7] = new Rook(PieceColor::White);

    grid[0][1] = new Knight(PieceColor::Black); grid[0][6] = new Knight(PieceColor::Black);
    grid[7][1] = new Knight(PieceColor::White); grid[7][6] = new Knight(PieceColor::White);

    grid[0][2] = new Bishop(PieceColor::Black); grid[0][5] = new Bishop(PieceColor::Black);
    grid[7][2] = new Bishop(PieceColor::White); grid[7][5] = new Bishop(PieceColor::White);

    grid[0][3] = new Queen(PieceColor::Black);
    grid[7][3] = new Queen(PieceColor::White);
    grid[0][4] = new King(PieceColor::Black);
    grid[7][4] = new King(PieceColor::White);
}
