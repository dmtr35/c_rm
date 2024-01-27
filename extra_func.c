#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>

#include "func.h"


int is_directory(const char *path)
{
    DIR *dir = opendir(path);
    if(dir != NULL) {
        closedir(dir);
        return 1;           // Это директория
    } else {
        return 0;
    }
}

int file_exists(const char *path)
{
    return access(path, F_OK) != -1;
}


char* get_current_datatime(void)
{
    time_t rawtime;
    // printf("rawtime: %ld\n", rawtime);
    struct tm *timeinfo;
    char *buffer = (char *)malloc(20 * sizeof(char));     // Достаточно памяти для хранения даты и времен

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 20, "%d-%m-%Y_%H-%M-%S", timeinfo);

    return buffer;
}


char *replace_slashes_dash(char *dir_file)
{
    char length_dir_file = strlen(dir_file) + 1;
    char slash_or_dash[2];
    char first_symbol[2];
    char second_symbol[2];
    strncpy(slash_or_dash, dir_file, 1);
    size_t length_symbol = sizeof(slash_or_dash) / sizeof(*slash_or_dash);


    if(strstr(slash_or_dash, "/")){
        first_symbol[0] = '/';
        second_symbol[0] = '-';
    } else {
        first_symbol[0] = '-';
        second_symbol[0] = '/';
    }

    char *result = (char *)malloc(length_dir_file);

    for (int i = 0, j = 0; i < length_dir_file; ++i) {
        if (dir_file[i] == first_symbol[0]) {
            result[j++] = second_symbol[0];
        } else {
            result[j++] = dir_file[i];
        }
    }
    result[length_dir_file] = '\0';
    return result;
}


// _Bool check_dir(char *file_name, size_t name_size_tar_gz)
// {
//     _Bool res = 0;
//     char cut_file_name[4];
//     // file_name: это указатель на первый символ в строке file_name
//     // name_size_tar_gz: это количество символов в строке file_name
//     // сдвиг на -11символов от конца строки
//     strncpy(cut_file_name, file_name + name_size_tar_gz - 11, 3);
//     cut_file_name[3] = '\0';
//     // printf("%s\n", cut_file_name);

//     char *ptr = strstr(file_name, "dir");
//     if(ptr != NULL){
//         // printf("ptr: %s\n", ptr);
//         return 1;
//     }
//     return 0;
// }


void extractFileNameAndPath(char *fullFileName, char *path)
{
    const char *pattern = "____";
    char *position = strstr(fullFileName, pattern);

    size_t length = strlen(position);
    char *start = position + 24;
    char *end = position + length - 11;

    size_t len_file_path = end - start;
    strncpy(path, start, len_file_path);
    path[len_file_path] = '\0';
}





// void extractFileNameAndPath(const char *arg_token, char *file_name, char *dir_file)
// {
//     char *underscore = strstr(arg_token, "____");
//     // printf("underscore: %s\n", underscore);
//     // printf("arg_token: %s\n", arg_token);
//     // printf("underscore - arg_token: %ld\n", underscore - arg_token);
//     // printf("file_name: %s\n", file_name);


//     if(underscore != NULL){
//         strncpy(file_name, arg_token, underscore - arg_token);
//         file_name[underscore - arg_token] = '\0';
//         // printf("file_name: %s\n", file_name);
//     }

//     size_t length = strlen(underscore);
//     char *start = underscore + 24;
//     char *end = underscore + length - 11;

//     strncpy(dir_file, start, end - start);
//     dir_file[end - start] = '\0';
//     size_t length_dir_file = strlen(dir_file);
//     // printf("length_dir_file: %zu\n", length_dir_file);

//     char *res = replace_slashes_dash(dir_file);
//     // printf("res:2: %s\n", res);

// }
