#ifndef FUNC_H
#define FUNC_H

struct user_data {
    char user[1024];
    char home[1024];
    char current_directory[1024];
    char trash_directory[1024];
};

// check_func.c
int check_func(struct user_data *ptr_user_data, _Bool flag_v);
// int check_func(char *ptr_user, char *ptr_home, char *ptr_current_directory, char *ptr_trash_directory, _Bool flag_v);


// extra_func.c
int is_directory(const char *path);
int file_exists(const char *path);
char* get_current_datatime(void);
char *replace_slashes_dash(char *file_path, size_t length_dir_file);
_Bool check_dir(char *file_name, size_t name_size_tar_gz);
void extractFileNameAndPath(const char *arg_token, char *file_name, char *dir_file);


// archive.c


// remove_files.c
void remove_one_file(const char *path, char *file_name, _Bool flag_v);
void remove_one_dir(const char *path, char *dir_name, _Bool flag_v);
int remove_directory_recursive(const char *path, char *file_name, _Bool flag_v);
void remove_files(char *arr_files, struct user_data *ptr_user_data, _Bool flag_v, _Bool flag_s);
void save_file(char *absolute_path, char *dir_name, char *file_name, struct user_data *ptr_user_data, _Bool flag_v);



#endif