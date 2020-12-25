#ifndef KERNEL_H
#define KERNEL_H

#include "sys_cfg.h"

class Kernel;

class KernelTask
{
  friend class Kernel;
  
public:
  KernelTask();
  virtual ~KernelTask();
  
protected:
  virtual void addTasks(Kernel& kernel) = 0;
  virtual void init() = 0;
  virtual void run() = 0;
  
  void* kernelData() const { return data_; }
  void setKernelData(void* value) { data_ = value; }
  
private:
  void* data_;
};

class RoundRobinTask : public KernelTask
{
  friend class Kernel;
  
public:
  RoundRobinTask();
  virtual ~RoundRobinTask() override;
  
protected:
  // Stubs
  virtual void addTasks(Kernel& kernel) override { }
  virtual void init() override { }
  virtual void run() override { }
};

class EventTask : public KernelTask
{
  friend class Kernel;
  
public:
  EventTask(unsigned int milliseconds);
  virtual ~EventTask() override;

protected:
  // Stubs
  virtual void addTasks(Kernel& kernel) override { }
  virtual void init() override { }
  virtual void run() override { }

private:
  unsigned long interval_;
  long ticks_;
};

class Kernel
{
public:
  Kernel();
  ~Kernel();
  
  void init();
  void execute();

  void addTask(RoundRobinTask& task);
  void addTask(EventTask& task);

  static inline unsigned long msToTicks(unsigned int milliseconds);

private:
  Kernel(const Kernel& other) = delete;

  void initTimer();

  static const unsigned char TCCR1A_REG = 0x00;
  static const unsigned char TCCR1B_REG = 0x03; // 64x prescaler
  static const unsigned char TCCR1C_REG = 0x00;
  static const unsigned char TIMSK1_REG = 0x00;

  static const unsigned char SYS_TICK_US = 64000000LU / System::CPU_FREQUENCY; // (prescaler * 1000000) / cpu_freq

  EventTask* eventTasks_[System::Kernel::TASK_COUNT];
  RoundRobinTask* roundRobinTasks_[System::Kernel::TASK_COUNT];

  RoundRobinTask** roundRobinTasksIterator_;
  RoundRobinTask** roundRobinTasksEnd_;

  EventTask** eventTasksIterator_;
  EventTask** eventTasksEnd_;

  unsigned short lastTickCount_;
};

#endif // KERNEL_H
