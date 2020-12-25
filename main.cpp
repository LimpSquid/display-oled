#include "main.h"

Main::Main() :
  RoundRobinTask()
{
  
}

Main::~Main()
{

}
 
void Main::addTasks(Kernel& kernel)
{
  kernel.addTask(commTask_);
}

/* We can actually see this as our actual main entry, we glue everything together here */
void Main::run()
{
  // @Todo: resolve this stub
}

