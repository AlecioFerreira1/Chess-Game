#include "game.h"

Chess::Game::Game(){
  generatePieces();
}

void Chess::Game::start(){
  std::string name[2] = {"Player 1", "Player 2"};

  for(int i : {0, 1}){
    players[i].setName(name[i]);
  }

  running = true;
}

void Chess::Game::end(){
  removePieces();
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

  if(!correctColorPieceToMove) 
    return true;

  for(Vec2 move : piece->getMoves(boardMatrix)){   
    if(to.row == move.row && to.col == move.col){
      if(Utils::momentaniumCheck({from, to}, boardMatrix))
        return true;

      else return false;
    }
  }

  return true;
}

bool Chess::Game::check(){
  if(Utils::plays.empty()) return false;

  const std::tuple<Vec2, Vec2> lastMove = Utils::plays.back();
  const Vec2 pieceLocated = std::get<1>(lastMove);
  const BoardMatrix &boardMatrix = board.getInfo();
  Piece *piece = boardMatrix[pieceLocated.row][pieceLocated.col];

  for(const Vec2& pos : piece->getMoves(boardMatrix)){
    if(
      boardMatrix[pos.row][pos.col] != nullptr &&
      boardMatrix[pos.row][pos.col]->getType() == Types::Piece::King &&
      boardMatrix[pos.row][pos.col]->getColor() != piece->getColor() 
    ) { return true; }
  }

  return false;
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
        for(const Vec2& target : piece->getMoves(boardMatrix)){
          std::tuple<Vec2, Vec2> move = std::tuple(piece->getPosition(), target); 

          if(Utils::momentaniumCheck(move, boardMatrix) == false)
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

  for(const Vec2 &move : moves){ 
    if(Utils::momentaniumCheck({pieceSelected->getPosition(), move}, boardMatrix) == false){
      filteredMoves.push_back(move);
    }
  }

  return filteredMoves;
}