#pragma once

#include <SFML/Graphics.hpp>
#include "../game/game.h"
#include "../config/config.h"
#include "../render/render.h"
#include "board_interaction_controller.h"
#include "../audio/sound_manager.h"
#include "../gui/dialog_window/confirm_dialog.h"

namespace Chess::Core{
  enum class Screen{
    Playing, GameOverDialog
  };

  class GameApp{
    private:
    sf::RenderWindow window;
    Game game;
    Render::BoardRenderer boardRenderer;
    Render::PieceRenderer pieceRenderer;
    Render::MoveIndicatorRenderer moveIndicatorRenderer;
    SelectionState selectionState;
    BoardInteractionController interactionController;
    Audio::SoundManager soundManager;
    Screen currentScreen = Screen::Playing;
    Gui::ConfirmDialog gameOverScreen;

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