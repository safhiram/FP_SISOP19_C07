#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

int k_root=0;
int k_kasih=0;
int k_asal=0;
int k=0;
int jk=0;
char semua[100][1000];
char dimanaberada[100][1000];
char bukandiroot[100][1000];
char asal[100][1000];
char yangdipop[1000];
char tidakbisadiakses[1000]={"/home/safhiram/bis/music"};
char pindah[1024];

void insert_stack(char input[10000]);
void pop_stack();

static const char *aslinya = "/home/safhiram";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	lstat(fol, stbuf);

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	
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
			if(strcmp(name,".")!=0 && strcmp(name,"..")!=0 && (name[0] != '.') && (name[1] != '.') && strcmp(de->d_name,"music")!=0)
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
								memset(pindah,0,sizeof(pindah));
								strcpy(bukandiroot[k_root],de->d_name);
								k_root++;
								strcpy(dimanaberada[k_kasih],sekarang);
								k_kasih++;
								snprintf(pindah, sizeof(pindah),"%s/%s",aslinya,de->d_name);
								strcpy(asal[k_asal],pindah);
								k_asal++;
								printf("pindah : %s\n",pindah);
								char apa[1024];
								snprintf(apa,sizeof(apa),"cp %s %s",sekarang,pindah);
								system(apa);
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
	}
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int res,fd;
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);

	(void) fi;
	fd=open(fol, O_RDONLY);
	res=pread(fd, buf, size, offset);
	close(fd);
	return res;
}


static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int res,fd;
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);

	(void) path;
	fd=open(fol, O_WRONLY);
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);

	res=open(fol, fi->flags);
	
	close(res);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	int res;
	if (S_ISREG(mode)) {
		res = open(fol, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fol, mode);
	else
		res = mknod(fol, mode, rdev);
		
	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	lchown(fol, uid, gid);
	return 0;
}

static int xmp_utimeandstart(const char *path, const struct timespec ts[2])
{
	struct timeval timevalue[2];
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);

	timevalue[0].tv_sec = ts[0].tv_sec;
	timevalue[0].tv_usec = ts[0].tv_nsec / 1000;
	timevalue[1].tv_sec = ts[1].tv_sec;
	timevalue[1].tv_usec = ts[1].tv_nsec / 1000;

	utimes(fol, timevalue);

	return 0;
}

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

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

void insert_stack(char input[1000])
{
		strcpy(semua[k],input);
		k++;
}

void pop_stack()
{
	strcpy(yangdipop,semua[0]);
	for (int i=0;i<k-1;i++)
	{
		memset(semua[i],0,sizeof(semua[i]));
		strcpy(semua[i],semua[i+1]);
	}
	if(k==1)
	{
		memset(semua[0],0,sizeof(semua[0]));
	}
	k--;
}

