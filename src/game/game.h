#pragma once
#include "..\entities/board/board.h"
#include "..\entities/player/player.h"
#include "..\entities/pieces/piece.h"
#include "../utils/types.h"

#include <tuple>
#include <vector>

namespace Chess{
  class Game{ 
    private:
    Board board;
    Player players[2];
    short int playerTurn = 1;
    bool running = false; 
    Types::GameEvent status = Types::GameEvent::Start;
    std::list<std::tuple<Vec2, Vec2>> plays;
    Vec2 blackKingPos = {0, 4}, whiteKingPos = {7, 4};

    void generatePieces();
    void removePieces();
    bool momentaniumCheck(std::tuple<Vec2, Vec2> move);

    public:
    Game();
    Board &getBoard() { return board; }
    Player *getPlayers() { return players; }
    Types::GameEvent getStatus() const { return status; }
    void setStatus(Types::GameEvent status) { this->status = status; }
    short int getPlayerTurn () { return playerTurn; }
    std::list<std::tuple<Vec2, Vec2>> &getPlays() { return plays; }
    bool isRunning() const { return running; }
    void start(); 
    void end();
    void restart();
    bool invalidMove(Vec2 from, Vec2 to);
    void updateStatus();
    void changeTurn();
    bool check();
    bool checkmate();
    bool stalemate();
    bool finished();
    bool verifyStalemate();
    std::vector<Vec2> filterMovesToAvoidInconsistencies(Piece *pieceSelected);
  };
}