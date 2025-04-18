#pragma once

enum class PieceType {
    King, Queen, Rook, Bishop, Knight, Pawn
};

enum class PieceColor {
    White, Black
};

struct Piece {
    PieceType type;
    PieceColor color;

    Piece(PieceType t, PieceColor c) : type(t), color(c) {}
};
