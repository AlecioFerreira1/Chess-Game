#pragma once
#include <vector>
#include "../../utils/types.h"
#include "../../utils/utils.h"

namespace Chess{
  class Piece{
    protected:
    Types::Piece type;
    Types::Color color;
    Vec2 position;
    int goFoward;
    bool alreadyMoved = false;
    bool pinned = false;
    
    public:
    Piece(
      Types::Piece type = Types::Piece::None, 
      Types::Color color = Types::Color::None,
      Vec2 position = {-1, -1}
    );
    virtual ~Piece() = default;
    Types::Piece getType() const { return type; }
    Vec2 getPosition() const { return position; }
    Types::Color getColor() const { return color; }
    const int getOrientation() { return color == Types::Color::Black ? 1 : -1; } 
    void setPosition(Vec2 pos) { this->position = pos; } 
    void moved() { this->alreadyMoved = true; }
    bool wasMoved() { return alreadyMoved; }
    virtual std::vector<Vec2> getMoves(const BoardMatrix &boardMatrix) = 0;
    // virtual bool isPinned(const BoardMatrix &boardMatrix);
    void invertOrientation() { goFoward *= -1; };
  };

  class Pawn : public Chess::Piece{
    private:
    public:
    Pawn(Types::Color color, Vec2 position = {-1, -1});
    std::vector<Vec2> getMoves(const BoardMatrix &boardMatrix) override;
    std::vector<Vec2> getMoves(const BoardMatrix &boardMatrix, const std::list<std::tuple<Vec2, Vec2>> &plays);
    bool enPassant(const BoardMatrix &boardMatrix, const std::list<std::tuple<Vec2, Vec2>> &plays);
    bool promotion(const BoardMatrix &boardMatrix);
  };

  class Knight : public Chess::Piece{
    private:
    public:
    Knight(Types::Color color, Vec2 position = Vec2{-1, -1});
    std::vector<Vec2> getMoves(const BoardMatrix &boardMatrix) override;
  };

  class Rook : public Chess::Piece{
    private:
    public:
    Rook(Types::Color color, Vec2 position = Vec2{-1, -1});
    std::vector<Vec2> getMoves(const BoardMatrix &boardMatrix) override;
  };

  class Bishop : public Chess::Piece{
    private:
    public:
    Bishop(Types::Color color, Vec2 position = Vec2{-1, -1});
    std::vector<Vec2> getMoves(const BoardMatrix &boardMatrix) override;
  };

  class Queen : public Chess::Piece{
    private:
    public:
    Queen(Types::Color color, Vec2 position = Vec2{-1, -1});
    std::vector<Vec2> getMoves(const BoardMatrix &boardMatrix) override;
  };

  class King : public Chess::Piece{
    private:
    std::vector<Vec2> getCastlingMoviments(const BoardMatrix &boardMatrix);
    bool isSquareAttackedLinearly(Vec2 move, const BoardMatrix &boardMatrix);
    bool isSquareAttackedByPawn(Vec2 move, const BoardMatrix &boardMatrix);
    bool isSquareAttackedByKnight(Vec2 move, const BoardMatrix &boardMatrix);
    
    public:
    King(Types::Color color, Vec2 position = Vec2{-1, -1});
    std::vector<Vec2> getMoves(const BoardMatrix &boardMatrix) override;
    bool notAttacked(Vec2 move, const BoardMatrix &boardMatrix);
    bool castling(const BoardMatrix &boardMatrix);
    //bool isPinned(const BoardMatrix &boardMatrix) override { return false; }
  };
}