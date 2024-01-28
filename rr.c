#define _GNU_SOURCE     // realpath (полный путь к файлу)
#include <stdio.h>
#include <unistd.h>    // getcwd - определение текущей директории
#include <getopt.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "func.h"


int is_tar_installed(void)
{
    return system("which tar > /dev/null 2>&1") == 0;
}


int main(int argc, char *argv[])
{
    struct user_data user_data;
    struct user_data *ptr_user_data = malloc(sizeof(struct user_data));


    int total_length = 0;
    char options[3] = "";
    _Bool flag_v = 0;
    _Bool flag_s = 0;


    if(argc == 1) return 0;

    int res = check_func(ptr_user_data, flag_v);
    if(res == 1) return 1;
    

    for(int i = 1; i <= argc - 1; i++){
        total_length += strlen(argv[i]) + 1;
    }


    char *arr_files = calloc(total_length, sizeof(char));
    if(arr_files == NULL)
    {
        perror("Memory allocation error");
        return 1;
    }

    int index = 0;
    for(int i = 1; i < argc; i++){
        if(argv[i][0] == '-') {
            strcat(options, argv[i] + 1);               // обьединение опций, удаляя '-'
            continue;
        }
        

        if(strchr(options, 'v') != 0) {
            flag_v = 1;

            size_t len_absolute_path = strlen(ptr_user_data->current_directory) + strlen(argv[i]) + 1;
            char *absolute_path = malloc(len_absolute_path);
            realpath(argv[i], absolute_path);

            if(!file_exists(absolute_path)) {
                printf("File %s don't exists\n", argv[i]);
                free(absolute_path);
                continue;
            }
            free(absolute_path);
        }

        
        strcpy(arr_files + index, argv[i]);
        index += strlen(argv[i]);
        if (i < argc - 1) {
            arr_files[index++] = ' ';
        }
    }
    arr_files[index] = '\0';




    if(strcmp(options, "") == 0 || strcmp(options, "v") == 0){

        if(strcmp(ptr_user_data->trash_directory, ptr_user_data->current_directory) != 0){
            flag_s = 1;
        }
        remove_files(arr_files, ptr_user_data, flag_v, flag_s);

    } else if(strstr(options, "r")){

        remove_files(arr_files, ptr_user_data, flag_v, flag_s);

    } else if(strstr(options, "u")){
        if(strcmp(ptr_user_data->trash_directory, ptr_user_data->current_directory) != 0){
            if(flag_v){
                printf("File can only be recovered from garbage");
            }
            return 0;
        } else {
            restore(arr_files, ptr_user_data, flag_v, flag_s);
        }

    } else if(strcmp(options, "h") == 0){
        printf("DESCRIPTION\n");
        printf("put program in /usr/bin/rm/\n");
        printf("\n");
        printf("SYNOPSIS\n");
        printf("rr [OPTION]... [FILE]... \n");
        printf("\n");
        printf("OPTIONS\n");
        printf("the \"rr\" command deletes the file, saving it in /home/<user>/.my_trash, or restores  it with the same command\n");
        printf("\n");
        printf("-r: without saving the file, normal deletion without recovery\n");
        printf("\n");
        printf("-v: receive a report on completed work\n");
        printf("\n");
        printf("-u: restore file or directory from /$HOME/.my_trash\n");
        printf("\n");
        printf("-h: help\n");
    } 




    free(ptr_user_data);
    free(arr_files);
    return 0;
}









