#include "board_interaction_controller.h"

Chess::Core::BoardInteractionController::BoardInteractionController(
  Game &game, 
  SelectionState &selectionState, 
  Render::BoardRenderer &boardRenderer, 
  Render::MoveIndicatorRenderer &moveIndicatorRenderer,
  Audio::SoundManager& soundManager
) : 
  game(game), selectionState(selectionState), 
  boardRenderer(boardRenderer), moveIndicatorRenderer(moveIndicatorRenderer), 
  soundManager(soundManager) { }

bool Chess::Core::BoardInteractionController::isEnemyPiece(Piece *piece) const{
  return (piece->getColor() == Types::Color::Black && game.getPlayerTurn() != 2) ||
         (piece->getColor() == Types::Color::White && game.getPlayerTurn() != 1);
}

void Chess::Core::BoardInteractionController::clearSelection(){
  const bool flipedCoords = Config::enableAutoFlip && game.getPlayerTurn() == 2;
  sf::Vector2i previusSelection = selectionState.getfrom().value();

  boardRenderer.deSelect(Utils::toBoard(previusSelection, flipedCoords));
  selectionState.clear();
  moveIndicatorRenderer.clear();
}

void Chess::Core::BoardInteractionController::handleClick(sf::Vector2f mousePos){
  const sf::Vector2i coords = convertMousePosToBoardCoords(mousePos);
  const BoardMatrix boardMatrix = game.getBoard().getInfo();
  const bool isPieceSelected = boardMatrix[coords.y][coords.x] != nullptr;

  if(isPieceSelected){
    if(isEnemyPiece(boardMatrix[coords.y][coords.x]) && !selectionState.getfrom().has_value()) return;

    if(selectionState.getfrom().has_value()){
      sf::Vector2i previusSquare = selectionState.getfrom().value();

      if(!isEnemyPiece(boardMatrix[coords.y][coords.x])) clearSelection();
      if(previusSquare.x == coords.x && previusSquare.y == coords.y) return;
    }

    if(!selectionState.getfrom().has_value()){
      const bool flipedCoords = Config::enableAutoFlip && game.getPlayerTurn() == 2;

      selectionState.select(coords);
      boardRenderer.select(Utils::toBoard(coords, flipedCoords));

      sf::Vector2i pieceSelectedCoords = selectionState.getfrom().value();
      Piece *pieceSelected = boardMatrix[pieceSelectedCoords.y][pieceSelectedCoords.x];

      moveIndicatorRenderer.setMoves(game.filterMovesToAvoidInconsistencies(pieceSelected));
      return;
    }
  }

  if(selectionState.getfrom().has_value()){
    selectionState.select(coords);

    sf::Vector2i from = selectionState.getfrom().value();
    sf::Vector2i to = selectionState.getTo().value();

    clearSelection();

    if(!game.invalidMove({from.y, from.x}, {to.y, to.x})){
      Player &player = game.getPlayers()[game.getPlayerTurn()];
      
      auto event = player.movePiece({from.y, from.x}, {to.y, to.x}, game.getBoard(), game.getPlays());

      game.setStatus(event);
      game.changeTurn();
      game.updateStatus();
      handleSound(event);
    }

    else handleSound(Types::GameEvent::IllegalMove);
  }
}

sf::Vector2i Chess::Core::BoardInteractionController::convertMousePosToBoardCoords(sf::Vector2f mousePos) {
  float squareSize = boardRenderer.getSquareSize();
  sf::Vector2f upperBounds = boardRenderer.getBoardPos();
  const bool flipedCoords = Config::enableAutoFlip && game.getPlayerTurn() == 2;
  
  sf::Vector2i boardCoords = {
    static_cast<int>((mousePos.x - upperBounds.x) / squareSize), 
    static_cast<int>((mousePos.y - upperBounds.y) / squareSize) 
  };

  return Utils::toView(boardCoords, flipedCoords);
}

void Chess::Core::BoardInteractionController::handleSound(Chess::Types::GameEvent event){
  event = event != Types::GameEvent::IllegalMove ? game.getStatus() : event; 

  if(event == Types::GameEvent::Capture) soundManager.playCaptureSound();
  else if(event == Types::GameEvent::Move) soundManager.playMovePieceSound();
  else if(event == Types::GameEvent::IllegalMove) soundManager.playIlegalMoveSound();
  else if(event == Types::GameEvent::Castle) soundManager.playCastleSound();
  else if(event == Types::GameEvent::Check) soundManager.playMoveCheckSound();
  else if(event == Types::GameEvent::Promotion) soundManager.playPromotionSound();
}