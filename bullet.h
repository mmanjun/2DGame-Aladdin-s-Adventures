#ifndef BULLET__H
#define BULLET__H
#include <string>
#include "collisionStrategy.h"
#include "multisprite.h"

class Bullet : public MultiSprite {
public :
Bullet(const std::string&);
Bullet(const std::string& name, const Vector2f& pos, const Vector2f& vel);
Bullet(const Bullet&);
virtual void update(Uint32 ticks);
bool goneTooFar()const {return tooFar;}
void reset();
bool collidedWith(const Drawable* d) const {
    return strategy->execute(*this, *d);
  }
protected :
int distance;
int maxDistance;
bool tooFar; 
CollisionStrategy * strategy;
Bullet& operator=(const Bullet&);
};
#endif
