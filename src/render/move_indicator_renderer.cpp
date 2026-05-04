#include "move_indicator_renderer.h"

void Chess::Draw::MoveIndicatorRenderer::setMoves(std::vector<sf::Vector2i> moves) { 
  for(sf::Vector2i move : moves){
    marks.push_back({sf::CircleShape(), move});
    marks[marks.size() - 1].shape.setFillColor(sf::Color(100.f, 100.f, 100.f, 100.f));
  } 
}

void Chess::Draw::MoveIndicatorRenderer::draw(
  sf::RenderWindow &renderWindow, float squareSize, sf::Vector2f boardPos, bool fliped
){
  for(Mark &mark : marks){
    mark.shape.setRadius(0.15 * squareSize);
    
    mark.shape.setOrigin({
      mark.shape.getLocalBounds().size.x / 2.f,
      mark.shape.getLocalBounds().size.y / 2.f
    });

    sf::Vector2i markPos = mark.pos;

    if(fliped)
      markPos = {markPos.x, 7 - markPos.y};

    mark.shape.setPosition({ 
      boardPos.x + markPos.x * squareSize + squareSize / 2.f,
      boardPos.y + markPos.y * squareSize + squareSize / 2.f
    });

    renderWindow.draw(mark.shape);
  }
}

void Chess::Draw::MoveIndicatorRenderer::clear(){
  marks.clear();
}