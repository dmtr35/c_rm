#define _GNU_SOURCE                     // realpath (полный путь к файлу)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <libgen.h>


#include "func.h"


void remove_one_file(const char *path, char *file_name, _Bool flag_v)
{

    if(remove(path) == 0) {
        if(flag_v)
            printf("File \"%s\" removed\n", file_name);
    } else if(!file_exists(path)) {
        if(flag_v)
            printf("File %s don't exists\n", file_name);
    } else {
        if(flag_v)
            printf("Error delete file: \"%s\"\n", file_name);    
    }
}

void remove_one_dir(const char *path, char *dir_name, _Bool flag_v)
{
    if(remove(path) == 0) {
        if(flag_v)
            printf("Directory \"%s\" removed\n", dir_name);
    }else {
        if(flag_v)
            printf("Error delete directory: \"%s\"\n", dir_name);    
    }
}


int remove_directory_recursive(const char *path, char *file_name, _Bool flag_v)
{
    DIR *dir = opendir(path);

    if(dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry;               // прочитать содержимое директории

    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        printf("entry->d_name: %s\n", entry->d_name);

        char absolute_path[strlen(path) + strlen(entry->d_name) + 2];
        snprintf(absolute_path, sizeof(absolute_path), "%s/%s", path, entry->d_name);

        // printf("absolute_path: %s\n", absolute_path);
        // printf("entry->d_name: %s\n", entry->d_name);

        if(is_directory(absolute_path)) {
            // printf("dir\n");
            remove_directory_recursive(absolute_path, entry->d_name, flag_v);
        } else {
            // printf("fil\n");
            remove_one_file(absolute_path, entry->d_name, flag_v);
        }
    }

    closedir(dir);
    remove_one_dir(path, file_name, flag_v);
}


void save_file(char *absolute_path, char *dir_name, char *file_name, struct user_data *ptr_user_data, _Bool flag_v)
{


}


void remove_files(char *arr_files, struct user_data *ptr_user_data, _Bool flag_v, _Bool flag_s)
{
    char *token = strtok(arr_files, " ");             // разбивка строки на подстроки
    
    while(token != NULL) {
        char *absolute_path = malloc(1024);
        realpath(token, absolute_path);
        char *file_name = basename(token);
        char *copy_absolute_path = strdup(absolute_path);
        char *dir_name = dirname(copy_absolute_path);
        free(copy_absolute_path);

        if(flag_s) {
            save_file(absolute_path, dir_name, file_name, ptr_user_data, flag_v);
        }

        if(is_directory(absolute_path)) {
            remove_directory_recursive(absolute_path, file_name, flag_v);
        } else {
            remove_one_file(absolute_path, file_name, flag_v);
        }

        free(absolute_path);
        token = strtok(NULL, " ");
    }
}


















