#include "piece_renderer.h"

Chess::Draw::PieceRenderer::PieceRenderer(){
  this->whitePieces = {
    allTextures.whitePawnSprite(), allTextures.whiteRookSprite(), 
    allTextures.whiteKnightSprite(), allTextures.whiteBishopSprite(),
    allTextures.whiteQueenSprite(), allTextures.whiteKingSprite()
  };

  this->blackPieces = {
    allTextures.blackPawnSprite(), allTextures.blackRookSprite(), 
    allTextures.blackKnightSprite(), allTextures.blackBishopSprite(),
    allTextures.blackQueenSprite(), allTextures.blackKingSprite()
  };
}

void Chess::Draw::PieceRenderer::draw(
  sf::RenderWindow &renderWindow, Piece *&piece, float squareSize, 
  sf::Vector2f boardPos, bool fliped
){
  unsigned int index = 0;

  switch(piece->getType()){
    case Types::Piece::Pawn: index = 0; break;
    case Types::Piece::Rook: index = 1; break;
    case Types::Piece::Knight: index = 2; break;
    case Types::Piece::Bishop: index = 3; break;
    case Types::Piece::Queen: index = 4; break;
    case Types::Piece::King: index = 5; break;
    default: break;
  }

  sf::Sprite pieceSprite = piece->getColor() == Types::Color::White ?
                           whitePieces[index] : blackPieces[index];
  
  pieceSprite.setScale({0.7, 0.7});

  pieceSprite.setOrigin({
    pieceSprite.getLocalBounds().size.x / 2.f,
    pieceSprite.getLocalBounds().size.y / 2.f
  });

  Vec2 piecePos = piece->getPosition();
  
  if(fliped)
    piecePos = {7 - piecePos.row, piecePos.col};
  
  pieceSprite.setPosition({
    boardPos.x + piecePos.col * squareSize + squareSize / 2.f,
    boardPos.y + piecePos.row * squareSize + squareSize / 2.f
  });

  renderWindow.draw(pieceSprite);
}