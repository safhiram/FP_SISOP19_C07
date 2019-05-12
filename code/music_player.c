

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





