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
      const std::string &label, sf::Vector2f windowRatio, sf::Color labelColor, 
      sf::Color backgroundColor, unsigned int fontSize
    );

    void draw(sf::RenderWindow &window);
  };
}