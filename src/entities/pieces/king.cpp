#include <algorithm>
#include <array>
#include "piece.h"

using namespace Chess;

King::King(Types::Color color, Vec2 position) : Piece(type, color, position) {
  this->type = Types::Piece::King;
}

std::vector<Vec2> King::getMoves(const BoardMatrix &boardMatrix){
  std::vector<Vec2> moves;
  Vec2 directions[8] = {
    {goFoward, goFoward}, {goFoward, -goFoward}, {-goFoward, goFoward}, 
    {-goFoward, -goFoward}, {goFoward, 0}, {0, goFoward}, 
    {-goFoward, 0}, {0, -goFoward}
  };

  for(size_t j = 0; j < 8; ++j){
    Vec2 move = {
      position.row + directions[j].row, position.col + directions[j].col
    };

    bool possibleCapture = Utils::coordsInsideBounds(move) && 
                           boardMatrix[move.row][move.col] != nullptr &&
                           color != boardMatrix[move.row][move.col]->getColor(); 
    bool emptySquare = Utils::coordsInsideBounds(move) && 
                       ((boardMatrix[move.row][move.col] == nullptr));

    if((emptySquare || possibleCapture) && notAttacked(move, boardMatrix)){
      moves.push_back(move);
    }
  }

  for(Vec2& move : getCastlingMoviments(boardMatrix)){
    moves.push_back(move);
  }

  return moves;   
}

bool King::notAttacked(Vec2 move, const BoardMatrix &boardMatrix){
  return !(isSquareAttackedLinearly(move, boardMatrix) ||
         isSquareAttackedByKnight(move, boardMatrix) || 
         isSquareAttackedByPawn(move, boardMatrix));
}

std::vector<Vec2> King::getCastlingMoviments(const BoardMatrix &boardMatrix){
  std::vector<Vec2> moves;
  const bool check = !notAttacked(position, boardMatrix);

  if(check) return moves;

  const int rooksRow = color == Types::Color::Black ? 0 : 7;
  
  bool kingNotMoved = boardMatrix[rooksRow][4] != nullptr && 
                      !boardMatrix[rooksRow][4]->wasMoved();

  bool queenSide = boardMatrix[rooksRow][0] != nullptr && 
                   !boardMatrix[rooksRow][0]->wasMoved() &&
                  kingNotMoved;

  bool kingSide = boardMatrix[rooksRow][7] != nullptr && 
                  !boardMatrix[rooksRow][7]->wasMoved() && 
                  kingNotMoved; 

  for(int i = 0; i < 8; ++i){
    if(i == 0 || i == 4 || i == 7) continue;

    bool squareNotAttacked = notAttacked({rooksRow, i}, boardMatrix);

    if(i < 4){
      queenSide = queenSide && boardMatrix[rooksRow][i] == nullptr && squareNotAttacked;
    }

    else{
      kingSide = kingSide && boardMatrix[rooksRow][i] == nullptr && squareNotAttacked;
    }
  }

  if(queenSide) moves.push_back({rooksRow, 2});
  if(kingSide) moves.push_back({rooksRow, 6});

  return moves;
}

bool King::castling(const BoardMatrix &boardMatrix){
  return !getCastlingMoviments(boardMatrix).empty();
}

bool King::isSquareAttackedByPawn(Vec2 move, const BoardMatrix &boardMatrix){
  Vec2 possPawn[2] = {
    {move.row + getOrientation(), move.col + getOrientation()},
    {move.row + getOrientation(), move.col - getOrientation()},
  };

  for(const Vec2& square : possPawn){
    if(Utils::coordsInsideBounds(square) && boardMatrix[square.row][square.col] != nullptr){
      Piece *piece = boardMatrix[square.row][square.col];

      if(piece->getColor() != this->color && piece->getType() == Types::Piece::Pawn) 
        return true;
    }
  }

  return false;
}

bool King::isSquareAttackedByKnight(Vec2 move, const BoardMatrix &boardMatrix){
  const Vec2 KnightPossAttacks[8] = {
    {2 * goFoward, goFoward}, {2 * goFoward, -goFoward}, {goFoward, 2 * goFoward}, 
    {goFoward, -2 * goFoward},{-goFoward, 2 * goFoward}, {-goFoward, -2 * goFoward},
    {-2 * goFoward, goFoward}, {-2 * goFoward, -goFoward}
  };

  for(const Vec2& possAttack : KnightPossAttacks){
    Vec2 evaluate = {possAttack.row + move.row, possAttack.col + move.col};
    
    if(
      Utils::coordsInsideBounds(evaluate) && 
      boardMatrix[evaluate.row][evaluate.col] != nullptr &&
      boardMatrix[evaluate.row][evaluate.col]->getType() == Types::Piece::Knight &&
      boardMatrix[evaluate.row][evaluate.col]->getColor() != this->color
    ) { return true; }
  }

  return false;
}

bool King::isSquareAttackedLinearly(Vec2 move, const BoardMatrix &boardMatrix){
  std::array<bool, 8> validDirs = {true, true, true, true, true, true, true, true};

  const Vec2 directions[8] = {
    {goFoward, goFoward}, {goFoward, -goFoward}, {-goFoward, goFoward}, 
    {-goFoward, -goFoward}, {goFoward, 0}, {0, goFoward},
    {-goFoward, 0}, {0, -goFoward}, 
  };

  for(int i = 1; std::find(validDirs.begin(), validDirs.end(), true) != validDirs.end(); ++i){
    for(size_t j = 0; j < 8; ++j){
      Vec2 evaluate = {
        move.row + (i * directions[j].row), move.col + (i * directions[j].col)
      };

      if(Utils::coordsInsideBounds(evaluate) == false) {
        validDirs[j] = false; continue;
      }

      Piece *piece = boardMatrix[evaluate.row][evaluate.col];

      if(piece == nullptr || !validDirs[j]) continue;

      if(piece->getColor() == color || piece->getType() == Types::Piece::Pawn){
        validDirs[j] = false; continue;
      } 

      const bool enemyPieceIsQueen = piece->getType() == Types::Piece::Queen;
      const bool enemyPieceIsBishopWithDiagonalCapture = j < 4 &&
                                                   piece->getType() == Types::Piece::Bishop;
      const bool enemyPieceIsRook = j >= 4 && piece->getType() == Types::Piece::Rook;

      if(
        enemyPieceIsQueen || enemyPieceIsBishopWithDiagonalCapture || enemyPieceIsRook
      ) { return true; }

      validDirs[j] = false; 
    }
  }

  return false;
}