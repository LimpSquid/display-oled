#include "kernel.h"
#include "sys_cfg.h"

class BasicTimer;

class TimerTask : public EventTask
{
  friend class BasicTimer;
  
public:
  static TimerTask& instance();

private:
  TimerTask();
  virtual ~TimerTask();

  void registerTimer(BasicTimer& timer);
  void unregisterTimer(BasicTimer& timer);
  
  virtual void run() override;

  BasicTimer** timersIterator_;
  BasicTimer** timersEnd_;

  BasicTimer* timers_[System::Timer::TIMER_COUNT];
};

class BasicTimer
{
  friend class TimerTask;
  
public:
  enum TimeUnit
  {
    TU_MS = 0,
    //TU_S
  };

  BasicTimer();
  virtual ~BasicTimer();

  enum TimeUnit unit() const { return static_cast<enum TimeUnit>(flags_.unit); }
  void setUnit(const enum TimeUnit& value) { flags_.unit = static_cast<unsigned char>(value); }

  unsigned int interval() const { return interval_; }
  void setInterval(int value) { interval_ = value; }

  bool timedOut() const { return flags_.timedOut; }

  void start();
  void stop();
  void resume();
  void restart();
  
protected:
  static unsigned long timeToTicks(unsigned int time, const enum TimeUnit& unit);

private:
  struct TimerFlags
  {
    unsigned char unit      :3;
    unsigned char timedOut  :1;
    unsigned char suspended :1;
    unsigned char reserved  :3;
  };
  
  unsigned long ticks_;
  unsigned int interval_;
  struct TimerFlags flags_;
};

