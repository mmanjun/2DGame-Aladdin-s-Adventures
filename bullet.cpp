#include <cmath>
#include "bullet.h"
#include "gamedata.h"



Bullet::Bullet(const std::string& name) :

 MultiSprite(name),
 distance(0),
 maxDistance(Gamedata::getInstance().getXmlInt(name+"/maxDistance")),
 tooFar(false) ,
 strategy(new PerPixelCollisionStrategy)

 { }


Bullet::Bullet(const std::string& name,const Vector2f& pos, const Vector2f& vel) :

 MultiSprite(name,pos,vel),
 distance(0),
 maxDistance(Gamedata::getInstance().getXmlInt(name+"/maxDistance")),
 tooFar(false),
 strategy(new PerPixelCollisionStrategy) 
 { }


Bullet::Bullet(const Bullet& b) :
 
 MultiSprite(b),
 distance(b.distance),
 maxDistance(b.maxDistance),
 tooFar(b.tooFar),
 strategy(b.strategy)
 { }

void Bullet::update(Uint32 ticks) {
 advanceFrame(ticks);
 Vector2f pos =  getPosition();
 Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
 setPosition(getPosition() + incr);
 if(Y() + frameHeight < 0 || Y() > worldHeight) tooFar = true;
 if(X() < 0 || X() > worldWidth ) tooFar = true;
 distance += hypot(X()-pos[0],Y()-pos[1]);
 if (distance > maxDistance) tooFar = true;
  
}
void Bullet::reset() {
distance = 0;
tooFar = false;
 }
