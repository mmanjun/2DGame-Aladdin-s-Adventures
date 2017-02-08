#ifndef HUD__H
#define HUD__H
#include <SDL.h>
#include "drawable.h"
#include "gamedata.h"
#include "ioManager.h"
#include "aaline.h"

class Manager;

class Hud {
public:
  static Hud& getInstance();

 protected:

  friend class Manager;
  static Hud& instance;
 const Gamedata& gdata;
 const IOManager& io;
  SDL_Surface* screen;
    void drawHUD(SDL_Surface* screen, int x, int y);
    const std::string title;
    Hud();
  Hud(const Hud&);
  Hud& operator=(const Hud&);
};
#endif
