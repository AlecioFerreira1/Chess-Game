#pragma once

#include "../game/game.h"
#include "../render/render.h"
#include "selection_state.h"
#include "../config/config.h"
#include "../audio/sound_manager.h"

namespace Chess::Core{
  class BoardInteractionController{
    private:
    Game &game;   
    SelectionState &selectionState;   
    Render::BoardRenderer &boardRenderer;  
    Render::MoveIndicatorRenderer &moveIndicatorRenderer;
    Audio::SoundManager &soundManager;

    bool isEnemyPiece(Piece *piece) const;
    void clearSelection();
    sf::Vector2i convertMousePosToBoardCoords(sf::Vector2f mousePos);


    public:
    BoardInteractionController(Game &game, 
                               SelectionState &selectionState, 
                               Render::BoardRenderer &boardRenderer, 
                               Render::MoveIndicatorRenderer &moveIndicatorRenderer,
                               Audio::SoundManager& soundManager
    );

    void handleClick(sf::Vector2f mousePos);
    void handleSound(Chess::Types::GameEvent event);
  };
}