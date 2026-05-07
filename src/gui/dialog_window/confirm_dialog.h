#pragma once

#include <SFML/Graphics.hpp>
#include "..\button\simple_button.h"

namespace Chess::Gui{
  enum class HoverType{
    None, Window, Confirm, Cancel
  };

  class ConfirmDialog{
    private:
    sf::Text title;
    sf::RectangleShape shape;
    sf::Vector2f baseSize;
    SimpleButton confirm;
    SimpleButton cancel;
    sf::Cursor arrowCursor;
    sf::Cursor pointerCursor;

    void updateComponentsPositions(sf::Vector2u windowSize);

    public:
    ConfirmDialog(
      const std::string title, sf::Vector2f size, sf::Color backgroundColor, 
      sf::Color textColor, unsigned int fontSize
    );

    void draw(sf::RenderWindow &window);
    void setTitle(std::string title) { this->title.setString(title); }
    void handleHoverButtons(sf::RenderWindow &window);
    HoverType getChoice(sf::Vector2f mousePos);
  };
}