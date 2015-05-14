#include "RenderEngine.h"
#include "PNGImage.h"
#include <list>

int main( int argc, char* argv[] )
{
    RenderEngine engine;
    engine.CreateWindow();

    Texture::MAX_X = engine.Width();
    Texture::MAX_Y = engine.Height();

    PNGImage background("MCTestTaskBackground.png");
    background.Load(engine);

    PNGImage paddle("MCTestTaskPaddle.png");
    paddle.Load(engine);
    paddle.SetPosition(engine.Width() / 2, engine.Height() - 50);

    PNGImage ball("ball.png");
    ball.Load(engine);
    ball.SetPosition(engine.Width() / 2, engine.Height() / 2);
    ball.SetColor(0,0,0);

    PNGImage piece("MCTestTaskPiece.png");
    piece.Load(engine);

    WeakCopyTexture red_piece(piece);
    red_piece.SetColor(255, 0, 0);
    red_piece.SetPosition(0,0);

    WeakCopyTexture green_piece(piece);
    green_piece.SetColor(0, 255, 0);
    green_piece.SetPosition(piece.Width(), 0);
    
    WeakCopyTexture blue_piece(piece);
    blue_piece.SetColor(0, 0, 255);
    blue_piece.SetPosition(2*piece.Width(), 0);
    

    bool quit = false;
    SDL_Event e;
    while (!quit)
	{
        std::list<Texture *> textures;
        textures.push_back(&background);
        textures.push_back(&red_piece);
        textures.push_back(&green_piece);
        textures.push_back(&blue_piece);
        textures.push_back(&paddle);
        textures.push_back(&ball);

        engine.Render(textures);

	    if(SDL_WaitEvent(&e) != 0)
	    {
            switch(e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                paddle.SetPosition(e.motion.x-paddle.Width()/2, paddle.Y());
                break;
            default:
                break;
            }
	    }
    }

    return 0;
}


