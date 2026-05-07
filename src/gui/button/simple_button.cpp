#include "simple_button.h"

Chess::Gui::SimpleButton::SimpleButton(
  const std::string label, sf::Vector2f size, sf::Color backgroundColor, 
  sf::Color labelColor, unsigned int fontSize) : label(allTextures.getFont())
{
  this->label.setString(label);
  this->label.setCharacterSize(fontSize);
  this->label.setFillColor(labelColor);
  this->baseSize = size;
  shape.setFillColor(backgroundColor);
  shape.setSize(size);
  shape.setPosition({0, 0});
}

void Chess::Gui::SimpleButton::draw(sf::RenderWindow &window){
  sf::Vector2u windowSize = window.getSize();

  sf::Vector2f scale = {
    (float) windowSize.x / (float) Chess::Config::windowWidth, 
    (float) windowSize.y / (float) Chess::Config::windowHeight
  };

  float minScale = std::min(scale.x, scale.y);

  shape.setSize({baseSize.x * minScale, baseSize.y * minScale});
  shape.setOutlineThickness(2);
  label.setCharacterSize((unsigned int) (0.6 * baseSize.y * scale.y));

  label.setPosition({
    (shape.getPosition().x) + (shape.getSize().x / 2) - (label.getLocalBounds().size.x / 2), 
    (shape.getPosition().y) + (shape.getSize().y / 2) - (label.getLocalBounds().size.y), 
  });

  window.draw(shape);
  window.draw(label);
}

void Chess::Gui::SimpleButton::setPosition(sf::Vector2f position){
  shape.setPosition(position);
}

bool Chess::Gui::SimpleButton::isHovered(sf::Vector2f mousePos){
  sf::Vector2f size = shape.getSize();
  sf::Vector2f upperBounds = shape.getPosition();
  sf::Vector2f lowerBounds = {upperBounds.x + size.x, upperBounds.y + size.y};

  return (mousePos.x >= upperBounds.x && mousePos.y >= upperBounds.y) &&
         (mousePos.x <= lowerBounds.x && mousePos.y <= lowerBounds.y);
}