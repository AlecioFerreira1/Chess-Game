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
}

sf::Vector2i Vec2::toVector2i(Vec2 coords){
  return {coords.col, coords.row};
}

sf::Vector2i Chess::Utils::toView(sf::Vector2i coord, bool flipped) {
  if(!flipped) return coord;
  return {7 - coord.x, 7 - coord.y};
}

sf::Vector2i Chess::Utils::toBoard(sf::Vector2i coord, bool flipped) {
  if(!flipped) return coord;
  return {7 - coord.x, 7 - coord.y};
}