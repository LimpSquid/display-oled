#ifndef DISPLAY_H
#define DISPLAY_H

#include "kernel.h"

class I2c;

class DisplayTask : public RoundRobinTask
{
public:
  static DisplayTask& instance();
  
private:
  DisplayTask();
  virtual ~DisplayTask() override;

  virtual void init() override;
  virtual void run() override;

  I2c& i2c_;
};

#endif // DISPLAY_H
