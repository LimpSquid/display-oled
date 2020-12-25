#include "kernel.h"
#include <avr/io.h>

#define restoreRoundRobinTasksIterator()                \
          (roundRobinTasksIterator_ = roundRobinTasks_)
#define restoreEventTasksIterator()                     \
          (eventTasksIterator_ = eventTasks_)

/* KernelTask */
KernelTask::KernelTask()
{
  
}

KernelTask::~KernelTask()
{

}

/* RoundRobinTask */
RoundRobinTask::RoundRobinTask() :
  KernelTask()
{
  
}

RoundRobinTask::~RoundRobinTask()
{

}

/* EventTask */
EventTask::EventTask(unsigned int milliseconds) :
  KernelTask()
{
  interval_ = Kernel::msToTicks(milliseconds);
  ticks_ = interval_; // Immediatly execute our event
}

EventTask::~EventTask()
{

}

/* Kernel */
Kernel::Kernel()
{
  roundRobinTasksIterator_ = roundRobinTasks_;
  roundRobinTasksEnd_ = roundRobinTasks_;
  eventTasksIterator_ = eventTasks_;
  eventTasksEnd_ = eventTasks_;
  lastTickCount_ = 0;
  
  // Initialize our tables to NULL
  for(unsigned char it = 0; it < System::Kernel::TASK_COUNT; ++it) {
    eventTasks_[it] = nullptr;
    roundRobinTasks_[it] = nullptr;
  }
}

Kernel::~Kernel()
{
  
}

void Kernel::init()
{
  while(eventTasksIterator_ != eventTasksEnd_)
    (*(eventTasksIterator_++))->init();
  while(roundRobinTasksIterator_ != roundRobinTasksEnd_)
    (*(roundRobinTasksIterator_++))->init();
  
  restoreEventTasksIterator();
  restoreRoundRobinTasksIterator();
  
  // Lastly kickoff timer1
  initTimer();
}

void Kernel::execute()
{
  KernelTask* task = nullptr;
  EventTask* event = nullptr;

  // According to the datasheet, both registers are read atomically, so we shouldn't need to take care of this ourselves

  unsigned int currentTickCount = TCNT1;
  unsigned short elapsed = currentTickCount - lastTickCount_;
  lastTickCount_ = currentTickCount;

  while(eventTasksIterator_ != eventTasksEnd_) {
    event = *(eventTasksIterator_++);
    event->ticks_ -= elapsed;
    if(task == nullptr && event->ticks_ <= 0) {
      event->ticks_ = event->interval_;
      task = event;
    }
  }
  restoreEventTasksIterator();
  
  if(task == nullptr) {
    if(roundRobinTasksIterator_ == roundRobinTasksEnd_)
      restoreRoundRobinTasksIterator();
    else
      task = *(roundRobinTasksIterator_++);
  }

  if(task)
    task->run();
}

void Kernel::addTask(RoundRobinTask& task)
{
  task.addTasks(*this);

  for(unsigned char it = 0; it < System::Kernel::TASK_COUNT; ++it) {
    // Found a spot
    if(roundRobinTasks_[it] == nullptr) {
      roundRobinTasks_[it] = &task;
      roundRobinTasksEnd_++;
      break;
    }
  }
}

void Kernel::addTask(EventTask& task)
{
  task.addTasks(*this);

  for(unsigned char it = 0; it < System::Kernel::TASK_COUNT; ++it) {
    // Found a spot
    if(eventTasks_[it] == nullptr) {
      eventTasks_[it] = &task;
      eventTasksEnd_++;
      break;
    }
  }
}

unsigned long Kernel::msToTicks(unsigned int milliseconds)
{
  return (milliseconds * 1000LU) / SYS_TICK_US;
}

void Kernel::initTimer()
{
  TCNT1 = 0;
  TCCR1A = TCCR1A_REG;
  TCCR1B = TCCR1B_REG;
  TCCR1C = TCCR1C_REG;
  TIMSK1 = TIMSK1_REG;
}

