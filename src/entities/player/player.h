#pragma once
#include <string>
#include "../board/board.h"
#include "../../utils/utils.h"
#include "../../utils/types.h"

namespace Chess{
  class Player{
    private:
    std::string name;
    int victories = 0;

    public:
    Player(std::string name = "");
    ~Player();
    void setName(std::string name) { this->name = name; }
    std::string getName() { return name; }
    Types::GameEvent movePiece(Vec2 from, Vec2 to, Board &board, std::list<std::tuple<Vec2, Vec2>> &plays);
  };
}