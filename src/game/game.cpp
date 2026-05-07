#include "game.h"

Chess::Game::Game(){ }

void Chess::Game::start(){
  status = Types::GameEvent::Start;
  blackKingPos = {0, 4}, whiteKingPos = {7, 4};

  generatePieces();

  std::string name[2] = {"Player 1", "Player 2"};

  for(int i : {0, 1}){
    players[i].setName(name[i]);
  }

  running = true;
}

void Chess::Game::end(){
  removePieces();
  plays.clear();
}

void Chess::Game::restart(){
  end();
  start();
  playerTurn = 1;
}

void Chess::Game::changeTurn(){
  playerTurn = playerTurn == 1 ? 2 : 1;
}

void Chess::Game::generatePieces(){
  Chess::BoardMatrix &boardMatrix = board.getInfo();
  const Types::Color WHITE = Types::Color::White;
  const Types::Color BLACK = Types::Color::Black;

  Piece *WhiteBackRow[8] = {
    new Rook(WHITE, {7, 0}), new Knight(WHITE, {7, 1}), new Bishop(WHITE, {7, 2}), 
    new Queen(WHITE, {7, 3}), new King(WHITE, {7, 4}), new Bishop(WHITE, {7, 5}), 
    new Knight(WHITE, {7, 6}), new Rook(WHITE, {7, 7})      
  };

  Piece *blackBackRow[8] = {
    new Rook(BLACK, {0, 0}), new Knight(BLACK, {0, 1}), new Bishop(BLACK, {0, 2}), 
    new Queen(BLACK, {0, 3}), new King(BLACK, {0, 4}), new Bishop(BLACK, {0, 5}), 
    new Knight(BLACK, {0, 6}), new Rook(BLACK, {0, 7})      
  };

  for(int i = 0; i < 8; ++i){
    boardMatrix[1][i] = new Pawn(BLACK, {1, i});             
    boardMatrix[6][i] = new Pawn(WHITE, {6, i});
    boardMatrix[0][i] = blackBackRow[i];
    boardMatrix[7][i] = WhiteBackRow[i];           
  }
}

void Chess::Game::removePieces(){
  Chess::BoardMatrix &boardMatrix = board.getInfo();

  for(size_t i = 0; i < 8; ++i){
    for(size_t j = 0; j < 8; ++j){
      if(boardMatrix[i][j] != nullptr){
        delete boardMatrix[i][j];
        boardMatrix[i][j] = nullptr;
      }
    }
  }
}

bool Chess::Game::invalidMove(Vec2 from, Vec2 to){
  Chess::BoardMatrix& boardMatrix = board.getInfo();      
  Chess::Piece *piece = boardMatrix[from.row][from.col];
  bool correctColorPieceToMove = piece != nullptr &&
                          ((playerTurn == 1 && piece->getColor() == Types::Color::White) ||
                          (playerTurn == 2 && piece->getColor() == Types::Color::Black)) ?
                          true : false;

  if(!correctColorPieceToMove) return true;
  
  std::vector<Vec2> moves = piece->getMoves(boardMatrix);

  if(piece->getType() == Types::Piece::Pawn){
    Chess::Pawn *pawn = static_cast<Pawn *>(piece);
    moves = pawn->getMoves(boardMatrix, plays);
  }

  for(Vec2 move : moves){   
    if(to.row == move.row && to.col == move.col){
      if(momentaniumCheck({from, to})) return true;

      else{
        if(piece->getType() == Types::Piece::King){
          if(piece->getColor() == Types::Color::White) whiteKingPos = to;
          else blackKingPos = to;
        }

        return false;
      }
    }
  }

  return true;
}

bool Chess::Game::check(){
  const BoardMatrix &boardMatrix = board.getInfo();
  Vec2 kingPos = whiteKingPos;

  if(playerTurn == 2)
    kingPos = blackKingPos;
  
  King *king = static_cast<King *>(boardMatrix[kingPos.row][kingPos.col]);
  return !king->notAttacked(king->getPosition(), boardMatrix);
}

bool Chess::Game::checkmate(){
  return status == Types::GameEvent::Checkmate;
}

bool Chess::Game::stalemate(){
  return status == Types::GameEvent::Stalemate;
}

bool Chess::Game::verifyStalemate(){
  const Types::Color color = playerTurn == 1 ? Types::Color::White : Types::Color::Black;
  BoardMatrix &boardMatrix = board.getInfo();

  for(int i = 0; i < 8; ++i){
    for(int j = 0; j < 8; ++j){
      Piece *piece = boardMatrix[i][j];

      if(piece != nullptr && piece->getColor() == color){
        std::vector<Vec2> moves = piece->getMoves(boardMatrix);

        if(piece->getType() == Types::Piece::Pawn){
          Chess::Pawn *pawn = static_cast<Pawn *>(piece);
          moves = pawn->getMoves(boardMatrix, plays);
        }

        for(const Vec2& target : moves){
          std::tuple<Vec2, Vec2> move = std::tuple(piece->getPosition(), target); 

          if(!momentaniumCheck(move))
            return false;
        }
      }
    }
  }

  return true;
}

void Chess::Game::updateStatus(){     
  const bool isCheck = check();
  const bool isStaleMate = verifyStalemate();
  const bool isCheckMate = isCheck && isStaleMate;

  if(isCheckMate) this->status = Types::GameEvent::Checkmate;
  else if(isStaleMate) this->status = Types::GameEvent::Stalemate;
  else if(isCheck) this->status = Types::GameEvent::Check;
}

bool Chess::Game::finished(){
  if(
    getStatus() == Types::GameEvent::Checkmate || 
    getStatus() == Types::GameEvent::Stalemate
  ){ 
    running = false;  //CHANGE TO A CONFIRM DIALOG WINDOW
    return true;
  }

  else return false;
}

std::vector<Vec2> Chess::Game::filterMovesToAvoidInconsistencies(Piece *pieceSelected){
  BoardMatrix &boardMatrix = board.getInfo();
  std::vector<Vec2> moves = pieceSelected->getMoves(boardMatrix);
  std::vector<Vec2> filteredMoves;

  if(pieceSelected->getType() == Types::Piece::Pawn){
    Chess::Pawn *pawn = static_cast<Pawn *>(pieceSelected);
    moves = pawn->getMoves(boardMatrix, plays);
  }

  for(const Vec2 &move : moves){ 
    if(!momentaniumCheck({pieceSelected->getPosition(), move})){
      filteredMoves.push_back(move);
    }
  }

  return filteredMoves;
}

bool Chess::Game::momentaniumCheck(std::tuple<Vec2, Vec2> move){
  using namespace Chess;

  Vec2 from = std::get<0>(move), to = std::get<1>(move); 

  if(from.row == -1) return false; 

  BoardMatrix boardMatrix = board.getInfo();
  Piece *pieceFrom = boardMatrix[from.row][from.col];
  const Types::Color color = pieceFrom->getColor();

  if(pieceFrom->getType() == Types::Piece::Pawn){
    Pawn *pawn = static_cast<Pawn *>(pieceFrom);

    if(pawn->enPassant(boardMatrix, plays)){
      boardMatrix[to.row - pawn->getOrientation()][to.col] = nullptr;
    }
  }

  boardMatrix[from.row][from.col] = nullptr;
  boardMatrix[to.row][to.col] = pieceFrom;

  if(pieceFrom->getType() == Types::Piece::King){
    King *king = static_cast<King *>(pieceFrom);
    return !king->notAttacked(to, boardMatrix);
  }

  Vec2 kingPos = whiteKingPos;

  if(pieceFrom->getColor() == Types::Color::Black)
    kingPos = blackKingPos;

  King *king = static_cast<King *>(boardMatrix[kingPos.row][kingPos.col]);
  return !king->notAttacked(king->getPosition(), boardMatrix);
}