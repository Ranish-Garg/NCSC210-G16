#include "kernel/types.h"
#include "user/user.h"

static void
logmsg(const char *who, const char *msg)
{
  printf("T+%d %s: %s\n", uptime(), who, msg);
}

int
main(void)
{
  int pid = fork();

  if(pid < 0){
    logmsg("demo", "fork failed");
    exit(1);
  }

  if(pid == 0){
    int i;
    for(i = 0; i < 8; i++){
      logmsg("child", "running");
      pause(15);
    }
    logmsg("child", "done");
    exit(0);
  }

  pause(30);
  if(freeze(pid) == 0){
    logmsg("parent", "child frozen");
  } else {
    logmsg("parent", "freeze failed");
  }

  pause(80);
  // Print before thaw so parent/child logs do not overlap at thaw instant.
  logmsg("parent", "thawing child");
  if(thaw(pid) == 0){
    // Keep parent quiet until wait() returns to avoid mixed output with child.
  } else {
    logmsg("parent", "thaw failed");
  }

  wait(0);
  logmsg("parent", "demo complete");
  exit(0);
}
