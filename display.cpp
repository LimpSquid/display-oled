#include "display.h"
#include "i2c.h"

DisplayTask& DisplayTask::instance()
{
  static DisplayTask displayTask_;
  return displayTask_;
}

DisplayTask::DisplayTask() :
  RoundRobinTask()
{
  
}

DisplayTask::~DisplayTask()
{
  i2c_.stop();
}

void DisplayTask::init()
{
  i2c_.start();
}

void DisplayTask::run()
{
  
}
