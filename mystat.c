#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <linux/limits.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



ssize_t readlink(const char * pathname, char *buf, size_t bufsiz);
void format_time(const struct timespec *ts, char *buf, size_t bufsize);

//function for changing mode in octal to symbolic representation
void modeToSymbolic(mode_t mode, char *symbolic) {
    char permissions[] = "rwxrwxrwx";

    for (int i = 0; i < 9; i++) {
        symbolic[i] = (mode & (1 << (8 - i))) ? permissions[i] : '-';
    }

    symbolic[9] = '\0';
}


int
main(int argc, char *argv[])
{
	//variables
	struct stat sb;
	char buf[1024];
       	struct passwd *pwd;
	struct group *grp;
	int owner_perm;
	int group_perm;
	int others_perm;

	//if no path is listed
        if (argc < 2) {
            fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

	//loops using argc but is used to loop through argv[] indices
	for(int i = 1; i < argc; i++){
		
		//grabs information about file using lstat()
		if(lstat(argv[i], &sb) == -1)	{
			perror("lstat");
			exit(EXIT_FAILURE);
		}
	
		//print statements for file information
		printf("File: %s\n", argv[i]);
		memset(buf, '\0', sizeof(buf));
		ssize_t len = readlink(argv[i], buf, 1023);
		buf[len]='\0';

		//printf("  Symbolic link: %s -> %s\n", argv[i] + 35, buf);
		
        	printf("  File type:                ");
        	switch (sb.st_mode & S_IFMT) {
       		 	case S_IFBLK:  printf("block device\n");            break;
        		case S_IFCHR:  printf("character device\n");        break;
        		case S_IFDIR:  printf("directory\n");               break;
        		case S_IFIFO:  printf("FIFO/pipe\n");               break;
        		case S_IFLNK:  
				       if(strcmp(buf, "DOES_NOT_EXIST") == 0)
					       printf("Symbolic link - with dangling destination\n");
				       else
					       printf("Symbolic link -> %s\n", buf);                 
				       break;
        		case S_IFREG:  printf("regular file\n");            break;
        		case S_IFSOCK: printf("socket\n");                  break;
        		default:       printf("unknown?\n");                break;
        	}

		
        	printf("  Device ID number:         %jxh/%jdd\n", 
			       	(uintmax_t) minor(sb.st_dev),
				(uintmax_t) minor(sb.st_dev));

        	printf("  I-node number:            %ju\n", (uintmax_t) sb.st_ino);

   		char symbolic[10]; // To hold the symbolic representation

    		// Check the file type
    		if (S_ISREG(sb.st_mode)) {
    		    symbolic[0] = '-';
    		} else if (S_ISDIR(sb.st_mode)) {
    		    symbolic[0] = 'd';
    		} else if (S_ISLNK(sb.st_mode)) {
    		    symbolic[0] = 'l';
    		} else if (S_ISBLK(sb.st_mode)) {
			symbolic[0] = 'b';
   		 } else if (S_ISCHR(sb.st_mode)) {
			symbolic[0] = 'c';
    		} else if (S_ISFIFO(sb.st_mode)) {
			symbolic[0] = 'p';
    		} else if (S_ISSOCK(sb.st_mode)) {
			symbolic[0] = 's';
    		} else {
    		    symbolic[0] = '?'; // Unknown file type
    		}

   		//converts octal to symbolic representation
   		modeToSymbolic(sb.st_mode & 0777, symbolic + 1);

		owner_perm = (sb.st_mode & S_IRWXU) >> 6;
		group_perm = (sb.st_mode & S_IRWXG) >> 3;
		others_perm = (sb.st_mode & S_IRWXO);

   		printf("  Mode:                     %s      (%o%o%o in octal)\n", symbolic, owner_perm, group_perm, others_perm);

        	printf("  Link count:               %ju\n", (uintmax_t) sb.st_nlink);
        

		pwd = getpwuid(sb.st_uid);
		grp = getgrgid(sb.st_gid);
		
		printf("  Owner Id:                 %s         (UID = %ju)\n", pwd->pw_name, (uintmax_t) sb.st_uid);
		printf("  Group Id:                 %s            (GID = %ju)\n", grp->gr_name, (uintmax_t) sb.st_gid);
 	        printf("  Preferred I/O block size: %jd bytes\n", (intmax_t) sb.st_blksize);
 	        printf("  File size:                %jd bytes\n", (intmax_t) sb.st_size);
        	printf("  Blocks allocated:         %jd\n", (intmax_t) sb.st_blocks);
		char time_buffer[80];
		format_time(&sb.st_atim, time_buffer, sizeof(time_buffer));
        	printf("  Last file access:	    %s\n", time_buffer);
		format_time(&sb.st_mtim, time_buffer, sizeof(time_buffer));
        	printf("  Last file modification:   %s\n", time_buffer);
		format_time(&sb.st_ctim, time_buffer, sizeof(time_buffer));
        	printf("  Last status change:       %s\n", time_buffer);
	}

       	exit(EXIT_SUCCESS);
}

void format_time(const struct timespec *ts, char *buf, size_t bufsize) {
	struct tm tm_result;
	localtime_r(&(ts->tv_sec), &tm_result);
	strftime(buf, bufsize, "%Y-%m-%d %H:%M:%S %z (%Z) %a (local)", &tm_result);
}



