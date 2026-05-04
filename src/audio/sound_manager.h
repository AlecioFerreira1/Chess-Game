#pragma once

#include <SFML/Audio.hpp>
#include <string>

namespace Chess::Audio{
    class SoundManager{
        private:
        sf::SoundBuffer soundBuffer;
        sf::Sound sound;

        void playSound(const std::string& audioPath);

        public:
        SoundManager() : sound(soundBuffer) { }
        void playCaptureSound();
        void playPromotionSound();
        void playCastleSound();
        void playIlegalMoveSound();
        void playGameEndSound();
        void playMoveCheckSound();
        void playMovePieceSound();
    };
}