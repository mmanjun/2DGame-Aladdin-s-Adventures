#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "manager.h"


int flag=0;

void Player::advanceFrame(Uint32 ticks)
 {
  timeSinceLastFrame+=ticks;
  if(timeSinceLastFrame>frameInterval)
  {
    if(velocityX()>0)
    {
     flag=0;
if(currentFrame>numberOfFrames/2)
  {
currentFrame=0;
}
}
  if(velocityX()<0)
  {
    flag=1;
}
if(flag)
{
    if(currentFrame < numberOfFrames/2)
{
    currentFrame=((numberOfFrames/2));
}
if(currentFrame == numberOfFrames-1)
{
    currentFrame=((numberOfFrames/2));
}
     currentFrame=(currentFrame+1)%numberOfFrames;
    timeSinceLastFrame=0;
  }
	else
  {
    currentFrame=(currentFrame+1)%(numberOfFrames/2);
    timeSinceLastFrame=0;
  }
}
  }


Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
    explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  //worldWidth(WORLD_WIDTH),
  //worldHeight(WORLD_HEIGHT),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
   shootLeft(FrameFactory::getInstance().getFrames("ShootLeft")),
  shootRight(FrameFactory::getInstance().getFrames("ShootRight")),
  timeSinceLastFrame( 0 ),
  explodeFlag(false),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  strategy( new PerPixelCollisionStrategy )
{ }

/*Player::Player(const Player& s) :
  Drawable(s), 
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }*/

Player::~Player() { 
  delete frames[currentFrame];
  delete strategy;
  delete explosion;
}


void Player::draw() const { 
  if (explosion) {
  explosion->draw();
        return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
   bar.draw(); 
}

void Player::explode() { 
  explodeFlag = true;
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite); 
  stop();
}

void Player::shoot() {

  

}



void Player::update(Uint32 ticks) { 
   if ( explosion ) {
  explosion->update(ticks);
  if ( explosion->chunkCount() == 0 ) {
     delete explosion;
    explosion = NULL;
  }
  return;
  }
SDL_Event event;
Uint8 *keystate = SDL_GetKeyState(NULL); 

 
  advanceFrame(ticks);


 Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
 setPosition(getPosition() + incr);

if(Y() < 400)
{
  velocityY( 0 );
}

  if ( keystate[SDLK_w]){ 
    velocityY( -300);
    velocityX(0);
  }

    if ( keystate[SDLK_s]){
    velocityY( 300);
    velocityX(0);
  }

   if (X()<0 && keystate[SDLK_d]) {
    flag=0;
  currentFrame=0;
    velocityX( 300 );
    velocityY(0);
  }

  if(Y()<400 && keystate[SDLK_w])
  {
    velocityY(300);
    velocityX(0);
  }

  if(Y()>worldHeight-frameHeight && keystate[SDLK_s])
  {
    velocityY(-300);
    velocityX(0);
  }

    if( Y()>340 && keystate[SDLK_w])   
  {
    velocityY(-300); 
    velocityX(0);  
  }

      if( Y()>470 && keystate[SDLK_z])   
  {
    velocityY(-300);  
    velocityX(0); 
  }
 
   if (keystate[SDLK_d]) {
    velocityX( 300 );
    velocityY(0);
  }
  
 if (keystate[SDLK_a] ){
    velocityX( -300 );
    velocityY(0);
 }

 if(keystate[SDLK_r]){
  Vector2f incr = Vector2f(120,370 );
  setPosition(incr);
 }

 if(keystate[SDLK_a]&&keystate[SDLK_d])
 {
  velocityX( 0 );
   velocityY(0);
 }

  if(keystate[SDLK_w]&&keystate[SDLK_s])
 {
  velocityX( 0 );
   velocityY(0);
 }

  if(keystate[SDLK_a]&&keystate[SDLK_s])
 {
  velocityX( 0 );
   velocityY(0);
 }

  if(keystate[SDLK_d]&&keystate[SDLK_s])
 {
  velocityX( 0 );
   velocityY(0);
 }

   if(keystate[SDLK_a]&&keystate[SDLK_w])
 {
  velocityX( 0 );
   velocityY(0);
 }

   if(keystate[SDLK_d]&&keystate[SDLK_w])
 {
  velocityX( 0 );
  velocityY(0);
 }

 if(X()<0)
 {
  flag=0;
  currentFrame=0;
  velocityX(300);
 }

 if(X()> worldWidth-frameWidth)
 {
  velocityX( -300 );
 }
   bar.update(ticks);      
}


bool Player::hasExploded() {
 if(explodeFlag) {
	bar.reset();
  }
  return false;
}

void Player::stop()
{
  velocityX( 0 );
}

void Player::resetPlayer() {
  setPosition(Vector2f(300.0,480.0));
  bar.reset();
}


/*float x = X();
  float y = Y();
  Vector2f incr = Vector2f(x,270);
  setPosition(incr);
  incr = Vector2f(x-x,y-y);
   setPosition(incr);*/






