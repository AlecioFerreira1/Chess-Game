#include "simple_button.h"

Chess::Gui::SimpleButton::SimpleButton(
  const std::string &label, sf::Vector2f size, sf::Color backgroundColor, 
  sf::Color labelColor, unsigned int fontSize) : label(allTextures.getFont())
{
  this->label.setString(label);
  this->label.setCharacterSize(fontSize);
  this->label.setFillColor(labelColor);
  this->baseSize = size;
  shape.setFillColor(backgroundColor);
  shape.setSize(size);
}

void Chess::Gui::SimpleButton::draw(sf::RenderWindow &window){
  sf::Vector2u windowSize = window.getSize();

  sf::Vector2f scale = {
    windowSize.x / Chess::Config::windowWidth, 
    windowSize.y / Chess::Config::windowHeight
  };

  shape.setSize({baseSize.x * scale.x, baseSize.y * scale.y});
  shape.setOutlineThickness(2);

  shape.setPosition({
    (windowSize.x / 2) - (baseSize.x * scale.x / 2), 
    (windowSize.y / 2) - (baseSize.y * scale.y / 2)
  });

  label.setCharacterSize((unsigned int) (0.2 * baseSize.x * scale.x));

  label.setPosition({
    (windowSize.x / 2) - (label.getLocalBounds().size.x / 2), 
    (windowSize.y / 2) - (label.getLocalBounds().size.y / 2), 
  });

  window.draw(shape);
  window.draw(label);
}