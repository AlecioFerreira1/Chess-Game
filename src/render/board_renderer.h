#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../utils/types.h"
#include "../textures/load_textures.h"

namespace Chess::Draw{
  class BoardRenderer{
    private:
    sf::RectangleShape square[8][8];
    float squareSize = 0.f, fontSize = 0.f;
    std::vector<sf::Text> columns;
    std::vector<sf::Text> rows;
    sf::Vector2f boardPos = {0, 0};

    public:
    BoardRenderer();
    float getSquareSize() { return squareSize; }
    sf::Vector2f getBoardPos () { return boardPos; }
    void draw(sf::RenderWindow &renderWindow, sf::Vector2f upLeft = {0.f, 0.f}, bool fliped = false);
    void select(sf::Vector2i coords);
    void deSelect(sf::Vector2i coords);
  };
}