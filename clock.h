#include <SDL.h>
#include <string>
#include <math.h>
#include "gamedata.h"

class Manager;

class Clock {
public:
  static Clock& getInstance();
  unsigned int getTicks() const;
  unsigned int getTotalTicks() const { return sumOfAllTicks; }
protected:
  friend class Manager;
  static Clock& instance;
  int getFps() const;
  int getAverageFps() const;
  bool started;
  bool paused;
  bool sloMo;
  bool isHud;
  std::deque<int> recentFrames;
  const bool framesAreCapped;
  const int frameCap;
    unsigned maxFrames;
  unsigned int frames;
  unsigned int fps;
  unsigned int tickSum;
  unsigned int sumOfAllTicks;
  unsigned int timeAtStart;
  unsigned int timeAtPause;
  unsigned int currTicks;
  unsigned int prevTicks;
  unsigned int ticks;
  unsigned int getElapsedTicks();
  Clock& operator++();
  void toggleSloMo();
  void toggleHud();

  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned int getFrames() const  { return frames;  } 
  unsigned int getSeconds() const { return getTicks()/1000;  }
  unsigned int capFrameRate() const;

  void start();
  void pause();
  void unpause();
  void display() const;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
