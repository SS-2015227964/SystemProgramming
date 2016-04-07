#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

ino_t   get_inode(char *);
void    printpathto(ino_t);
void    inum_to_name(ino_t,char *, int);

int main(){
        printpathto(get_inode("."));
        putchar('\n');
        return 0;
}

void printpathto(ino_t this_inode){
        ino_t   my_inode;
        char    its_name[BUFSIZ];
        char    full_path[BUFSIZ];
        char    temp[BUFSIZ];

        while(1){
                my_inode = get_inode(".");

                if (get_inode("..") != this_inode){

                        chdir("..");

                        inum_to_name(this_inode, its_name, BUFSIZ);
                        my_inode = get_inode(".");

                        this_inode = my_inode;

                        strcpy(temp,full_path);
                        sprintf(full_path, "/%s%s",its_name,temp);
                }
                else{
                        break;
                }
        }
        printf("%s", full_path);

}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen){

        DIR     *dir_ptr;
        struct dirent   *direntp;
        dir_ptr = opendir(".");
        if (dir_ptr == NULL){
                perror(".");
                exit(1);
        }

        while((direntp = readdir(dir_ptr)) != NULL){
                if (direntp->d_ino == inode_to_find){
                        strncpy(namebuf, direntp->d_name, buflen);
                        namebuf[buflen-1] = '\0';
                        closedir(dir_ptr);
                        return;
                }
        }

        fprintf(stderr, "Error looking for inum %d\n", inode_to_find);
        exit(1);
}

ino_t get_inode(char *fname){

        struct stat info;
        if(stat(fname, &info) == -1){
                fprintf(stderr,"Cannot stat ");
                perror(fname);
                exit(1);
        }
        return info.st_ino;
}

