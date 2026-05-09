#include "confirm_dialog.h"

namespace Chess::Gui{
  ConfirmDialog::ConfirmDialog(
    const std::string title, sf::Vector2f size, sf::Color backgroundColor, 
    sf::Color textColor, unsigned int fontSize
  ) 
  : title(allTextures.getFont()), 
    confirm("Sim", {0.1f * Config::windowWidth, 0.04f * Config::windowHeight}, 
            sf::Color::Black, sf::Color(122, 122, 122), 
            (unsigned int) (0.05 *Config::windowHeight)),
    cancel("Nao", {0.1f * Config::windowWidth, 0.04f * Config::windowHeight}, 
            sf::Color::Black, sf::Color(122, 122, 122), 
            (unsigned int) (0.05 *Config::windowHeight)),
    arrowCursor(sf::Cursor::Type::Arrow),
    pointerCursor(sf::Cursor::Type::Hand)
  {
    this->title.setString(title);
    this->title.setFillColor(textColor);
    this->title.setCharacterSize(fontSize);
    baseSize = size;
    shape.setFillColor(backgroundColor);
    shape.setSize(size);
  }

  void ConfirmDialog::draw(sf::RenderWindow &window){
    sf::Vector2u windowSize = window.getSize();

    sf::Vector2f scale = {
      (float) windowSize.x / (float) Chess::Config::windowWidth, 
      (float) windowSize.y / (float) Chess::Config::windowHeight
    };

    float minScale = std::min(scale.x, scale.y);

    title.setCharacterSize((unsigned int) (0.07f * baseSize.x * minScale));
    shape.setSize({baseSize.x * minScale, baseSize.y * minScale});
    shape.setOutlineThickness(2);

    updateComponentsPositions(windowSize);
    handleHoverButtons(window);

    window.draw(shape);
    window.draw(title);
    confirm.draw(window);
    cancel.draw(window);
  }

  void ConfirmDialog::updateComponentsPositions(sf::Vector2u windowSize){
    shape.setPosition({
      (float) ((windowSize.x / 2)) - (shape.getSize().x / 2), 
      (float) ((windowSize.y / 2)) - (shape.getSize().x / 2)
    });

    title.setPosition({
      shape.getPosition().x + (shape.getSize().x / 2) - (title.getLocalBounds().size.x / 2),
      (shape.getPosition().y) + (0.2f * shape.getSize().y)
    });

    confirm.setPosition({
      shape.getPosition().x + (shape.getSize().x / 4) - (confirm.getSize().x / 2),
      shape.getPosition().y + (0.7f * shape.getSize().y)
    });

    cancel.setPosition({
      shape.getPosition().x + (shape.getSize().x * 0.75f) - (cancel.getSize().x / 2),
      shape.getPosition().y + (0.7f * shape.getSize().y)
    });
  }

  void ConfirmDialog::handleHoverButtons(sf::RenderWindow &window){
    sf::Vector2 mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool hasHovered = confirm.isHovered(mousePos) || cancel.isHovered(mousePos);

    window.setMouseCursor(hasHovered ? pointerCursor : arrowCursor);
  }

  HoverType ConfirmDialog::getChoice(sf::Vector2f mousePos){
    sf::Vector2f windowUpperBounds = shape.getPosition();

    sf::Vector2f windowLowerBounds = {
      windowUpperBounds.x + shape.getSize().x, 
      windowUpperBounds.y + shape.getSize().y
    };

    if(confirm.isHovered(mousePos)) return HoverType::Confirm;
    else if(cancel.isHovered(mousePos)) return HoverType::Cancel;
    
    else if((mousePos.x >= windowUpperBounds.x && mousePos.y >= windowUpperBounds.y) &&
           (mousePos.x <= windowLowerBounds.x && mousePos.y <= windowLowerBounds.y))
           return HoverType::Window;

    return HoverType::None;
  }

  void ConfirmDialog::resetState(sf::RenderWindow &window){
    window.setMouseCursor(arrowCursor);
  }
}