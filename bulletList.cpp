#include "bulletList.h"
#include <string>
#include "frameFactory.h"

MultiBullet::MultiBullet(const string& name) :
 name(name),
 bulVel(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")),
 frames(FrameFactory::getInstance().getFrames(name)),
 frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
 timeSinceLastFrame(0),
 freeList(), 
 bulletList()
{ }


MultiBullet::MultiBullet(const MultiBullet& b) :
 name(b.name),
 bulVel(b.bulVel),
 frames(b.frames),
 frameInterval(b.frameInterval),
 timeSinceLastFrame(b.timeSinceLastFrame),
 freeList(b.freeList),
 bulletList(b.bulletList)
{ }


void MultiBullet::draw()const {
 std::list<Bullet>::const_iterator itr = bulletList.begin();
 while(itr != bulletList.end()) {
	itr->draw();
	itr++;
  }     
}

void MultiBullet::update(Uint32 ticks) 
{
 timeSinceLastFrame += ticks;
 std::list<Bullet>::iterator itr = bulletList.begin();
 while(itr != bulletList.end()) {
	itr->update(ticks);
  	if(itr->goneTooFar()) {
		freeList.push_back(*itr);
		itr = bulletList.erase(itr);
  	}
	else {
		itr++;
	}
   }
}

void MultiBullet::shoot(const Vector2f& pos, const Vector2f& vel) {
 if(timeSinceLastFrame > frameInterval) {
	if(freeList.empty()) {
		Bullet b(name,pos,vel); 
		bulletList.push_back(b);
  	}
  	else {
  		Bullet b = freeList.front();
       		b.reset(); 
  		b.setVelocity(vel*1.3); 
  		b.setPosition(pos);
  		bulletList.push_back(b);
        	freeList.pop_front();
  	}
	timeSinceLastFrame = 0;
 }
}

void MultiBullet::bulletHUD() const  {
 IOManager::getInstance().printMessageValueAt("Magic Knife List" , bulletCount(), 20, 255);
 IOManager::getInstance().printMessageValueAt("Free List : " ,freeCount(), 20,275);
 IOManager::getInstance().printMessageAt("Touch the lamps to gain health", 20, 235);
 IOManager::getInstance().printMessageAt("Press G for GodMode", 20, 215);
}


bool MultiBullet::bulletCollidedWith(const Drawable* d) const {
 std::list<Bullet>::iterator itr= bulletList.begin();
 while(itr != bulletList.end()) {
	if(itr->collidedWith(d)) {
		freeList.push_back(*itr);
		itr = bulletList.erase(itr);
	  	return true;
	}
	else ++itr;
   }
  return false;
}

