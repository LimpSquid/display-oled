#include "kernel.h"
#include "commtask.h"

class Main : public RoundRobinTask
{
public:
  Main();
  virtual ~Main() override;

private:
  virtual void addTasks(Kernel& kernel) override;
  virtual void run() override;

  CommTask commTask_;
};
