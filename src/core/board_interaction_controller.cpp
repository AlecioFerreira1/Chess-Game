#include "board_interaction_controller.h"

Chess::Core::BoardInteractionController::BoardInteractionController(
  Game &game, 
  SelectionState &selectionState, 
  Draw::BoardRenderer &boardRenderer, 
  Draw::MoveIndicatorRenderer &moveIndicatorRenderer,
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

  if(flipedCoords)
    previusSelection = {previusSelection.x, 7 - previusSelection.y};

  boardRenderer.deSelect(previusSelection);
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
      boardRenderer.select({coords.x, flipedCoords ? 7 - coords.y : coords.y});

      sf::Vector2i pieceSelectedCoords = selectionState.getfrom().value();
      Piece *pieceSelected = boardMatrix[pieceSelectedCoords.y][pieceSelectedCoords.x];

      moveIndicatorRenderer.setMoves(Vec2::vectorOfVec2ToVectorOfVector2i(
        game.filterMovesToAvoidInconsistencies(pieceSelected)
      ));

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
      
      auto event = player.movePiece({from.y, from.x}, {to.y, to.x}, game.getBoard());

      game.setStatus(event);
      game.updateStatus();
      game.changeTurn();
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

  return {
    boardCoords.x,
    flipedCoords ? 7 - boardCoords.y : boardCoords.y 
  };
}

void Chess::Core::BoardInteractionController::handleSound(Chess::Types::GameEvent event){
  event = event != Types::GameEvent::IllegalMove ? game.getStatus() : event; 

  switch (event){
    case Types::GameEvent::Capture:
      soundManager.playCaptureSound();
      break;
  
    case Types::GameEvent::Move:
      soundManager.playMovePieceSound();
      break;
    
    case Types::GameEvent::IllegalMove:
      soundManager.playIlegalMoveSound();
      break;

    case Types::GameEvent::Castle:
      soundManager.playCastleSound();
      break;
    
    case Types::GameEvent::Check:
      soundManager.playMoveCheckSound();
      break;
    
    case Types::GameEvent::Promotion:
      soundManager.playPromotionSound();
      break;

    default:
      break;
  }
}