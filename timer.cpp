#include "timer.h"

#define restoreTimersIterator()         \
          (timersIterator_ = timers_)
          
/* TimerTask */
TimerTask& TimerTask::instance()
{
  static TimerTask timerTask_;
  return timerTask_;
}

TimerTask::TimerTask() :
  EventTask(System::Timer::EVENT_TIME)
{
  timersIterator_ = timers_;
  timersEnd_ = timers_;
  
  // Initialize our table to NULL
  for(unsigned char it = 0; it < System::Timer::TIMER_COUNT; ++it)
    timers_[it] = nullptr;
}

TimerTask::~TimerTask()
{
  
}

void TimerTask::registerTimer(BasicTimer& timer)
{
  for(unsigned char it = 0; it < System::Timer::TIMER_COUNT; ++it) {
    // Found a spot
    if(timers_[it] == nullptr) {
      timers_[it] = &timer;
      timersEnd_++;
      break;
    }
  }
}

void TimerTask::unregisterTimer(BasicTimer& timer)
{
  for(unsigned char it = 0; it < System::Timer::TIMER_COUNT; ++it) {
    // Found the timer
    if(timers_[it] == &timer) {
      // @Todo: we could stop when we find a nullptr
      for(;it < System::Timer::TIMER_COUNT - 1; ++it)
        timers_[it] = timers_[it + 1];
      timers_[it] = nullptr;
      timersEnd_--;
      break;
    }
  }  
}

void TimerTask::run()
{
  BasicTimer* timer = nullptr;
  
  while(timersIterator_ != timersEnd_) {
    timer = *(timersIterator_++);
    if(!timer->flags_.suspended) {
      if(timer->ticks_)
        timer->ticks_--;
      else {
        timer->flags_.suspended = true;
        timer->flags_.timedOut = true;
      }
    }
  }
  restoreTimersIterator();
}

/* BasicTimer */
BasicTimer::BasicTimer()
{
  ticks_ = 0;
  interval_ = 0;

  flags_.unit = TU_MS;
  flags_.timedOut = false;
  flags_.suspended = true;

  TimerTask::instance().registerTimer(*this);
}

BasicTimer::~BasicTimer()
{
  TimerTask::instance().unregisterTimer(*this);
}

void BasicTimer::start()
{
  ticks_ = timeToTicks(interval_, flags_.unit);
  flags_.timedOut = false;
  flags_.suspended = false;
}

void BasicTimer::stop()
{
  flags_.suspended = true;
}

void BasicTimer::resume()
{
  flags_.suspended = false;
}

void BasicTimer::restart()
{
  stop();
  start();
}

unsigned long BasicTimer::timeToTicks(unsigned int time, const enum TimeUnit& unit)
{
    unsigned long ticks = 0;
    switch(unit) {
      default:
      case TU_MS:   ticks = time;   break;
    }
    ticks /= System::Timer::EVENT_TIME;
    return ticks;
}
