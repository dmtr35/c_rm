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
    // char absolute_path[1024];


    if(argc == 1) return 0;

    int res = check_func(ptr_user_data, flag_v);
    if(res == 1) return 1;
    // printf("user: %s\n", ptr_user_data->user);
    // printf("home: %s\n", ptr_user_data->home);
    // printf("dir_trash: %s\n", ptr_user_data->trash_directory);
    // printf("dir_current: %s\n", ptr_user_data->current_directory);
    

    for(int i = 1; i <= argc - 1; i++){
        total_length += strlen(argv[i]) + 1;
    }
    // printf("total_length: %d\n", total_length);


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
            // printf("absolute_path: %s\n", absolute_path);

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

    // printf("options: %s\n", options);
    // printf("arr_files:0: %s\n", arr_files);
    // printf("arr_files:0: %ld\n", strlen(arr_files));
    



    if(strcmp(options, "") == 0 || strcmp(options, "v") == 0){
        // printf("Option '' || 'v' is set\n");

        if(ptr_user_data->trash_directory != ptr_user_data->current_directory){
            flag_s = 1;
        }
        remove_files(arr_files, ptr_user_data, flag_v, flag_s);

    } else if(strstr(options, "r")){
        // printf("Option 'r' is set\n");

        remove_files(arr_files, ptr_user_data, flag_v, flag_s);

    } else if(strstr(options, "u")){
        // printf("Option 'u' is set\n");
        if(ptr_user_data->trash_directory == ptr_user_data->current_directory){        // ИЗМЕНИТЬ НА !=
            return 0;
        } else {
            restore(arr_files, ptr_user_data, flag_v, flag_s);
        }


    } else if(strcmp(options, "h") == 0){
        // printf("Option 'h' is set\n");
    } else if(strcmp(options, "d") == 0){
        // printf("Option 'd' is set\n");
    }




    free(ptr_user_data);
    free(arr_files);
    return 0;
}










