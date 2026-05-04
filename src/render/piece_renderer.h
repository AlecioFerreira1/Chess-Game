#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "../textures/load_textures.h" 
#include "../entities/pieces/piece.h"
#include "../config/config.h"

namespace Chess::Draw{
  class PieceRenderer{
    private:
    std::vector<sf::Sprite> whitePieces; 
    std::vector<sf::Sprite> blackPieces; 

    public:
    PieceRenderer();
    void draw(
      sf::RenderWindow &renderWindow, Piece *&piece, float squareSize, 
      sf::Vector2f boardPos, bool fliped = false
    );
  };
}