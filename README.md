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

