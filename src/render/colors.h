#pragma once
#include <SFML/Graphics.hpp>

namespace Chess{
  class Colors { 
    public:
    static constexpr sf::Color dark = sf::Color(184, 135, 98);
    static constexpr sf::Color light = sf::Color(237, 214, 176); 
    static constexpr sf::Color darkSelection = sf::Color(220.f, 195.f, 75.f);
    static constexpr sf::Color lightSelecion = sf::Color(246.f, 235.f, 114.f);
  };
}