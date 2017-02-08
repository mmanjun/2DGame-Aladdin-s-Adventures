#ifndef MULTIBULLET__H
#define MULTIBULLET__H
#include <string>
#include "bullet.h"
#include "chunk.h"
#include <list>
#include "ioManager.h"
class MultiBullet {
public : 
 MultiBullet(const string &name);
 MultiBullet(const MultiBullet& b);
 void shoot(const Vector2f& pos, const Vector2f& vel);
 void draw () const ;
 void update(Uint32 ticks);
 void bulletHUD() const ;
 bool bulletCollidedWith(const Drawable* d) const; 
 unsigned int freeCount() const {return freeList.size();}
 unsigned int bulletCount() const { return bulletList.size();}
 bool isEmpty() const {return freeList.empty();}
private :
 std::string name;
 Vector2f bulVel;
 const std::vector<Frame *> frames;
 int frameInterval;
 int timeSinceLastFrame;
 mutable std::list<Bullet>freeList;
 mutable std::list<Bullet>bulletList;
 MultiBullet& operator=(MultiBullet&);
};
#endif