#pragma once
#include <vector>
#include "pieces.hpp"

class Board {
private:
    std::vector<std::vector<Piece*>> grid;
    std::pair<int, int> findKing(PieceColor color) const;


public:
    Board();
    ~Board();

    void initialize();
    void display() const;
    bool movePiece(int fromX, int fromY, int toX, int toY);
    bool isInCheck(PieceColor color) const;
    bool isCheckmate(PieceColor color);
    std::vector<std::pair<int, int>> getAllValidMoves(int x, int y);
    void setPiece(int x, int y, Piece* p);
    Piece* getPiece(int x, int y) const;
    bool isStalemate(PieceColor color);


};



