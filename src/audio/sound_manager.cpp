#include "sound_manager.h"

namespace Chess::Audio{
    void SoundManager::playSound(const std::string& audioPath){
        if(soundBuffer.loadFromFile(audioPath) && Config::soundEnabled){
            sound.setBuffer(soundBuffer);
            sound.play();
        }
    }

    void SoundManager::playCaptureSound(){
        playSound("assets/audio/capture.mp3");
    }
    
    void SoundManager::playPromotionSound(){
        playSound("assets/audio/promote.mp3");
    }

    void SoundManager::playCastleSound(){
        playSound("assets/audio/castle.mp3");
    }

    void SoundManager::playIlegalMoveSound(){
        playSound("assets/audio/illegal.mp3");
    }

    void SoundManager::playGameEndSound(){
        playSound("assets/audio/game-end.mp3");
    }

    void SoundManager::playMoveCheckSound(){
        playSound("assets/audio/move-check.mp3");
    }

    void SoundManager::playMovePieceSound(){
        playSound("assets/audio/move-self.mp3");
    }

    void SoundManager::playStartGameSound(){
        playSound("assets/audio/game-start.mp3");
    }
}