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

