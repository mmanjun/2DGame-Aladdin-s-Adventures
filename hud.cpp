#include <algorithm>
#include <cmath>
#include <SDL.h>
#include <stdio.h>
#include "hud.h"
#include "aaline.h"
#include "ioManager.h"
#include "gamedata.h"
#include <iomanip>
#include <iostream>

const int HUD_WIDTH = 300;
const int HUD_HEIGHT = 300;

Hud& Hud::getInstance() {
  static Hud hud;
  return hud;
}

Hud::Hud() : 
  gdata(Gamedata::getInstance()),
    io( IOManager::getInstance() ),
    screen(io.getScreen())
{}

void Hud::drawHUD(SDL_Surface* screen, int x, int y) {
  const Uint32 BLUE = SDL_MapRGB(screen->format, 0,0, 0xff);
  Draw_AALine(screen, x, y+HUD_HEIGHT/2, 
                      x+HUD_WIDTH,y+HUD_HEIGHT/2, 
                      HUD_HEIGHT, 0xff, 0xff, 0xff, 0xff/2);
  Draw_AALine(screen, x,y, x+HUD_WIDTH,y, BLUE);
  Draw_AALine(screen, x+HUD_WIDTH,y+HUD_HEIGHT, x,y+HUD_HEIGHT, BLUE);
  Draw_AALine(screen, x+HUD_WIDTH,y, x+HUD_WIDTH,y+HUD_HEIGHT, BLUE);
}