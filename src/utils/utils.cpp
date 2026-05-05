#include <algorithm>
#include "utils.h"
#include "../entities/pieces/piece.h"

namespace Chess::Utils{
  std::vector<Vec2> genSlidingMoves(   //FOR ROOK, BISHOP AND QUEEN MOVES
    Vec2 position, const BoardMatrix &boardMatrix, std::vector<Vec2> directions
  ){
    const Types::Color color = boardMatrix[position.row][position.col]->getColor();
    std::vector<Vec2> moves;
    std::vector<bool> validDirs(directions.size(), true);

    for(int i = 1; std::find(validDirs.begin(), validDirs.end(), true) != validDirs.end(); ++i){
      for(size_t j = 0; j < validDirs.size(); ++j){
        if(validDirs[j]){
          Vec2 move = {
            position.row + (i * directions[j].row), position.col + (i * directions[j].col)
          };

          bool possibleCapture = coordsInsideBounds(move) && 
                               boardMatrix[move.row][move.col] != nullptr &&
                               color != boardMatrix[move.row][move.col]->getColor(); 
          bool emptySquare = coordsInsideBounds(move) && 
                             boardMatrix[move.row][move.col] == nullptr;
          
          if(emptySquare || possibleCapture){
            moves.push_back(move);
            validDirs[j] = possibleCapture ? false : true;
          }

          else validDirs[j] = false;
        } 
      }
    }

    return moves; 
  }

  bool momentaniumCheck(std::tuple<Vec2, Vec2> move, Chess::BoardMatrix boardMatrix, std::list<std::tuple<Vec2, Vec2>> &plays){
    Vec2 from = std::get<0>(move), to = std::get<1>(move); 

    if(from.row == -1) return false; 

    Piece *pieceFrom = boardMatrix[from.row][from.col];
    const Types::Color color = pieceFrom->getColor();

    if(pieceFrom->getType() == Types::Piece::Pawn){
      Pawn *pawn = static_cast<Pawn *>(pieceFrom);

      if(pawn->enPassant(boardMatrix, plays)){
        boardMatrix[to.row - pawn->getOrientation()][to.col] = nullptr;
      }
    }

    boardMatrix[from.row][from.col] = nullptr;
    boardMatrix[to.row][to.col] = pieceFrom;

    if(pieceFrom->getType() == Types::Piece::King){
      King *king = static_cast<King *>(pieceFrom);
      return king->notAttacked(to, boardMatrix) == false;
    }

    for(int i = 0; i < 8; ++i){
      for(int j = 0; j < 8; ++j){
        if(
          boardMatrix[i][j] != nullptr && boardMatrix[i][j]->getColor() == color && 
          boardMatrix[i][j]->getType() == Types::Piece::King
        ){
          King *king = static_cast<King *>(boardMatrix[i][j]);
          return king->notAttacked(king->getPosition(), boardMatrix) == false;
        }
     }
   }

    return false;
  }
}

sf::Vector2i Vec2::toVector2i(Vec2 coords){
  return {coords.col, coords.row};
}