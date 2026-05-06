#pragma once
#include <vector>
#include <list>
#include <SFML/System/Vector2.hpp>

#include "types.h"

struct Vec2{ 
  int row;
  int col;
  
  static sf::Vector2i toVector2i(Vec2 coords); 
};

namespace Chess::Utils{
  inline bool coordsInsideBounds(Vec2 coord){
    return coord.row >= 0 && coord.row <= 7 && coord.col >=0 && coord.col <= 7; 
  }

  std::vector<Vec2> genSlidingMoves(
    Vec2 pos, const BoardMatrix &boardMatrix, std::vector<Vec2> directions
  );

  sf::Vector2i toView(sf::Vector2i coord, bool flipped);
  sf::Vector2i toBoard(sf::Vector2i coord, bool flipped);
}   