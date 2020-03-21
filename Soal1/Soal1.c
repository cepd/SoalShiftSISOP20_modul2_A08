#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

time_t currenttime = time(NULL);
struct tm tm = *localtime(&currenttime);

if(argc!=5){
	exit(EXIT_FAILURE);
}


int seconds = -1;
if(*argv[1]>'9' || *argv[1]<'0') exit(EXIT_FAILURE);
else if(*argv [1] != '*') seconds = atoi (argv[1]);

int minutes = -1;
if(*argv[2]>'9' || *argv[2]<'0') exit(EXIT_FAILURE);
else if(*argv [2] != '*') minutes = atoi (argv[2]);

int hours = -1;
if(*argv[3]>'9' || *argv[3]<'0') exit(EXIT_FAILURE);
else if(*argv [3] != '*') hours = atoi (argv[3]);

if(seconds<0 ||seconds>59) exit(EXIT_FAILURE);
if(minutes<0 ||minutes>59) exit(EXIT_FAILURE);
if(hours<0 ||hours>23) exit(EXIT_FAILURE);

  while (1) {
  //program
currenttime = time(NULL);
tm = *localtime(&currenttime);
if((tm.tm_sec == seconds || seconds == -1) && (tm.tm_min == minutes || minutes == -1) && (tm.tm_hour == hours || hours == -1)){
	if (fork() == 0) {
		char *argexec[] = {"bash", argv[4], NULL};
		execv("/bin/bash", argexec);
	}

    sleep(1);
  }
}
}
