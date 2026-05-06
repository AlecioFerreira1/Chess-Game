#include <SFML/Graphics.hpp>
#include "..\button\simple_button.h"

namespace Chess::Gui{
  class ConfirmDialog{
    private:
    sf::Text title;
    sf::RectangleShape shape;
    SimpleButton confirm;
    SimpleButton cancel;

    public:
    ConfirmDialog(
      const std::string &title, sf::Vector2f size, sf::Color backgroundColor, 
      sf::Color textColor, unsigned int fontSize
    );
  };
}