#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

int main() {
  pid_t child, child1, child2, child3, child4, child5, child6;
  int status;

child = fork();
  
  if (child == 0) {
    printf("creating folder indomie\n\n");
    char *argv[] = {"mkdir", "-p", "modul2/indomie", NULL};
    execv("/bin/mkdir", argv);
  }

child1 = fork(); 

  if (child1 == 0){
    sleep(5);
    printf("creating folder sedaap\n\n");
    char *argv[] = {"mkdir", "-p", "modul2/sedaap", NULL};
    execv("/bin/mkdir", argv);
  }

child2 = fork();

  if (child2 == 0) {
    printf("unzip jpg\n\n");
    char *argv[] = {"unzip", "Downloads/jpg.zip", "-d", "modul2/", NULL};
    execv("/usr/bin/unzip", argv);
  }

waitpid(child, &status, 0);
waitpid(child1, &status, 0);
waitpid(child2, &status, 0);
child3 = fork();
  
  if(child3 == 0){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("modul2/jpg/")) != NULL){
      while((ent = readdir (dir)) != NULL){
        char folder_name[] = "modul2/jpg/";
        strcat(folder_name, ent->d_name);
      
        if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){
          continue;
        }
       
        else if(ent->d_type == DT_DIR){
          child4 = fork();
          
          if(child4 == 0){
            char *argv[] = {"mv", folder_name, "modul2/indomie/", NULL};
            execv("/bin/mv", argv);
          }
          waitpid(child4, &status, 0);
          printf("folder %s has been transfered\n\n", folder_name);
          char folder_name2 [] = "modul2/indomie/";
          strcat(folder_name2, ent->d_name);
          child5 = fork();
          
          if(child5 == 0){
            char file_name [] = "a";
            strcpy(file_name, folder_name2);
            strcat(file_name, "/coba1.txt");
            printf("membuat %s\n", file_name);
            char *argv[] = {"touch", file_name, NULL};
            execv("/usr/bin/touch", argv);
          }
          waitpid(child5, &status, 0);
          child6 = fork();
          
          if(child6 == 0){
            sleep(3);
            char file_name2 [] = "a";
            strcpy(file_name2, folder_name2);
            strcat(file_name2, "/coba2.txt");
            printf("membuat %s\n", file_name2);
            char *argv[] = {"touch", file_name2, NULL};
            execv("/usr/bin/touch", argv);
          } 
          waitpid(child6, &status, 0); 
        }
        
        else if(ent->d_type == DT_REG){
          child4 = fork();
          if(child4 == 0){
            char *argv[] = {"mv", folder_name, "modul2/sedaap/", NULL};
            execv("/bin/mv", argv);
          }
          printf("file %s has been transfered\n\n", folder_name);
        }
      }
      closedir(dir);
      printf("done");
    }
  }
  return 0;
}
