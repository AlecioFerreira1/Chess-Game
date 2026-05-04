#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "../utils/utils.h"

namespace Chess::Draw{
  class MoveIndicatorRenderer{
    private:
    struct Mark{
      sf::CircleShape shape;
      sf::Vector2i pos = {-1, -1};
    };

    Mark markStyle;
    std::vector<Mark> marks; 

    public:
    MoveIndicatorRenderer() = default;
    void setMoves(std::vector<sf::Vector2i> moves);
    void draw(sf::RenderWindow &renderWindow, float squareSize, sf::Vector2f boardPos, bool fliped = false);
    void clear();
  };
}