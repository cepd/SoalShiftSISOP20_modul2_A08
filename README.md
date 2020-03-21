# SoalShiftSISOP20_modul2_A08

## Kelompok
- **Dwi Wahyu Santoso**  (05111840000120)
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
