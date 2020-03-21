# SoalShiftSISOP20_modul2_A08

## Kelompok
- **Dwi Wahyu Santoso**  (05111840000121)
- **I Gusti Agung Chintya Prema Dewi** (05111840000130)
### Pembahasan Nomor 1 

**soal**
> Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:
> - a. Program menerima 4 argumen berupa:
>i. Detik: 0-59 atau * (any value)
>ii. Menit: 0-59 atau * (any value)
>iii. Jam: 0-23 atau * (any value)
>iv. Path file .sh
> - b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai
> - c. Program hanya menerima 1 config cron
> - d. Program berjalan di background (daemon)
> - e. Tidak boleh menggunakan fungsi system()

**jawaban**
``` c
int main(int argc, char *argv[])
```
argc dan argv ini merupakan penerima informasi dari command line menuju program, argc dan argv berperan aktif untuk memberikan argument yang diberikan user melalui command line. Argv merupakan array of yang berisikan pointer of char, dalam penerapannya, argv ini dapat dipanggil seperti two dimensional array. 

```c
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
```
Dengan menggunakan local time dalam library time.h kita dapat menemukan waktu local tempat kita berada sekarang. Ketika arugmen yang diberikan jumlahnya tidak lima maka program akan memberikan output error.  Pendefinisian 'detik', 'menit', dan 'jam' dengan value -1 dilakukan agar inputan asterisk yang diberikan oleh user dapat dengan mudah dideteksi karena sudah bernilai default sebesar -1. Setiap detik menit dan jam juga dicek agar apakah sudah memenuhi formatnya masing-masing. 

```c
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
```

Program ini akan melakukan pengecekan tiap detik, apakah waktu sekarang sama dengan waktu yang diminta oleh user (mirip seperti cara kerja cron). Oleh karena itu, condition yang terdapat di dalam *if* bernilai true apabila:
1.	Waktunya sesuai dengan waktu sekarang, atau
2.	User meng-input asterisk ( * ). 
Bagaimana dengan *if (fork( ) == 0)* Kita tidak ingin itu terjadi karena harusnya program kita tetap berjalan hingga user melakukan kill. Jadi, penggunaan fork tersebut diperlukan agar fungsi execv berjalan dan menyelesaikan process child saja. *sleep (1)* digunakan untuk mengatur agar fungsi while berjalan setiap satu detik.



### Pembahasan nomor 2

**soal**
> Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia
merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat
sebuah program.
> - a. Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat
sebuah program C yang per 30 detik membuat sebuah folder dengan nama
timestamp [YYYY-mm-dd_HH:ii:ss].
> - b. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari
https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap
gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-
mm-dd_HH:ii:ss].
> - c. Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan
folder akan di delete(sehingga hanya menyisakan .zip).
> - d. Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-
generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.
> - e. Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu
MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan
dengan argumen 
-a. Untuk MODE_B, program harus dijalankan dengan argumen
-b. Ketika dijalankan dalam MODE_A, program utama akan langsung
menghentikan semua operasinya ketika program killer dijalankan. Untuk
MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi
membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua
folder terisi gambar, terzip lalu di delete).

**jawaban**

```c
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
```
Program ini berjalan sebagai daemon. Dengan fork() ada dua cabang parent dan child. Progam berada di child process yang parent processnya sudah mati. Sehingga child process akan menjadi orphan process. Namun, dengan setsid() membuat child process tersebut tidak menjadi orphan process.
Dalam daemon terdapat proses membuat directory dan mengunduh file gambar. Setiap 30 detik directory akan dibuat. Program masuk ke directory yang baru saja dibuat. Setiap 5 detik program akan mengunduh gambar dan disimpan dengan nama waktu sekarang. setelah semua 20 gambar selesai diunduh, program keluar dari directory tersebut. Program membuat directory baru dan seterusnya berulang seperti proses sebelumnya.
Proses download menggunakan library curl. Pada proses download, program membuat dahulu file dengan nama waktu sekarang dan dengan format .jpg. Kemudian program mengunduh dan menuliskan hasil unduhannya ke file tersebut sesuai dengan size yang telah diatur sebelumnya.


### Pembahasan Soal 3
**soal**

> Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia
memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan
(multiprocessing).
> - a. Program buatan jaya harus bisa membuat dua direktori di “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
> - b. Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori “/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya.
> - c. Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke “/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke “/home/[USER]/modul2/indomie/”.
> - d. Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/” harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu 3 detik kemudian membuat file bernama “coba2.txt”. (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”). Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat program tersebut.

**jawaban**

Membuat Folder Indomie
```c
child = fork();
  if (child == 0) {
    printf("creating folder indomie\n\n");
    char *argv[] = {"mkdir", "-p", "modul2/indomie", NULL};
    execv("/bin/mkdir", argv);
  }
```

Membuat Folder  Sedaap
```c
child1 = fork(); 
  if (child1 == 0){
    sleep(5);
    printf("creating folder sedaap\n\n");
    char *argv[] = {"mkdir", "-p", "modul2/sedaap", NULL};
    execv("/bin/mkdir", argv);
  }
```
*sleep(5)* berfungsi untuk menunggu 5 detik sebelum membuat directory sedaap

Men-unzip file
```c
child2 = fork();

  if (child2 == 0) {
    printf("unzip jpg\n\n");
    char *argv[] = {"unzip", "Downloads/jpg.zip", "-d", "modul2/", NULL};
    execv("/usr/bin/unzip", argv);
  }
```

Menunggu child proses selesai
```c
waitpid(child, &status, 0);
waitpid(child1, &status, 0);
waitpid(child2, &status, 0);
```

Membuka directory
```c
 if ((dir = opendir("modul2/jpg/")) != NULL)
```

Mengecek jika tikenya directory, maka directory tersebut akan dipindahkan ke directory indomie melalui process child 4 dengan menggunakan *exec mv*
```c
  if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){
          continue;
        }
  else if(ent->d_type == DT_DIR){
          child4 = fork();
```

Membuat File coba1.txt
```c
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
```

Membuat file coba2.txt
```c
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
```

Memindahkan file ke sedaap
```c
        else if(ent->d_type == DT_REG){
          child4 = fork();
          if(child4 == 0){
            char *argv[] = {"mv", folder_name, "modul2/sedaap/", NULL};
            execv("/bin/mv", argv);
          }
          printf("file %s has been transfered\n\n", folder_name);
        }
```
