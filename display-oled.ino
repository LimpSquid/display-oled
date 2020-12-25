#include "kernel.h"
#include "timer.h"
#include "display.h"
#include "main.h"

static Kernel kernel;
static Main main;

void setup()
{
  kernel.addTask(TimerTask::instance());
  kernel.addTask(DisplayTask::instance());
  kernel.addTask(main);
  kernel.init();
}

void loop()
{
  kernel.execute();
}
