#pragma once

#include <SFML/Graphics.hpp>
#include "../game/game.h"
#include "../config/config.h"
#include "../render/render.h"
#include "board_interaction_controller.h"
#include "../audio/sound_manager.h"

namespace Chess::Core{
  class GameApp{
    private:
    sf::RenderWindow window;
    Game game;
    Draw::BoardRenderer boardRenderer;
    Draw::PieceRenderer pieceRenderer;
    Draw::MoveIndicatorRenderer moveIndicatorRenderer;
    SelectionState selectionState;
    BoardInteractionController interactionController;
    Audio::SoundManager soundManager;
    bool checkAlertPlayed = false;

    void handleEvents();
    void getInput(std::optional<sf::Event>& event);
    void render();
    void renderPieces(bool fliped = false);
    bool boardClicked(sf::Vector2f mousePos);

    public:
    GameApp();
    void run();
  };
}