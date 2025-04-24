#pragma once
#include <vector>
#include <utility>

enum class PieceColor { White, Black };

class Piece {
protected:
    PieceColor color;

public:
    Piece(PieceColor c) : color(c) {}
    virtual ~Piece() = default;

    PieceColor getColor() const { return color; }
    virtual char getSymbol() const = 0;
    virtual std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const = 0;
};

class Pawn : public Piece {
    private:
        mutable bool hasMoved = false;
    
    public:
        Pawn(PieceColor c) : Piece(c) {}
    
        char getSymbol() const override;
        std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const override;
    
        void markMoved() const { hasMoved = true; }
    };
    

class Knight : public Piece {
public:
    Knight(PieceColor c) : Piece(c) {}
    char getSymbol() const override;
    std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const override;
};

class Bishop : public Piece {
public:
    Bishop(PieceColor c) : Piece(c) {}
    char getSymbol() const override;
    std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const override;
};

class Queen : public Piece {
public:
    Queen(PieceColor c) : Piece(c) {}
    char getSymbol() const override;
    std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const override;
};

class King : public Piece {
    private:
        mutable bool hasMoved = false;
    
    public:
        King(PieceColor c) : Piece(c) {}
    
        char getSymbol() const override;
        std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const override;
    
        void markMoved() const { hasMoved = true; }
        bool canCastle() const { return !hasMoved; }
    };
    

class Rook : public Piece {
    private:
        mutable bool hasMoved = false;
    
    public:
        Rook(PieceColor c) : Piece(c) {}
    
        char getSymbol() const override;
        std::vector<std::pair<int, int>> getLegalMoves(int x, int y, const std::vector<std::vector<Piece*>>& board) const override;
    
        void markMoved() const { hasMoved = true; }
        bool canCastle() const { return !hasMoved; }
    };
        
    