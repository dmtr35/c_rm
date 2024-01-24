#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>


#include "func.h"


int check_func(struct user_data *ptr_user_data, _Bool flag_v)
// int check_func(char *ptr_user, char *ptr_home, char *ptr_current_directory, char *ptr_trash_directory, _Bool flag_v)
{
    uid_t euid = geteuid();               // Получаем UID


    if(euid == 0) {
        const char *sudo_user = getenv("SUDO_USER");

        if(sudo_user != NULL) {
            strcpy(ptr_user_data->user, sudo_user);
        }else{
            printf("%s not determined\n", sudo_user);
            return 1;
        }

    } else {
        const char *user = getenv("USER");
        if(user != NULL) {
            strcpy(ptr_user_data->user, user);
        } else {
            printf("%s not determined\n", user);
            return 1;
        }
    }
    
    // получаем домашнюю директорию
    struct passwd *pw;

    if((pw = getpwnam(ptr_user_data->user)) != NULL) {
        strcpy(ptr_user_data->home, pw->pw_dir);
        snprintf(ptr_user_data->trash_directory, strlen(ptr_user_data->home) + 11, "%s/.my_trash", ptr_user_data->home);
        // printf("pw:: %s\n", ptr_trash_directory);

        struct stat st = {0};
        if(stat(ptr_user_data->trash_directory, &st) == -1) {   // получение инфо из файла/директории ptr_user_data->trash_directory; &st - куда сохранится результат
            if(mkdir(ptr_user_data->trash_directory, 0700) != 0) {
                perror("Error crating .my_trash directory");
                return 1;
            } else {
                // printf("Directory %s created\n", ptr_user_data->trash_directory);
            }
        } else {
            // printf("Directory %s already exists\n", ptr_user_data->trash_directory);
        }
    } else {
        fprintf(stderr, "Unable to determine home directory\n");
        return 1;
    }


    // получаем текущюю директорию
    if (getcwd(ptr_user_data->current_directory, 1024) == NULL) {
        perror("Error getting current directory");
        return 1;
    }





}


