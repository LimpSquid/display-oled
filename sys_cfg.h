#ifndef SYS_CFG_H
#define SYS_CFG_H

namespace System
{
  static const unsigned long CPU_FREQUENCY = 16000000LU;
  
  namespace Kernel
  {
    static const unsigned char TASK_COUNT = 10; // One setting for both round robin and event driven tasks
  };

  namespace Timer
  {
    static const unsigned char TIMER_COUNT = 10;
    static const int EVENT_TIME = 5; // In ms, this defines the resolution for the software timers
  };
};

#endif // SYS_CFG_H
