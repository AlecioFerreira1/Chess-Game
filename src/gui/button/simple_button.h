#include <SFML/Graphics.hpp>
#include <string.h>
#include "..\..\textures\load_textures.h"
#include "..\..\config\config.h"

namespace Chess::Gui{
  class SimpleButton{
    private:
    sf::Text label;
    sf::RectangleShape shape;
    sf::Vector2f baseSize;

    public:
    SimpleButton(
      const std::string label, sf::Vector2f size, sf::Color labelColor, 
      sf::Color backgroundColor, unsigned int fontSize
    );

    sf::Vector2f getSize() { return shape.getSize(); }
    sf::Vector2f getPosition() { return shape.getPosition(); }
    void setPosition(sf::Vector2f position);
    void draw(sf::RenderWindow &window);
    bool isHovered(sf::Vector2f mousePos);
  };
}