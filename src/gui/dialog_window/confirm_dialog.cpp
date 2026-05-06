#include "confirm_dialog.h"

Chess::Gui::ConfirmDialog::ConfirmDialog(
  const std::string &title, sf::Vector2f size, sf::Color backgroundColor, 
  sf::Color textColor, unsigned int fontSize
) : title(allTextures.getFont()), confirm(), cancel() 
{
  this->title.setString(title);
  
  shape.setFillColor(backgroundColor);
  shape.setSize(size);
}