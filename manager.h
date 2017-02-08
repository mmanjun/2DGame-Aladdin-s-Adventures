#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "scaledSprite.h"
#include "hud.h"
#include "player.h"
#include "bulletList.h"
#include "sound.h"



class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  Hud& hud;

  SDL_Surface * const screen;
  SDL_Surface * const orbSurface;
  SDL_Surface * const backSurface;
  Frame1 backFrame;
  Player player;
  // The vector must be pointers to ScaledSprite because there
  // is no SDL function to copy a Surface, probably because it
  // would be an expensive operation. Thus, due to the value 
  // semantics of STL containers, we use pointers:
  std::vector<ScaledSprite*> orbs;

  World world;
  World world1;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
   std::vector<Drawable*> smartSprites;
  int currentSprite;
  SDLSound sound;
  bool makeVideo;
  bool isHud;
  bool godMode;
  int frameCount;
  int spellcount;
  int lampcount;
  MultiBullet bullets;
  MultiBullet bullets1;
  const std::string username;
  const std::string title;
  const int frameMax;

  void draw();
  void update();
  void makeOrbs();
  void makeSprites();
  void printOrbs() const;
 // bool checkForCollisions1();
  void checkForCollisions();
  void checkForPlayerCollisions();
  void gameEnd();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
