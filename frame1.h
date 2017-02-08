#ifndef FRAME1__H
#define FRAME1__H

#include <iostream>
#include <string>
#include <SDL.h>

class Frame1 {
public:
  Frame1( const std::string& fn );
  // Must pass both width and height here because the sprite might
  // not consist of the whole surface, only part of the surface.
  Frame1( SDL_Surface* spr, Uint16 sprite_width, Uint16 sprite_height,
         Sint16 src_x, Sint16 src_y);
  Frame1(const Frame1&);
  Frame1& operator=(const Frame1&);

  SDL_Surface* getSurface() const { return spriteSurface; }
  void draw(Sint16 x, Sint16 y) const;
  void draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const;

  Uint16 getWidth()  const { return spriteWidth; }
  Uint16 getHeight() const { return spriteHeight; }
  Uint16 getSourceX() const { return spriteSourceX; }
  Uint16 getSourceY() const { return spriteSourceY; }

private:
  SDL_Surface * screen;
  SDL_Surface * spriteSurface;
  Uint16 spriteWidth;
  Uint16 spriteHeight;
  Sint16 spriteSourceX;
  Sint16 spriteSourceY;
};

#endif