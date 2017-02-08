#include "drawable.h"
#include "frame1.h"
#include "ioManager.h"

Frame1::Frame1( const Frame1& frame1 ) :
  screen(frame1.screen),
  spriteSurface(frame1.spriteSurface), 
  spriteWidth(frame1.spriteWidth), spriteHeight(frame1.spriteHeight),
  spriteSourceX(frame1.spriteSourceX), spriteSourceY(frame1.spriteSourceY) 
  { }

Frame1::Frame1( SDL_Surface* spr, Uint16 sprite_width, Uint16 sprite_height,
              Sint16 src_x, Sint16 src_y) :
  screen(IOManager::getInstance().getScreen()),
  spriteSurface(spr), 
  spriteWidth(sprite_width), spriteHeight(sprite_height),
  spriteSourceX(src_x), spriteSourceY(src_y) {
}

Frame1& Frame1::operator=(const Frame1& rhs) {
  spriteSurface = rhs.spriteSurface;
  screen = rhs.screen;
  spriteWidth = rhs.spriteWidth;
  spriteHeight = rhs.spriteHeight;
  spriteSourceX = rhs.spriteSourceX;
  spriteSourceY = rhs.spriteSourceY;
  return *this;
}

void Frame1::draw(Sint16 x, Sint16 y) const {
  SDL_Rect src = { spriteSourceX, spriteSourceY, 
                   spriteWidth, spriteHeight 
                 };    
  SDL_Rect dest = {x, y, spriteWidth, spriteHeight };
  SDL_BlitSurface(spriteSurface, &src, screen, &dest);
}

void Frame1::draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const {
  SDL_Rect src = { sx, sy, spriteWidth, spriteHeight };    
  SDL_Rect dest = {dx, dy, spriteWidth, spriteHeight };
  SDL_BlitSurface(spriteSurface, &src, screen, &dest);
}


