#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "drawable.h"
#include "bulletList.h"
#include "health.h"
#include "explodingSprite.h"

class Player : public Drawable {
public:
  Player(const std::string&);
  virtual ~Player();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
   bool collidedWith(const Drawable* d) const {
	return strategy->execute(*this, *d);
 }
   void resetPlayer();
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  bool hasExploded();
  void explode();
  void stop();
  void decrementHealthBar() {
  bar.decrement();
  }
  void incrementHealthBar() {
    bar.increment();
  }
 bool healthIsEmpty() {
  return bar.isEmpty();
 }
   void shoot();

protected:
  ExplodingSprite* explosion;
  const std::vector<Frame *> frames;
  const std::vector<Frame *> shootLeft;
  const std::vector<Frame *> shootRight;
  int worldWidth;
  int worldHeight;
  int explodeFlag;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  Health bar;
  CollisionStrategy* strategy;
  void advanceFrame(Uint32 ticks);
  Player(const Player&);
  Player& operator=(const Player&);
};
#endif
