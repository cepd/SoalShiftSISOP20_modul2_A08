#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include <curl/curl.h>

int main(char *argv[]) {
  //var to store PID
  pid_t pid, sid;
  //store PID of child process
  pid = fork();  
  
  //exit if fork() fail
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  //exit if PID from parent process
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
  
  //dir accessible
  umask(0);
  
  //prevent orphan process
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  //change dir
  if ((chdir("/home/dws/Documents/modul2/soal2/")) < 0) {
    exit(EXIT_FAILURE);
  }
  
  //close standar file descriptor
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  
  //programs inside daemon
  while (1) {    
    //var for name and path of dir 
    char drN[50];
    char dr[100] = {"/home/dws/Documents/modul2/soal2/"};

    //get timestamp and store to var
    time_t rTm = time(NULL);
    struct tm tmI = *localtime(&rTm);
    sprintf(drN,"%02d-%02d-%02d_%02d:%02d:%02d",tmI.tm_mday, tmI.tm_mon + 1, tmI.tm_year + 1900, tmI.tm_hour, tmI.tm_min, tmI.tm_sec);
    
    //create dir with timestamp name
    mkdir(drN,0777);

    //change dir into newly created
    strcat(dr,drN);
    chdir(dr);

    CURL *curl;
    
    long int nowEp;
    int size;

    FILE *f;
    char *url = "https://i.picsum.photos/id/";
    char img[50];
     
    //loop for download 20 images
    curl = curl_easy_init();
    int i;
    for(i=1; i<21; i++){

      time_t rTm = time(NULL);
      struct tm tmI = *localtime(&rTm);
      sprintf(drN,"%02d-%02d-%02d_%02d:%02d:%02d",tmI.tm_mday, tmI.tm_mon + 1, tmI.tm_year + 1900, tmI.tm_hour, tmI.tm_min, tmI.tm_sec);
      
      time_t ep = time(NULL);

      nowEp = ep;
      size = (nowEp%1000)+100;

      sprintf(img,"%s%d/%d/%d.jpg",url,i,size,size);

      f = fopen(strcat(drN,".jpg"), "wb");
      
      curl_easy_setopt(curl,CURLOPT_URL,img);
      curl_easy_setopt(curl,CURLOPT_WRITEDATA,f);
      curl_easy_setopt(curl,CURLOPT_FAILONERROR,1L);
     
      int res= curl_easy_perform(curl);
      if(res==CURLE_OK){
        printf("Download Successful!\n");
      }
      else printf("Error: %s\n",curl_easy_strerror(res));         
        
      fclose(f);
      //loop with interval 5s
      sleep(5);
    }
    curl_easy_cleanup(curl);

    //return to prev dir
    chdir("..");

    //loop with interval 30s
    sleep(30);
  };
}

