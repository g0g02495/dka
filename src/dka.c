#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int isDirectoryExists(const char *path);
int keepAliveLoop();

int main(int argc, char *argv[])
{
  int secs = 300;
  int status = 0;

  if (argc < 2 || argc > 3)
  {
    printf("Usage: %s <target_directory> [wait_seconds]\n", argv[0]);
  }
  if (isDirectoryExists(argv[1]))
  {
    printf("Directory exists at path '%s'\n", argv[1]);
  }
  else
  {
    printf("Directory does not exists at path '%s'\n", argv[1]);
    status = 1;
  }
  if (!(argv[2]))
  {
    printf("Delay not specified. Using default value of 300 seconds\n");
    secs = 300;
  }
  if (status == 0)
  {
    printf("All set. Let's keep that drive spinning.\n");
    status = keepAliveLoop(argv[1], secs);
  }
  else
  {
    printf("Not starting keep-alive process.\n");
  }
  exit(status);
}

int isDirectoryExists(const char *path)
{
  if (access(path, F_OK) == 0)
    return 1;

  return 0;
}

int keepAliveLoop(const char *path, int secs)
{
  char command[200];
  int loopStatus=0;
  snprintf(command, sizeof(command), "dd if=%s bs=4096 skip=1024 count=1 status=none iflag=direct of=/dev/null", path);
  printf("Command: %s\n", command);
  do {
    loopStatus = system(command);
    if (loopStatus != 0)
    {
      perror("Error executing command");
      return 1;
    }
    else
    {
      printf("Disk-Keep-Alive loop is running. See you in %d seconds.\n",secs);
    }
    sleep(secs); // Sleep for 5 minutes
  } while (loopStatus == 0);
  return loopStatus;
}
