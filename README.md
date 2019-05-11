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
code here
```

<h5>MUSIC PLAYER</h5>

