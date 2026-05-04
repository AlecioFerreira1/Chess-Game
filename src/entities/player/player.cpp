#include "player.h"

#include <iostream>

Chess::Player::Player(std::string name){
  this->name = name;
}

Chess::Player::~Player(){}

Chess::Types::GameEvent Chess::Player::movePiece(Vec2 from, Vec2 to, Board &board){
  Types::GameEvent eventType = Types::GameEvent::Move;
  BoardMatrix &boardMatrix = board.getInfo();
  Piece *&piece = boardMatrix[from.row][from.col];
  bool isCapture = boardMatrix[to.row][to.col] != nullptr &&
                   piece->getColor() != boardMatrix[to.row][to.col]->getColor();

  bool isPawn = piece->getType() == Types::Piece::Pawn;
  Pawn *pawn = isPawn ? static_cast<Pawn *>(piece) : nullptr;
  
  if(isPawn && pawn->enPassant(boardMatrix))
    board.dealWithEnPassant(pawn, to);

  if(piece->getType() == Types::Piece::King){
    King *king = static_cast<King *>(piece);

    if(king->castling(boardMatrix)){
      board.dealWithCastling(king, to);
      eventType = Types::GameEvent::Castle;
    }
  }

  piece->setPosition(to);
  piece->moved();

  if(isCapture){  
    delete boardMatrix[to.row][to.col];
    boardMatrix[to.row][to.col] = nullptr;
    eventType = Types::GameEvent::Capture;
  }

  boardMatrix[to.row][to.col] = piece;
  boardMatrix[from.row][from.col] = nullptr;

  if(isPawn && pawn->promotion(boardMatrix)){
    board.dealWithPromotion(pawn);
    eventType = Types::GameEvent::Promotion;
  }

  Utils::plays.push_back(std::tuple<Vec2, Vec2>(from, to));

  return eventType;
}     