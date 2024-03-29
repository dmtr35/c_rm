#define _GNU_SOURCE // realpath (полный путь к файлу)
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

void remove_one_file(const char *path, char *file_name, _Bool flag_v, _Bool flag_s)
{
    // printf("path: %s\n", path);
    // printf("file_name: %s\n", file_name);

    if (remove(path) == 0) {
        if (flag_v) {
            if (flag_s) {
                printf("File \"%s\" saved & removed\n", file_name);
            } else {
                printf("File \"%s\" removed\n", file_name);
            }
        }
    } else {
        if (flag_v) {
            printf("Error deleting file: \"%s\"\n", file_name);
        }
    }
}

void remove_one_dir(const char *path, char *dir_name, _Bool flag_v, _Bool flag_s)
{
    if (remove(path) == 0) {
        if (flag_v)
            printf("Directory \"%s\" removed\n", dir_name);
    } else {
        if (flag_v)
            printf("Error delete directory: \"%s\"\n", dir_name);
    }
}

int remove_directory_recursive(const char *path, char *file_name, _Bool flag_v, _Bool flag_s)
{
    DIR *dir = opendir(path);

    if (dir == NULL){
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry; // прочитать содержимое директории

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char absolute_path[strlen(path) + strlen(entry->d_name) + 2];
        snprintf(absolute_path, sizeof(absolute_path), "%s/%s", path, entry->d_name);


        if (is_directory(absolute_path)) {
            remove_directory_recursive(absolute_path, entry->d_name, flag_v, flag_s);
        } else {
            remove_one_file(absolute_path, entry->d_name, flag_v, flag_s);
        }
    }

    closedir(dir);
    remove_one_dir(path, file_name, flag_v, flag_s);

    return 0;
}

void save_file(char *absolute_path, char *dir_name, char *file_name, struct user_data *ptr_user_data)
{
    char dir_or_file[4];
    int res = is_directory(absolute_path);
    if (res) {
        strcpy(dir_or_file, "dir");
    } else {
        strcpy(dir_or_file, "fil");
    }

    char *time = get_current_datatime();
    char *new_path = replace_slashes_dash(dir_name);


    size_t length_tar = strlen(file_name) + strlen(time) + strlen(new_path) + strlen(dir_or_file) + 14;
    char name_tar[length_tar];
    snprintf(name_tar, length_tar, "%s____%s_%s.%s.tar.gz", file_name, time, new_path, dir_or_file);
    size_t length_command_tar = strlen(name_tar) + strlen(absolute_path) + strlen(ptr_user_data->trash_directory) + 41;
    char command_tar[length_command_tar];
    snprintf(command_tar, length_command_tar, "tar -czf %s/%s --absolute-names %s 2>/dev/null", ptr_user_data->trash_directory, name_tar, absolute_path);

    system(command_tar);
}

void remove_files(char *arr_files, struct user_data *ptr_user_data, _Bool flag_v, _Bool flag_s)
{
    char *token = strtok(arr_files, " "); // разбивка строки на подстроки

    while (token != NULL)  {
        char *absolute_path = malloc(1024);
        realpath(token, absolute_path);
        char *file_name = token;

        char *copy_absolute_path = strdup(absolute_path);
        char *dir_name = dirname(copy_absolute_path);

        if (flag_s) {
            save_file(absolute_path, dir_name, file_name, ptr_user_data);
        }
        free(copy_absolute_path);

        if (is_directory(absolute_path)) {
            remove_directory_recursive(absolute_path, file_name, flag_v, flag_s);
        } else {
            remove_one_file(absolute_path, file_name, flag_v, flag_s);
        }

        free(absolute_path);
        token = strtok(NULL, " ");
    }
}

void restore(char *arr_files, struct user_data *ptr_user_data, _Bool flag_v, _Bool flag_s)
{
    char *token = strtok(arr_files, " ");

    while (token != NULL) {
        char *file_name = token;
        char file_path[strlen(token)];
        extractFileNameAndPath(file_name, file_path);
        char *new_path = replace_slashes_dash(file_path);

        size_t len_absolute_path = strlen(ptr_user_data->trash_directory) + strlen(file_name) + 2;
        char *absolute_path = malloc(len_absolute_path);
        snprintf(absolute_path, len_absolute_path, "%s/%s", ptr_user_data->trash_directory, file_name);

        size_t length = strlen(ptr_user_data->trash_directory) + strlen(file_name) + strlen(new_path) + 32;
        char command[length];
        snprintf(command, length, "tar -xzf %s/%s --absolute-names -C %s", ptr_user_data->trash_directory, file_name, new_path);
        if(system(command) == 0) {
            remove_one_file(absolute_path, file_name, flag_v, flag_s);
        }
        
        token = strtok(NULL, " ");
    };
}