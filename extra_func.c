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
    struct tm *timeinfo;
    char *buffer = (char *)malloc(20 * sizeof(char));

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 20, "%d-%m-%Y_%H-%M-%S", timeinfo);

    return buffer;
}


char *replace_slashes_dash(char *dir_file)
{
    size_t length_dir_file = strlen(dir_file) + 1;
    char slash_or_dash[2];
    char first_symbol[2];
    char second_symbol[2];
    strncpy(slash_or_dash, dir_file, 1);


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

