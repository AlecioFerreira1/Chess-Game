#include "move_indicator_renderer.h"

void Chess::Render::MoveIndicatorRenderer::setMoves(std::vector<Vec2> moves) { 
  for(Vec2 move : moves){
    marks.push_back({sf::CircleShape(), Vec2::toVector2i(move)});
    marks[marks.size() - 1].shape.setFillColor(sf::Color(100.f, 100.f, 100.f, 100.f));
  } 
}

void Chess::Render::MoveIndicatorRenderer::draw(
  sf::RenderWindow &renderWindow, float squareSize, sf::Vector2f boardPos, bool fliped
){
  for(Mark &mark : marks){
    mark.shape.setRadius(0.15 * squareSize);
    
    mark.shape.setOrigin({
      mark.shape.getLocalBounds().size.x / 2.f,
      mark.shape.getLocalBounds().size.y / 2.f
    });

    sf::Vector2i markPos = Utils::toView(mark.pos, fliped);

    mark.shape.setPosition({ 
      boardPos.x + markPos.x * squareSize + squareSize / 2.f,
      boardPos.y + markPos.y * squareSize + squareSize / 2.f
    });

    renderWindow.draw(mark.shape);
  }
}

void Chess::Render::MoveIndicatorRenderer::clear(){
  marks.clear();
}