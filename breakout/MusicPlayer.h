#pragma once
#include "SDL_mixer.h"

#include <string>
class MusicPlayer
{
public:
    MusicPlayer(void);

    virtual ~MusicPlayer(void);

    inline void PlayGameStart() {PlayChunck(_game_start);}

    inline void PlayGameOver() {PlayChunck(_game_over);}

    inline void PlayBounce() {PlayChunck(_bounce);}

    inline void PlayExtraLife() {PlayChunck(_extra_life);}

    inline void PlayBonus() {PlayChunck(_bonus);}

    inline void PlayWin() {PlayChunck(_win);}

protected:
    void PlayChunck(Mix_Chunk *chunck);

    Mix_Chunk *_bounce, *_game_over, *_game_start, *_extra_life, *_bonus, *_win;
};

