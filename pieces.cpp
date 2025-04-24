#include "pieces.hpp"

// Fonction utilitaire : vérifie si une case est sur le plateau
static bool inBounds(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// ===== Pawn =====

char Pawn::getSymbol() const {
    return (color == PieceColor::White) ? 'P' : 'p';
}

// Gère les mouvements valides d’un pion (avance + capture)
std::vector<std::pair<int, int>> Pawn::getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const {
    std::vector<std::pair<int, int>> moves;
    int dir = (color == PieceColor::White) ? -1 : 1;

    // Avancer d’une case
    int oneStep = x + dir;
    if (inBounds(oneStep, y) && board[oneStep][y] == nullptr) {
        moves.emplace_back(oneStep, y);

        // Avancer de deux si jamais bougé
        int twoStep = x + 2 * dir;
        if (!hasMoved && inBounds(twoStep, y) && board[twoStep][y] == nullptr) {
            moves.emplace_back(twoStep, y);
        }
    }

    // Captures diagonales
    for (int dy = -1; dy <= 1; dy += 2) {
        int nx = x + dir;
        int ny = y + dy;
        if (inBounds(nx, ny) && board[nx][ny] != nullptr &&
            board[nx][ny]->getColor() != color) {
            moves.emplace_back(nx, ny);
        }
    }

    return moves;
}

// ===== Rook =====

char Rook::getSymbol() const {
    return (color == PieceColor::White) ? 'R' : 'r';
}

// Déplacements en lignes et colonnes
std::vector<std::pair<int, int>> Rook::getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const {
    std::vector<std::pair<int, int>> moves;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (int dir = 0; dir < 4; ++dir) {
        int nx = x + dx[dir], ny = y + dy[dir];
        while (inBounds(nx, ny)) {
            if (board[nx][ny] == nullptr) {
                moves.emplace_back(nx, ny);
            } else {
                if (board[nx][ny]->getColor() != color)
                    moves.emplace_back(nx, ny);
                break;
            }
            nx += dx[dir];
            ny += dy[dir];
        }
    }

    return moves;
}

// ===== Knight =====

char Knight::getSymbol() const {
    return (color == PieceColor::White) ? 'N' : 'n';
}

// Les 8 déplacements possibles en L
std::vector<std::pair<int, int>> Knight::getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const {
    std::vector<std::pair<int, int>> moves;
    int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i], ny = y + dy[i];
        if (inBounds(nx, ny) && (!board[nx][ny] || board[nx][ny]->getColor() != color)) {
            moves.emplace_back(nx, ny);
        }
    }
    return moves;
}

// ===== Bishop =====

char Bishop::getSymbol() const {
    return (color == PieceColor::White) ? 'B' : 'b';
}

// Se déplace uniquement en diagonales
std::vector<std::pair<int, int>> Bishop::getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const {
    std::vector<std::pair<int, int>> moves;
    int dx[] = {1, 1, -1, -1};
    int dy[] = {1, -1, 1, -1};

    for (int dir = 0; dir < 4; ++dir) {
        int nx = x + dx[dir], ny = y + dy[dir];
        while (inBounds(nx, ny)) {
            if (board[nx][ny] == nullptr) {
                moves.emplace_back(nx, ny);
            } else {
                if (board[nx][ny]->getColor() != color)
                    moves.emplace_back(nx, ny);
                break;
            }
            nx += dx[dir];
            ny += dy[dir];
        }
    }
    return moves;
}

// ===== Queen =====

char Queen::getSymbol() const {
    return (color == PieceColor::White) ? 'Q' : 'q';
}

// Combine rook + bishop (lignes, colonnes, diagonales)
std::vector<std::pair<int, int>> Queen::getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const {
    std::vector<std::pair<int, int>> moves;
    Rook tempR(color);
    Bishop tempB(color);
    auto rookMoves = tempR.getLegalMoves(x, y, board);
    auto bishopMoves = tempB.getLegalMoves(x, y, board);
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    return moves;
}

// ===== King =====

char King::getSymbol() const {
    return (color == PieceColor::White) ? 'K' : 'k';
}

// Se déplace d’une case dans toutes les directions + roque
std::vector<std::pair<int, int>> King::getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const {
    std::vector<std::pair<int, int>> moves;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (inBounds(nx, ny) && (!board[nx][ny] || board[nx][ny]->getColor() != color)) {
                moves.emplace_back(nx, ny);
            }
        }
    }

    // Roque : roi + tour n’ont pas bougé + pas de pièces entre eux
    if (!hasMoved) {
        int row = (color == PieceColor::White) ? 7 : 0;

        // Petit roque
        if (board[row][7] && dynamic_cast<Rook*>(board[row][7]) &&
            board[row][7]->getColor() == color &&
            static_cast<Rook*>(board[row][7])->canCastle()) {
            if (!board[row][5] && !board[row][6]) {
                moves.emplace_back(row, 6);
            }
        }

        // Grand roque
        if (board[row][0] && dynamic_cast<Rook*>(board[row][0]) &&
            board[row][0]->getColor() == color &&
            static_cast<Rook*>(board[row][0])->canCastle()) {
            if (!board[row][1] && !board[row][2] && !board[row][3]) {
                moves.emplace_back(row, 2);
            }
        }
    }

    return moves;
}
