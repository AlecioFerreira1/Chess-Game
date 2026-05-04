#include "board_renderer.h"
#include "piece_renderer.h"
#include "colors.h"

Chess::Draw::BoardRenderer::BoardRenderer(){ 
  for(int i = 0; i < 8; ++i){
    rows.push_back(sf::Text(allTextures.getFont(), std::string(1, ('8' - i))));
    rows[i].setFillColor(i % 2 == 0 ? Colors::dark : Colors::light);
    
    columns.push_back(sf::Text(allTextures.getFont(), std::string(1, ('a' + i))));
    columns[i].setFillColor(i % 2 == 0 ? Colors::light : Colors::dark);

    for(int j = 0; j < 8; ++j){
      sf::Color color = (i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0) ?
                        Colors::light : Colors::dark;

      square[i][j].setFillColor(color);
    }
  }
}

void Chess::Draw::BoardRenderer::draw(sf::RenderWindow &renderWindow, sf::Vector2f upLeft, bool fliped){
  for(int i = 0; i < 8; ++i){
    rows[i].setString(fliped ? std::string(1, '1' + i) : std::string(1, '8' - i));
    columns[i].setString(fliped ? std::string(1, 'h' - i) : std::string(1, 'a' + i));
  }

  const sf::Vector2u winSize = renderWindow.getSize();
  squareSize = std::min(winSize.x, winSize.y) / 8.f;
  fontSize = 0.2 * squareSize;
  boardPos = upLeft;

  for(int i = 0; i < 8; ++i){
    rows[i].setCharacterSize(fontSize);
    rows[i].setPosition({5 + upLeft.x, 5 + upLeft.y + i * squareSize});

    columns[i].setCharacterSize(fontSize);
    columns[i].setPosition({
      (upLeft.x + squareSize - 5 - (0.7f * fontSize)) + i * squareSize, 
      (upLeft.y + squareSize - 10 - (0.9f * fontSize)) + 7 * squareSize
    });

    for(int j = 0; j < 8; ++j){
      square[i][j].setSize(sf::Vector2f(squareSize, squareSize));
      square[i][j].setPosition({j * squareSize + upLeft.x, i * squareSize + upLeft.y});
      renderWindow.draw(square[i][j]);
    }

    renderWindow.draw(rows[i]);           
  }

  for(int i = 0; i < 8; ++i){
    renderWindow.draw(columns[i]);
  }
}

void Chess::Draw::BoardRenderer::select(sf::Vector2i coords){
  sf::Color color = square[coords.y][coords.x].getFillColor() == Colors::dark ? 
                    Colors::darkSelection : Colors::lightSelecion; 

  square[coords.y][coords.x].setFillColor(color);
}

void Chess::Draw::BoardRenderer::deSelect(sf::Vector2i coords){
  sf::Color color = square[coords.y][coords.x].getFillColor() == Colors::darkSelection ? 
                    Colors::dark : Colors::light; 

  square[coords.y][coords.x].setFillColor(color);
}