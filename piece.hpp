#pragma once

enum class PieceType {
    Roi, Dame, Tour, Fou, Cavalier, Pion
};

enum class PieceColor {
    Blanc, Noir
};

struct Piece {
    PieceType type;
    PieceColor couleur;

    Piece(PieceType t, PieceColor c) : type(t), couleur(c) {}
};
