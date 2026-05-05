#include <algorithm>
#include "piece.h"

using namespace Chess;

Pawn::Pawn(Types::Color color, Vec2 position) : Piece(type, color, position) {
  this->type = Types::Piece::Pawn;
}

std::vector<Vec2> Pawn::getMoves(const BoardMatrix &boardMatrix){
  std::vector<Vec2> moves;
  return moves;
}

std::vector<Vec2> Pawn::getMoves(const BoardMatrix &boardMatrix, const std::list<std::tuple<Vec2, Vec2>> &plays){
  std::vector<Vec2> moves;
  Vec2 captures[2] = {
    {position.row + goFoward, position.col + goFoward},
    {position.row + goFoward, position.col - goFoward}
  };
  bool front1isEmpty = Utils::coordsInsideBounds({position.row + goFoward, position.col}) &&      
              boardMatrix[position.row + goFoward][position.col] == nullptr;
  
  bool front2isEmpty = Utils::coordsInsideBounds({position.row + (2 * goFoward), position.col}) && 
              boardMatrix[position.row + (2 * goFoward)][position.col] == nullptr;

  if(front1isEmpty){  // ONE SQUARE MOVE  
    moves.push_back({position.row + goFoward, position.col}); 
  }

  if(front1isEmpty && front2isEmpty && !alreadyMoved){  // TWO SQUARES MOVE (ONLY ON FIRST MOVE)
    moves.push_back({position.row + (2 * goFoward), position.col});
  }

  for(const Vec2& capture : captures){                 //  VERIFY CAPTURES 
    if(Utils::coordsInsideBounds(capture) &&          
      boardMatrix[capture.row][capture.col] != nullptr &&
      boardMatrix[capture.row][capture.col]->getColor() != this->color
    ){
      moves.push_back(capture);
    } 
  }

  if(enPassant(boardMatrix, plays)){
    Vec2 lastSquareOfMov = std::get<1>(plays.back());
    moves.push_back({lastSquareOfMov.row + goFoward, lastSquareOfMov.col});
  }

  return moves;
}

bool Chess::Pawn::enPassant(const BoardMatrix &boardMatrix, const std::list<std::tuple<Vec2, Vec2>> &plays){
  if(plays.empty()) return false;

  const std::tuple<Vec2, Vec2> lastMove = plays.back();
  const Vec2 from = std::get<0>(lastMove), to = std::get<1>(lastMove);
  const Types::Piece lastPieceMoved = boardMatrix[to.row][to.col]->getType();
  const Types::Color color = boardMatrix[to.row][to.col]->getColor();
  const bool twoSepMove = abs(to.row - from.row) == 2; 
  const bool isAdjacent = to.row == position.row  && abs(to.col - position.col) == 1;

  return lastPieceMoved == Types::Piece::Pawn && twoSepMove && 
         isAdjacent && color != this->color;
}

bool Chess::Pawn::promotion(const BoardMatrix &boardMatrix){
  return position.row == 0 || position.row == 7;
}