#include "RenderEngine.h"
#include "IRenderElement.h"
#include <sstream>
#include <stdexcept>
#include "SDL_log.h"
#include "SDL_ttf.h"

RenderEngine::RenderEngine()
{
    //members init
	_window = NULL;
    _renderer = NULL;
    _width = 0;
    _height = 0;

#ifdef _DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
#endif
 
    //first, load the SDL lib
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0)
    {
        std::stringstream ss;
        ss << "SDL could not initialize. Error " << SDL_GetError();
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", ss.str().c_str());
		throw std::runtime_error(ss.str());
    }

    //then, init the SDL_image to load .png files
	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
	{
        std::stringstream ss;
        ss << "SDL_image could not initialize. Error " << IMG_GetError();
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", ss.str().c_str());

		SDL_Quit();
		throw std::runtime_error(ss.str());
	}

    //initialize ttf
    if(TTF_Init() < 0)
    {
        std::stringstream ss;
        ss << "SDL_ttf could not initialize. Error " << TTF_GetError();
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", ss.str().c_str());

        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error(ss.str());
    }
}


RenderEngine::~RenderEngine(void)
{
    ReleaseMouseCursor();


    if(_renderer != NULL)
        SDL_DestroyRenderer(_renderer);

    if(_window != NULL)
        SDL_DestroyWindow(_window);

    //shutdown SDL_ttf library
    TTF_Quit();

    //shutdown SDL_image library
	IMG_Quit();

    //shutdown SDL library
	SDL_Quit();
}

bool RenderEngine::CreateWindow
(
    const std::string &title /*= "Breakout"*/,
    int width /*= 640*/,
    int height /*= 480*/
)
{
    //we only have 1 window
    if(_window != NULL)
        return false;

    _width = width;
    _height = height;

    //create window at undefined position at the desktop
    _window = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width,
                               height,
                               SDL_WINDOW_SHOWN);
    if(_window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Can't create main window. SDL_CreateWindow error %s", SDL_GetError());

        return false;
    }

    //Create renderer for window
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if(_renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Can't get renderer from window. SDL_CreateRenderer error %s", SDL_GetError());

        SDL_DestroyWindow(_window);
        _window = NULL;

        return false;
    }

    //Initialize renderer color
    if(SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF ) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Can't initialize renderer. SDL_SetRenderDrawColor error %s", SDL_GetError());

        SDL_DestroyRenderer(_renderer);
        _renderer = NULL;

        SDL_DestroyWindow(_window);
        _window = NULL;
        
        return false;
    }

    return true;
}

void RenderEngine::Render(const std::vector<IRenderElement *> &textures)
{
    //Clear screen
    SDL_RenderClear(Handle());

    //Render textures to screen
    for(unsigned int i=0; i<textures.size(); i++)
    {
        textures[i]->Show(*this);
    }

    //Update screen
    SDL_RenderPresent(Handle());
}

void RenderEngine::CaptureMouseCursor()
{
    //To increase user experience, we disable the mouse cursor...
    SDL_ShowCursor(SDL_DISABLE);
    //and force it report relative movement.
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void RenderEngine::ReleaseMouseCursor()
{
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
}
