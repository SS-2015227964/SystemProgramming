#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <unistd.h>

int chkFile(char[]);            // 0-file, 1-dir, 2-none

int show_file_info(char *, struct stat *);
void mode_to_letters(int, char[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

void mvOverwrite(char *, char *);
void mvNewwrite(char *, char *);

main(int ac, char *av[]){

        int     file1 = 0,
                file2 = 0;      //handle filestat

        if(ac == 3){
                 if(strcmp(av[1], av[2]) == 0){      // both file and same
                        printf("'%s' and '%s' are the same file\n",av[1],av[2]);
                        return;
                }

                file1 = chkFile(av[1]);
                file2 = chkFile(av[2]);


                if(file1 == 2){ //no such file or dir
                        printf("'%s' No Such Original File\n",av[1]);
                        return;
                }

                else if(file1 == 0){                    //0 = file
                        if(file2 == 2){         //0,0 both file / file2 = 0 exist, file2 = 2 no
                                mvNewwrite(av[1], av[2]);
                                return;
                        }

                        else if(file2 == 0){         //0,0 both file / file2 = 0 exist, file2 = 2 no
                                mvOverwrite(av[1], av[2]);
                                return;
                        }

                        else if(file2 == 1){    //0,1 file & dir

                                char dirPath[256] = {0,};
                                int file3;

                                sprintf(dirPath,"%s/%s",av[2],av[1]);
                                file3 = chkFile(dirPath);

                                if (file3 == 2){
                                        mvNewwrite(av[1], dirPath);
                                        return;
                                }
                                else if(file3 == 0){
                                        mvOverwrite(av[1], dirPath);
                                        return;
                                }
                                return;
                        }
                }

                else if(file1 == 1){
                        printf("Error: CANNOT USE DIR AS ORGINAL SOURCE\n");
                        return;
                }
        }
        else{
                printf("Error: USE THIS COMMAND LIKE ./PE4 (oldLink) (newLink)\n");
                return;
        }
}

void mvOverwrite(char file1[], char file2[]){
        char typeL;

        printf("WOULD YOU WANT TO OVERWRITE %s? (YES: Y,y NO: N,n) ",file2);
        scanf("%c", &typeL);

        if(typeL = 'y' || 'Y'){
                int result = remove(file2);

                result = link(file1, file2);
                result = unlink(file1);

                printf("Move Succefully\n");
                return;
        }
        else if(typeL = 'n' || 'N'){
                return;
        }
        else{
                printf("Input Collect Value. (Y,y,N,n)\n");
                return;
        }

        return;
}


void mvNewwrite(char file1[], char file2[]){

        int result = link(file1,file2);

        if (result == -1){
                perror(file1);
                return;
        }
        result = unlink(file1);
        printf("Move Succefully\n");

        return;
}

int chkFile(char handName[]){

        struct stat info;

        if (stat(handName, &info) == -1){
                return 2;
        }

        if(show_file_info(handName, &info) == 0){       //0 = file
                return 0;
        }
        else{                                           //1 = dir
                return 1;
        }
}

int show_file_info(char *filename, struct stat *info_p){

        char    *uid_to_name(), *ctime(), *gid_to_name(), *filemod();
        void    mode_to_letters();
        char    modestr[11];

        mode_to_letters(info_p->st_mode, modestr);

        if(modestr[0] == 'd' && strcmp(filename,".") && strcmp(filename,"..")){
                return 1;
        }
        else{
                return 0;
        }

        return 0;
}


void mode_to_letters(int mode, char str[]){

        strcpy(str, "-----------");

        if (S_ISDIR(mode)){
                str[0] = 'd';
        }
        if (S_ISCHR(mode)){
                str[0] = 'c';
        }
        if (S_ISBLK(mode)){
                str[0] = 'b';
        }

        if (mode & S_IRUSR){
                str[1] = 'r';
        }
        if (mode & S_IWUSR){
                str[2] = 'w';
        }
        if (mode & S_IXUSR){
                str[3] = 'x';
        }

        if (mode & S_IRGRP){
                str[4] = 'r';
        }
        if (mode & S_IWGRP){
                str[5] = 'w';
        }
        if (mode & S_IXGRP){
                str[6] = 'x';
        }

        if (mode & S_IROTH){
                str[7] = 'r';
        }
        if (mode & S_IWOTH){
                str[8] = 'w';
        }
        if (mode & S_IXOTH){
                str[9] = 'x';
        }
}


char *uid_to_name(uid_t uid){
        struct  passwd *getpwuid(), *pw_ptr;

        static char numstr[10];

        if((pw_ptr = getpwuid(uid)) == NULL ){
                sprintf(numstr, "%d", uid);
                return numstr;
        }
        else{
                return pw_ptr->pw_name;
        }
}

char *gid_to_name(gid_t gid){
        struct  group *getgrgid(), *grp_ptr;
        static char numstr[10];

        if((grp_ptr = getgrgid(gid)) == NULL ){
                sprintf(numstr, "%d", gid);
                return numstr;
        }
        else{
                return grp_ptr->gr_name;
        }
}


