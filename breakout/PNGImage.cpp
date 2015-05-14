#include "PNGImage.h"


PNGImage::PNGImage(const std::string &filename) : _filename(filename)
{
}


PNGImage::~PNGImage(void)
{
}

bool PNGImage::Load(RenderEngine &engine)
{
    if(IsValid() == true)
        Free();

    SDL_Surface *loaded = IMG_Load(_filename.c_str());
    if(loaded == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can't load PNG file %s. IMG_Load error ", _filename.c_str(), SDL_GetError());
        return false;
    }

    this->_width = loaded->w;
    this->_height = loaded->h;

    //optimize loaded image to screen format
    _texture = SDL_CreateTextureFromSurface(engine.Handle(), loaded);
    if(_texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                     "Can't create texture from PNG file %s. SDL_CreateTextureFromSurface error ", 
                     _filename.c_str(), 
                     SDL_GetError());
    }

    //get rid of the loaded image
    SDL_FreeSurface(loaded);

    return IsValid();
}