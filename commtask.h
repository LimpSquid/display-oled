#ifndef COMMTASK_H
#define COMMTASK_H

#include "kernel.h"

// @Todo: resolve all these stubs
  
class CommTask : public RoundRobinTask
{
public:
  CommTask() : RoundRobinTask() { }
  virtual ~CommTask() override { };  
};

#endif // COMMTASK_H
