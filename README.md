<h4>KELOMPOK C07</h4>
<h5>Safhira Maharani        05111740000027</h5>
<h5>Mohammad Iman Affandy   05111740000129</h5>
<hr>

<h4>SOAL</h4>
<p>Buatlah sebuah music player dengan bahasa C yang memiliki fitur play nama_lagu, pause, next, prev, list lagu. Selain music player juga terdapat FUSE untuk mengumpulkan semua jenis file yang berekstensi .mp3 kedalam FUSE yang tersebar pada direktori /home/user. Ketika FUSE dijalankan, direktori hasil FUSE hanya berisi file .mp3 tanpa ada direktori lain di dalamnya. Asal file tersebut bisa tersebar dari berbagai folder dan subfolder. program mp3 mengarah ke FUSE untuk memutar musik.</p>
<p>Note: playlist bisa banyak, link mp3player</p>
<h4>JAWABAN</h4>

<p>Implementasi code untuk menyelesaikan masalah diatas ada dua file .c yaitu untuk fuse dan untuk music player. </p>
<p>Code fuse untuk mengimplementasikan fuse untuk mengumpulkan semua jenis file yang berekstensi .mp3 ke dalam fuse yang tersebar di direktori /home/safhiram. Hasil fuse adalah file ..mp3 yang ada di semua folder dan subfolder.</p>
<p>Code music player untuk mengimplementasikan music player yang terdiri dari beberapa thread untuk melakukan next, previous, play dan pause.</p>

<h5>FUSE</h5>
<p>1. Untuk membuat file sistem disini, program membutuhkan beberapa fungsi yaitu:</p>

* .getattr untuk mendapatkan jenis filenya
* .read untuk membaca file
* .write untuk mengedit file
* .utimesandstart untuk mengupdate waktu jika user mengedit
* .mknod untuk membuat file khusus
* .readdir untuk membaca setiap file
* .chown untuk menulis kepemilikan

```
static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.write		= xmp_write,
	.open		= xmp_open,
	.mknod		= xmp_mknod,
	.chown		= xmp_chown,
	.utimens	= xmp_utimeandstart,
};
```

<p>2. Buat fungsi pendukung</p>

* insert  : untuk memasukkan nilai ke array
* pop     : untuk menghapus nilai array[0]

```
void insert_queue(char input[1000])
{
		strcpy(semua[k],input);
		k++;
}

void pop_queue()
{
	strcpy(yangdipop,semua[0]);
    j=k-1;
    int i=0;
    while(j--)
	{
		memset(semua[i],0,sizeof(semua[i]));
		strcpy(semua[i],semua[i+1]);
        i++;
	}
	if(k==1)
	{
		memset(semua[0],0,sizeof(semua[0]));
	}
	k--;
}
```

<p>3. Di fungsi readdir.</p>

* awalnya open file /home/safhiram
* lalu cek apakah suatu file yang sedang di read berupa folder atau file
* jika berupa folder maka masukkan ke dalam queue. Nilai k akan bertambah. alamat folder tersebut ditambahkan ke queue paling belakang.
* jika berupa file. Cek apakah 4 huruf terakhir adalah ".mp3", jika ia maka nilai cek = 2.
* jika nilai cek=2 . Cek apakah file tersebut berada di root atau bukan. 
* jika berada di root , maka file langsung dipindahkan ke folder mount. Jika tidak lakukan pemindahan file tersebut ke root.
* Cek apakah nilai k lebih dari 0. Jika ia maka lakukan pop terhadap queue yang paling depan.
* lalu lakukan perulangan seperti pada step ke 1, sampai semua isi queue habis.

```
	insert_stack(fol);
	
	while(k>0)
	{
		char alamat[1024];
		DIR *dp;
		struct dirent *de;
		(void) offset;
		(void) fi;
		struct stat st;
		struct stat hm;
		
		pop_stack();
		strcpy(alamat,yangdipop);
		int hehe = strlen(aslinya);
		if(strcmp(alamat,".")==0 && strcmp(alamat,"..")==0 && (alamat[hehe+1] != '.') && (alamat[hehe+2] != '.'))
		{
			continue;
		}
		
		printf("alamat: %s\n",alamat);
		
		dp = opendir(alamat);
		if (dp == NULL)
			return -errno;
		

		while ((de = readdir(dp)) != NULL) {
					
			char name[1024];
			char sekarang[1024];
			int cek=0;
			memset(name,0,1024);
			memset(&st, 0, sizeof(st));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;
			
			strcpy(name,de->d_name);
			if(strcmp(name,".")!=0 && strcmp(name,"..")!=0 && (name[0] != '.') && (name[1] != '.'))
			{
				printf("alamat: %s, name: %s\n",alamat,name);
				char cekin[1024]={"/home/safhiram/"};
				if(strcmp(alamat,cekin)==0)
				{
					snprintf(sekarang, sizeof(sekarang),"%s%s",alamat,de->d_name);
				}
				else
				{
					snprintf(sekarang, sizeof(sekarang),"%s/%s",alamat,de->d_name);
				}
				stat(sekarang, &hm);
				printf("sekarang: %s\n",sekarang);
				if(S_ISDIR(hm.st_mode))
				{
					printf("masuk folder\n");
					int cur_length = strlen(sekarang);
					if((strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) && (sekarang[cur_length-1] != '.') && (sekarang[cur_length-2] != '.') && strcmp(de->d_name,"music")!=0)
					{
						printf("masuk stack\n");
						insert_stack(sekarang);
					}
				}
				else if(S_ISREG(hm.st_mode))
				{
					printf("masuk file\n");
					if(strcmp(name+strlen(name)-4,".mp3")==0)
					{
						printf("file mp3\n");
						cek=2;
					}
					
					if(cek==2)
					{
						//cari asal file itu - pwd
						printf("file mp3 suk\n");
						char ketemu[1024];
						memset(ketemu,0,sizeof(ketemu));
						int i,l=0;
						int j= strlen(sekarang);
						for (i=0;i<j;i++)
						{
							if(aslinya[i]!=sekarang[i])
							{
								ketemu[l]=sekarang[i];
								l++;
							}
						}
						printf("ketemu path tanpa root : %s\n",ketemu);	
						//kalo misal ada di root maka cek bernilai 1
						char dirootgak[10]={"/"};
						strcat(dirootgak,de->d_name);
						
						printf("dirootgak %s\n",dirootgak);
						
						if(strcmp(ketemu,dirootgak)==0)
						{
							printf("nama file sama kayak di root\n");
							cek=1;
						}
						memset(dirootgak,0,sizeof(dirootgak));
							
						if(cek==2)
							{
								printf("nama file tidak sama kayak di root\n");
								char pindah[1024];
								strcpy(bukandiroot[k_root],de->d_name);
								k_root++;
								strcpy(dimanaberada[k_kasih],sekarang);
								k_kasih++;
								snprintf(pindah, sizeof(pindah),"%s/%s",aslinya,de->d_name);
								printf("pindah : %s\n",pindah);
								if(fork()==0)
								{
									char copy[1024];
									snprintf(copy,sizeof(copy),"cp %s %s",sekarang,pindah);
								 	execlp("bash", "bash", "-c",  copy,NULL);
								}
								printf("melewati rename\n");
							}
							
						if(cek==1)
						{
							printf("file di mount\n");
							if (filler(buf, de->d_name, &st, 0))
							break;
						}
					}		
				}			
			}		
		}
	printf("selasai %d\n",k);
		closedir(dp);
```

<h5>MUSIC PLAYER</h5>
<p>terdapat beberapa fungsi yang dibuat yaitu</p>
* Play Song
 <p>fungsi ini berfungsi untuk melakukan play</p>
 
```
void *playSong(void *ptr){

    //char *songName = (char *)ptr;
    
    while(1){
        
		wait(NULL);                
        memset(songName,0,sizeof(songName));
        strcpy(songName,dirpathh);
        strcat(songName,song);

     if(strlen(song) > 0){
        
            songIsOver = 0;
            //cleanSong();

            size_t buffer_size;
            size_t done;
            int err;

            int driver;
            
            ao_sample_format format;
            int channels, encoding;
            long rate;

            FILE *songFile = fopen(songName,"r");
            if(songFile == NULL)
                continue;
            
            /*if(strlen(songName) < 4)
                exit(0);*/

            /* initializations */
            ao_initialize();
            driver = ao_default_driver_id();
            mpg123_init();
            mh = mpg123_new(NULL, &err);
            buffer_size = mpg123_outblock(mh);
            buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

            /* open the file and get the decoding format */
            mpg123_open(mh, songName);
            mpg123_getformat(mh, &rate, &channels, &encoding);

            /* set the output format and open the output device */
            format.bits = mpg123_encsize(encoding) * BITS;
            format.rate = rate;
            format.channels = channels;
            format.byte_format = AO_FMT_NATIVE;
            format.matrix = 0;
            dev = ao_open_live(driver, &format, NULL);

            /* decode and play */
            /*while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
                
                if(isPause == 0){
                    ao_play(dev, buffer, done);
                    //printf("not pause\n");
                }
                
                if(songIsOver){
                    break;
                }
            }*/

            while(1){
                if(isPause == 0){
                    if(mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
                        ao_play(dev, buffer, done);
                    }else break;
                }

                if(songIsOver){
                    break;
                }
            }

            cleanSong();
        }

    }
```

* List Song
<p>Fungsi ini berfungsi untuk melakukan list terhadap lagu yang ada</p>

```
void listSong(){
    
    DIR *dp;
	struct dirent *de;

	dp = opendir(dirpathh);
	if (dp == NULL)
		return;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0)
		    printf("%s\n",de->d_name);
    }

    closedir(dp);
    
}
```

* Inisialisasi Song
<p>Fungsi ini berfungsi untuk menyimpan nama file mp3 yang ada di folder fuse ke sebuah array of array yang berisi string</p>

```
void initSong(){
    DIR *dp;
	struct dirent *de;

	dp = opendir(path);
	if (dp == NULL)
		return;

    int i = 0;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0){
            //store the song
            strcpy(qSong[i],de->d_name);
            //printf("%s\n",qSong[i]);
            i++;
        }
		    
    }

    closedir(dp);
}
```

* FindSongIndex
<p>Fungsi ini berfungsi untuk mencari index dari lagu yang ada di folder tersebut</p>

```
int findSongIndex(char path_lagu[100]){
    DIR *dp;
	struct dirent *de;

	dp = opendir(path);
	if (dp == NULL)
		printf("Eror direktori \n");

    int count=0;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0){
            if(strcmp(path_lagu,de->d_name) == 0){
                return count;
            }
            count++;
        }
		    
    }

    closedir(dp);
    return -1;
}
```

* itunglagu
<p>Fungsi ini berfungsi untuk mengetahui jumlah lagu yang ada di folder fuse</p>

```
int itunglagu(){
    DIR *dp;
	struct dirent *de;

	dp = opendir(path);
	if (dp == NULL)
		printf("Eror direktori \n");

    int count=0;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0){
            count++;
        }
    }

    closedir(dp);
    return count;
}
```

* masukin
<p>Fungsi ini berfungsi untuk memasukkan input berupa play, next , previous atau pause</p>

```
void  masukin(){

    char masukan[100];  
    char pTemp[100];  

    while(1){

        printf("masukan input : ");
        memset(masukan,0,sizeof(masukan));
        memset(pTemp,0,sizeof(pTemp));
        
        //scanf("%s",masukan);
	if(scanf("%s",masukan)){};        

        for(int i=0; i < 4; i++){
            pTemp[i] = masukan[i];
        }
            
        if(strcmp(masukan,"pause")==0){
            Pause = 1;
           
        }else if(strcmp(masukan,"next")==0){

            selesai = 1;
            memset(lagu,0,sizeof(lagu));
            songIndex++;
            if(songIndex < 0){
                songIndex = itunglagu();
            }else if(songIndex > itunglagu() - 1){
                songIndex = 0;
            }
            
            //printf("%d\n",itunglagu());
            strcpy(lagu,qSong[songIndex]);
            Pause = 0;
          
            
            
            
        }else if(strcmp(masukan,"prev")==0){

            selesai = 1;
            memset(lagu,0,sizeof(lagu));
            songIndex--;
            if(songIndex < 0){
                songIndex = itunglagu() - 1;
            }
            
            
            strcpy(lagu,qSong[songIndex]);
            Pause = 0;
            

        }else if(strcmp(masukan,"list_lagu")==0){
            listSong();
        }else if(strcmp(pTemp,"play")==0 && Pause == 1 && strlen(masukan) < 5){

            Pause = 0;         

        }else if(strcmp(pTemp,"play")==0 && strlen(masukan) > 4){

            selesai= 1;
            memset(lagu,0,sizeof(lagu));
        
            
            for(int i=0; i+4 < strlen(masukan); i++){
                lagu[i] = masukan[i+4];

            }
            Pause = 0;
            
            songIndex = findSongIndex(lagu);
printf("%s\n",path_lagu);

        }else{
            printf("jenis input tidak ada\n");
        }

    }

}
```

* clean
<p>Fungsi ini untuk menghapus file yang diputar dan library yang dipakai yaitu mpg123</p>

```
void cleanSong(){

    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

}
```

<h3><b> THREAD </b></h3>


```



#define BITS 8
#include <sys/time.h>
#include <sys/wait.h>
#include <ao/ao.h>
#include <mpg123.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

void cleanSong();
void masukin();

char path[] = "/home/safhiram/bis/music";

unsigned char *buffer;
ao_device *dev;
mpg123_handle *mh;
char path_lagu[124];
int selesai= 0;

int songIndex;
char lagu[101];
int Pause = 0;
char qSong[102][102];

void *putar_lagu(void *ptr){

    while(1){
        
		wait(NULL);                
        memset(path_lagu,0,sizeof(path_lagu));
        strcpy(path_lagu,path);
        strcat(path_lagu,lagu);

     if(strlen(lagu) > 0){
        
            selesai = 0;
            //cleanSong();

            size_t buffer_size;
            size_t done;
            int err;

            int driver;
            
            ao_sample_format format;
            int channels, encoding;
            long rate;

            FILE *songFile = fopen(path_lagu,"r");
            if(songFile == NULL)
                continue;

            ao_initialize();
            driver = ao_default_driver_id();
            mpg123_init();
            mh = mpg123_new(NULL, &err);
            buffer_size = mpg123_outblock(mh);
            buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

      
            mpg123_open(mh, path_lagu);
            mpg123_getformat(mh, &rate, &channels, &encoding);

            format.bits = mpg123_encsize(encoding) * BITS;
            format.rate = rate;
            format.channels = channels;
            format.byte_format = AO_FMT_NATIVE;
            format.matrix = 0;
            dev = ao_open_live(driver, &format, NULL);
      while(1){
            if(Pause == 0){
             if(mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
           ao_play(dev, buffer, done);
                   }else break;
        }

           if(selesai){
         break;
                }
            }

            cleanSong();
        }

    }

}

void listSong(){
    
    DIR *dp;
	struct dirent *de;

	dp = opendir(path);
	if (dp == NULL)
		return;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0)
		    printf("%s\n",de->d_name);
    }

    closedir(dp);
    
}

int itunglagu(){
    DIR *dp;
	struct dirent *de;

	dp = opendir(path);
	if (dp == NULL)
		printf("Eror direktori \n");

    int count=0;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0){
            count++;
        }
    }

    closedir(dp);
    return count;
}

void initSong(){
    DIR *dp;
	struct dirent *de;

	dp = opendir(path);
	if (dp == NULL)
		return;

    int i = 0;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0){
          
            strcpy(qSong[i],de->d_name);
          
            i++;
        }
		    
    }

    closedir(dp);
}

int findSongIndex(char path_lagu[100]){
    DIR *dp;
	struct dirent *de;

	dp = opendir(path);
	if (dp == NULL)
		printf("Eror direktori \n");

    int count=0;

	while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name,"..") != 0 && strcmp(de->d_name,".") != 0){
            if(strcmp(path_lagu,de->d_name) == 0){
                return count;
            }
            count++;
        }
		    
    }

    closedir(dp);
    return -1;
}



void cleanSong(){

    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

}


int main(int argc, char *argv[])
{
    initSong();
    pthread_t thread[2];

    if(pthread_create(&thread[0],NULL,putar_lagu,NULL)){
        printf("gagal\n");
    }

    masukin();    
    
    return 0;
}

void  masukin(){

    char masukan[100];  
    char awal[100];  

    while(1){

        printf("masukan input : ");
        memset(masukan,0,sizeof(masukan));
        memset(awal,0,sizeof(awal));
        
        //scanf("%s",masukan);
	if(scanf("%s",masukan)){};        

        for(int i=0; i < 4; i++){
            awal[i] = masukan[i];
        }
            
        if(strcmp(masukan,"pause")==0){
            Pause = 1;
           
        }else if(strcmp(awal,"play")==0 && strlen(masukan) > 4){

            selesai= 1;
            memset(lagu,0,sizeof(lagu));
        
            
            for(int i=0; i+4 < strlen(masukan); i++){
                lagu[i] = masukan[i+4];

            }
            Pause = 0;
            
            songIndex = findSongIndex(lagu);
printf("%s\n",path_lagu);

        }else if(strcmp(masukan,"next")==0){

            selesai = 1;
            memset(lagu,0,sizeof(lagu));
            songIndex++;
            if(songIndex < 0){
                songIndex = itunglagu();
            }else if(songIndex > itunglagu() - 1){
                songIndex = 0;
            }
            
            //printf("%d\n",itunglagu());
            strcpy(lagu,qSong[songIndex]);
            Pause = 0;
          
            
            
            
        }else if(strcmp(masukan,"prev")==0){

            selesai = 1;
            memset(lagu,0,sizeof(lagu));
            songIndex--;
            if(songIndex < 0){
                songIndex = itunglagu() - 1;
            }
            
            
            strcpy(lagu,qSong[songIndex]);
            Pause = 0;
            

        }else if(strcmp(awal,"play")==0 && Pause == 1 && strlen(masukan) < 5){

            Pause = 0;         

        }else if(strcmp(masukan,"list_lagu")==0){
            listSong();
        }

        else{
            printf("jenis input tidak ada\n");
        }

    }

}




```

