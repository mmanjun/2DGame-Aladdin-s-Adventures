#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "player.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include <iostream>
#include "ioManager.h"
#include <algorithm>
#include "aaline.h"
#include "hud.h"
#include "bullet.h"
#include "smartSprite.h"


const int HUD_WIDTH = 300;
const int HUD_HEIGHT = 300;
static bool lost = false;
static bool won = false;

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};


Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.

    std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    delete (*ptr);
    ++ptr;
  }
 
  std::vector<ScaledSprite*>::const_iterator ptr2 = orbs.begin();
  while ( ptr2 != orbs.end() ) {
    delete (*ptr2);
    ++ptr2;
  }

  std::vector<Drawable*>::const_iterator ptr3 = smartSprites.begin();
  while ( ptr3 != smartSprites.end() ) {
    delete (*ptr3);
    ++ptr3;
  }
  orbs.clear();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  hud( Hud::getInstance() ),
  isHud(false),
  screen( io.getScreen() ),
  world("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  world1("castle", Gamedata::getInstance().getXmlInt("castle/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  smartSprites(),
  player("aladdin"),
  bullets("knife"),
  bullets1("spell"),
  spellcount(0),
  lampcount(0),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  sound(),
  godMode(false),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  orbSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("lantern/file"), 
              Gamedata::getInstance().getXmlBool("lantern/transparency")) ),
  backSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("sky/file"), 
              Gamedata::getInstance().getXmlBool("sky/transparency")) ),
  backFrame(backSurface,
                Gamedata::getInstance().getXmlInt("sky/width"), 
                Gamedata::getInstance().getXmlInt("sky/height"), 
                Gamedata::getInstance().getXmlInt("sky/src/x"), 
                Gamedata::getInstance().getXmlInt("sky/src/y")),
  orbs()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
    Uint8 *keystate = SDL_GetKeyState(NULL);
  
 
  SDL_WM_SetCaption(title.c_str(), NULL);
  makeSprites();
  makeOrbs();
  viewport.setObjectToTrack(&player);

  }

  void Manager::makeSprites(){
   int i;
  sprites.push_back( new Sprite("sorcerer"));
  spellcount = Gamedata::getInstance().getXmlInt("spell/spellcount");
  lampcount = Gamedata::getInstance().getXmlInt("lamp/lampcount");
 for( i =0; i<lampcount; i++) {
  smartSprites.push_back(new SmartSprite ("lamp",Vector2f(100.0,200),player)); 
  }

   for( i =0; i<spellcount-1; i++) {
  sprites.push_back(new Sprite ("spell",Vector2f(100.0,200),Vector2f(0,0))); 
  }
    sprites.push_back( new MultiSprite("jasmine") );
}

  void Manager::makeOrbs() {
  unsigned numberOfOrbs = Gamedata::getInstance().getXmlInt("numberOfOrbs");
  orbs.reserve( numberOfOrbs );
  for (unsigned i = 0; i < numberOfOrbs; ++i) {
    orbs.push_back( new ScaledSprite("lantern", orbSurface) );
  }
  sort(orbs.begin(), orbs.end(), ScaledSpriteCompare());
}

void Manager::printOrbs() const {
  for (unsigned i = 0; i < orbs.size(); ++i) {
    std::cout << orbs[i]->getScale() << std::endl;
  }
}

/*bool Manager::checkForCollisions1()  {
  std::vector<Drawable*>::const_iterator sprite = sprites.begin();
  while ( sprite != sprites.end()-1 ) {
    if ( player.collidedWith(*sprite) )
     return true;
    ++sprite;
  }
  return false;
}*/

void Manager::checkForCollisions() {

   ExplodingSprite* e = dynamic_cast<ExplodingSprite*>(sprites[0]);
    Sprite* sprite = static_cast<Sprite*>( sprites[0] );

    if(e && e->chunkCount()==0) {
     e->setPosition(Vector2f(-1000.0,-1000.0));
     e->setVelocity(Vector2f(0.0,0.0));
    }
    if ( !e && bullets.bulletCollidedWith((sprite)) ) {    
      sprites[0] = new ExplodingSprite(*sprite);
      sound[0];
      delete sprite;
      if ( spellcount ) spellcount--;

    }
    
  for (unsigned i = 1; i < sprites.size()-1; ++i) {
    ExplodingSprite* e = dynamic_cast<ExplodingSprite*>(sprites[i]);
    Sprite* sprite = static_cast<Sprite*>( sprites[i] );

    if(e && e->chunkCount()==0) {
     e->setPosition(Vector2f(-1000.0,-1000.0));
     e->setVelocity(Vector2f(0.0,0.0));
    }
    if ( !e && bullets.bulletCollidedWith((sprite)) ) {    
      sprites[i] = new ExplodingSprite(*sprite);
      sound[0];
      delete sprite;
      if ( spellcount ) spellcount--;
    }
    
  }

}

void Manager::gameEnd() {
  if(spellcount==0) {
     won = true;
     lost = false;
    }
}

void Manager::checkForPlayerCollisions(){

 std::vector<Drawable*>::const_iterator sprite = sprites.begin();
 std::vector<Drawable*>::const_iterator smSprite = smartSprites.begin();
 while(sprite != sprites.end() && smSprite !=  smartSprites.end()) {
    if(player.collidedWith(*smSprite)) {
             player. incrementHealthBar();
           }
	  if(player.collidedWith(*sprite)) {
             player. decrementHealthBar();
    if(player.healthIsEmpty()) {
	   player.explode();
     sound[0];
     player.setPosition(Vector2f(player.X()+100,player.Y()-100));
     lost = true;
      won = false;
	   }}
      ++sprite; 
      ++smSprite;
	}
 }



void Manager::draw() {
  bool keyCatch=false;
  world.draw();

  int startX = 0;
  int startY = 0;


  for (unsigned i = 0; i < orbs.size()/2; ++i) {
    orbs[i]->draw();
   }


     world1.draw();

   for (unsigned i = orbs.size()/2; i < orbs.size(); ++i) {
    orbs[i]->draw();
   }

  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }

  std::vector<Drawable*>::const_iterator ptr1 = smartSprites.begin();
  while ( ptr1 != smartSprites.end() ) {
  (*ptr1)->draw();
  ++ptr1;
  }

  if(clock.getSeconds()<3)
  {
    hud.drawHUD(screen, startX, startY);
     io.printMessageAt("Aladdin's Adventures!", 15, 0);
     io.printMessageAt("Destroy 10 spells and kill sorcerer", 0, 25);
     io.printMessageValueAt("Seconds: ", clock.getSeconds(), 20, 45);
     io.printMessageValueAt("FPS: ",clock.getFps(), 20, 75);
     io.printMessageAt("Press D to move right", 20,95);
     io.printMessageAt("Press A to move left", 20, 115);
     io.printMessageAt("Press W to move up", 20, 135);
     io.printMessageAt("Press S to move down", 20, 155);
     io.printMessageAt("Press spacebar to shoot", 20, 175);
     io.printMessageAt("Press R to restart game", 20, 195);
     bullets.bulletHUD();
    }
    else if(isHud==true)
    { 
     hud.drawHUD(screen, startX, startY);
     io.printMessageAt("Aladdin's Adventures!", 15, 0);
    io.printMessageAt("Destroy 10 spells and kill sorcerer!", 0, 25);
     io.printMessageValueAt("Seconds: ", clock.getSeconds(), 20, 45);
     io.printMessageValueAt("FPS: ",clock.getFps(), 20, 75);
     io.printMessageAt("Press D to move right", 20,95);
     io.printMessageAt("Press A to move left", 20, 115);
     io.printMessageAt("Press W to move up", 20, 135);
     io.printMessageAt("Press S to move down", 20, 155);
     io.printMessageAt("Press spacebar to shoot", 20, 175);
     io.printMessageAt("Press R to restart game", 20, 195);
     bullets.bulletHUD();
     }
 
    player.draw();
    bullets.draw();
    bullets1.draw();

     io.printMessageValueAt("Enemies left to destroy:", spellcount, 300, 30);

      if(godMode) {
     io.printMessageCenteredAt("Aladdin in God Mode", 70); 
  }
 std::stringstream strm;
  int playerX = static_cast<int>(player.X());
  int playerY = static_cast<int>(player.Y());
  strm << "Player at: (" << playerX << ", " << playerY << ")";
 


 /* if ( checkForCollisions1() ) {
   // player.explode();
    io.printMessageAt("*** Oops ***, collision!", 320, 60);
   // io.printMessageAt("***", playerX-viewport.X(), playerY-viewport.Y()-10);
   // io.printMessageAt("***", playerX-viewport.X(), playerY-viewport.Y()+20);
  }
  else {
    io.printMessageAt("No Collision.", 320, 60);
  }*/

   if(lost) { 
     io.printMessageAt("OOPS!ALADDIN COULD NOT RESCUE JASMINE!!",200,250);
  }
  if(won) {
    io.printMessageAt("HURRAY!ALADDIN RESCUED JASMINE!",200,250);
    //sound[1];
  }

  viewport.draw();
  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}



void Manager::update() {
   Uint8 *keystate = SDL_GetKeyState(NULL);
       ++(clock);
  Uint32 ticks = clock.getElapsedTicks();
     for (unsigned i = 0; i < orbs.size(); ++i) {
    orbs[i]->update(ticks);
  }
  if(keystate[SDLK_a]||keystate[SDLK_s]||keystate[SDLK_d]||keystate[SDLK_w]||keystate[SDLK_r]||keystate[SDLK_SPACE])
  {
  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
    //switchSprite();
  }
  
   /*for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }*/

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  player.update(ticks);
  }
  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }

  std::vector<Drawable*>::iterator ptr1 = smartSprites.begin();
  while ( ptr1 != smartSprites.end() ) {
  (*ptr1)->update(ticks);
  ++ptr1;
}
  checkForCollisions();
    if(!godMode) {
    checkForPlayerCollisions();
  }
   gameEnd();
  world.update();
  world1.update();
  viewport.update(); // always update viewport last
  bullets.update(ticks);
  bullets1.update(ticks);
}




void Manager::play() {

     
  Uint32 ticks = clock.getElapsedTicks();
  SDL_Event event;
    int minSpeed = 200;
  bool done = false;
  bool keyCatch=false;
  bool zeekey = false;


  while ( not done ) {

    SDL_Flip(screen);
     Uint8 *keystate = SDL_GetKeyState(NULL);
    while ( SDL_PollEvent(&event) ) {
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYUP) { keyCatch = true; }
      if(event.type == SDL_KEYDOWN || event.type==SDL_KEYUP) {
        switch ( event.key.keysym.sym) {
          case SDLK_ESCAPE : done = true; break;
          case SDLK_q : done =true; break;
          case SDLK_l : clock.toggleSloMo();
        default : break;
      }
       if(keystate[SDLK_F1]  ) {
          if(!isHud)
            isHud = true;  
          else isHud = false;
    } 


    if(keystate[SDLK_SPACE]) {
          Vector2f sVel = player.getVelocity();
      float x = player.X()+20;
      float y = player.Y()-20;
      if(sVel[0] >= 0) {
        x = player.X() ;
        sVel[0] += minSpeed;
      }
      else {
        x = player.X();
        sVel[0] -= minSpeed;
      }
      sound[3];
      bullets.shoot(Vector2f(x,y),sVel);
    } 

      if(keystate[SDLK_r]) {  
      player.resetPlayer();
      sprites.clear();
      makeSprites();
      //isHud=true;
      lost = false;
      won = false;
    }   

    //god mode
    if(keystate[SDLK_g]) {
      if(!godMode)
            godMode = true;  
          else godMode = false;
    } 

        
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
  }
       


      } 
    }

    draw();
   update();
           
  
 }
}
