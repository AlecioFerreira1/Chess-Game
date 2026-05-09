#include "game_app.h"
#include <iostream>

Chess::Core::GameApp::GameApp() : 
  interactionController(
    game, selectionState, boardRenderer, 
    moveIndicatorRenderer, soundManager
  ),
  gameOverScreen(
    "Xeque-Mate!\nDeseja jogar novamente?", {300, 200}, 
    sf::Color::White, sf::Color::Black, 20
  )
{
  window = sf::RenderWindow(
    sf::VideoMode({Config::windowWidth, Config::windowWidth}), 
    Config::windowTitle, sf::Style::Default
  );

  window.setVerticalSyncEnabled(true);
  game.start();
  soundManager.playStartGameSound();
}

void Chess::Core::GameApp::run(){ 
  while(window.isOpen()){
    handleEvents();

    if(game.finished()){
      if(game.stalemate()){
        gameOverScreen.setTitle("Jogo Empatado!\nDeseja jogar novamente?");
      }

      else{
        gameOverScreen.setTitle("Xeque-Mate!\nDeseja jogar novamente?");
      }

      if(currentScreen == Screen::Playing){
        soundManager.playGameEndSound();
      }

      currentScreen = Screen::GameOverDialog;
      render();
    }

    render();
  }
}

void Chess::Core::GameApp::handleEvents(){
  while(std::optional<sf::Event> event = window.pollEvent()){
    if(event->is<sf::Event::Closed>())
      window.close();

    getInput(event);
  }
}

void Chess::Core::GameApp::getInput(std::optional<sf::Event>& event){
  if(event->is<sf::Event::KeyPressed>()){
    auto key = event->getIf<sf::Event::KeyPressed>();

    if(key->code == sf::Keyboard::Key::Escape){
      window.close();
    }
  }

  if(event->is<sf::Event::MouseButtonPressed>()){
    auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

    if(mouseEvent->button == sf::Mouse::Button::Left){
      sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
      sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
      
      if(currentScreen == Screen::Playing && boardClicked(worldPos)){
        interactionController.handleClick(worldPos);
      }

      if(currentScreen == Screen::GameOverDialog){
        if(gameOverScreen.getChoice(worldPos) == Gui::HoverType::Confirm){
          game.restart();
          soundManager.playStartGameSound();
          currentScreen = Screen::Playing;
          gameOverScreen.resetState(window);
        }

        else if(gameOverScreen.getChoice(worldPos) == Gui::HoverType::Cancel){
          game.end();
          window.close();
        }
      }
    }
  }
}

void Chess::Core::GameApp::render(){
  unsigned int w = window.getSize().x;
  unsigned int h = window.getSize().y;

  sf::Vector2f center = {
    (w / 2) - 4 * boardRenderer.getSquareSize(), 
    (h / 2) - 4 * boardRenderer.getSquareSize()
  };

  sf::View view(sf::FloatRect{
    {0.f, 0.f}, 
    {static_cast<float>(w), 
     static_cast<float>(h)}
  });

  bool flipSides = Config::enableAutoFlip && game.getPlayerTurn() == 2;
  
  window.clear(sf::Color::White);
  
  boardRenderer.draw(window, center, flipSides);
  renderPieces(flipSides);

  moveIndicatorRenderer.draw(
    window, boardRenderer.getSquareSize(), 
    boardRenderer.getBoardPos(), flipSides
  );

  if(currentScreen == Screen::GameOverDialog)
    gameOverScreen.draw(window);
  
  window.setView(view);
  window.display();
}

void Chess::Core::GameApp::renderPieces(bool fliped){
  BoardMatrix boardMatrix = game.getBoard().getInfo();

  for(int i = 0; i < 8; ++i){
    for(int j = 0; j < 8; ++j){
      if(boardMatrix[i][j] != nullptr){
        pieceRenderer.draw(
          window, 
          boardMatrix[i][j], 
          boardRenderer.getSquareSize(), 
          boardRenderer.getBoardPos(),
          fliped
        );
      }
    }
  }
}

bool Chess::Core::GameApp::boardClicked(sf::Vector2f mousePos){
  if(currentScreen == Screen::GameOverDialog) return false;

  float squareSize = boardRenderer.getSquareSize();
  sf::Vector2f upperBounds = boardRenderer.getBoardPos();

  sf::Vector2f lowerBounds = {
    8 * squareSize + upperBounds.x, 
    8 * squareSize + upperBounds.y
  };

  return (mousePos.x >= upperBounds.x && mousePos.x <= lowerBounds.x &&
          mousePos.y >= upperBounds.y && mousePos.y <= lowerBounds.y);
}