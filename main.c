#include <stdio.h>
#include <malloc.h>

#include "strings.h"

#define MAX_PATH 260
#define MAX_ERROR_MESSAGE 255

#define ERROR_TOO_LONG_PATH "path is too long"
#define ERROR_INCORECT_PATH "incorect path"
#define ERROR_INCORECT_PROTOCOL "incorect protocol"
#define ERROR_INCORECT_IP "incorect ip"
#define ERROR_INCORECT_DOMEN "incorect domen"

int input(char *line, char *delim);
void output(char **paths, size_t num);
int check(char *origin_path, char delim, size_t *error_index, char *message_error);
int check_ip(char *str, size_t *error_index, char *message_error);
int check_domen(char *str, size_t *error_index, char *message_error);
void to_windows(char **path);
void process(char *path, char delim);

int main()
{
    char path[MAX_PATH * 10];
    char delim;

    input(path, &delim);
    process(path, delim);

    return 0;
}

int input(char *path, char *delim)
{
    if (!path || !delim) {
        return -1;
    }
    
    printf("delim: ");
    scanf("%c", delim);

    printf("paths: ");
    scanf("%s", path);

    return 0;
}

void process(char *path, char delim)
{
    char message_error[MAX_ERROR_MESSAGE];
    size_t error_index = 0;

    size_t paths_count = scount(path, delim);

    char **correct_paths = malloc(sizeof(char *) * paths_count);
    size_t corrects = 0;

    char *token = stok(path, delim);
    for (int i = 0; i <= paths_count; i++) {
        if (!check(token, '/', &error_index, message_error)) {
            to_windows(&token);
            correct_paths[corrects++] = token;
        }
        token = stok(token + slen(token) + 1, delim);
    }

    output(correct_paths, corrects);

}

void to_windows(char **path)
{
    char *token = stok(*path, '/');
    token += slen(token);

    *path = token;
    *token = '\\';
    while(*token != '\0') {
        if (*token == '/') {
            *token = '\\';
        }
        token++;
    }
}

void output(char **paths, size_t num)
{
    printf("new paths: ");
    for (int i = 0; i < num; i++) {
        if (i != 0) printf("+");
        printf("%s", paths[i]);
    }
    printf("\n");
}

int check(char *origin_path, char delim, size_t *error_index, char *message_error)
{
    char *path = malloc((MAX_PATH + 1)* sizeof(char));
    scpy(path, origin_path);

    if (slen(path) > MAX_PATH) {
        *error_index = 260;
        scpy(message_error, ERROR_TOO_LONG_PATH);
        return -1;
    }
    if (scspn(path, "\\") < slen(path)) {
        *error_index = scspn(path, "\\");
        scpy(message_error, ERROR_INCORECT_PATH);
        return -1;
    }

    size_t index = 0;    
    char *token = stok(path, delim);
    if (scmp(token, "http:") != 0 && scmp(token, "smb:") != 0) {
        *error_index = 0;
        scpy(message_error, ERROR_INCORECT_PROTOCOL);
        return -1;
    }
    index += slen(token);

    token = stok(NULL, delim);
    token = stok(NULL, delim);
    index += 2;
    if (sspn(token, "0123456789.") == slen(token)) {
        if (check_ip(token, &index, message_error)) {
            *error_index = index;
            return -1;
        }
    } else if (scspn(token, "0123456789") == slen(token)) {
        if (check_domen(token, &index, message_error)) {
            *error_index = index;
            return -1;
        }
    } else {
        scpy(message_error, ERROR_INCORECT_DOMEN);
        *error_index = index + scspn(token, "0123456789");
        return -1;
    }
    return 0;
}

int check_ip(char *str, size_t *error_index, char *message_error)
{
    if (scount(str, '.') != 3) {
        scpy(message_error, ERROR_INCORECT_IP);
        return -1;
    }

    char *token = stok(str, '.');
    size_t index = 0;

    while (token) {
        int k = stoi(token);

        if (k < 0 || k > 255) {
            *error_index += index;
            scpy(message_error, ERROR_INCORECT_IP);
            return -1;
        }

        index += slen(token);
        token = stok(NULL, '.');
    }
    return 0;   
}

int check_domen(char *str, size_t *error_index, char *message_error)
{
    size_t levels = scount(str, '.');
    if (levels > 3) {
        scpy(message_error, ERROR_INCORECT_DOMEN);
        return -1;
    }

    char *token = stok(str, '.');
    size_t index = 0;

    while (levels > 0) {
        index += slen(token);
        token = stok(NULL, '.');
        levels--;
    }

    if (scmp(token, "ru") && scmp(token, "com") && scmp(token, "org")) {
        *error_index += index;
        scpy(message_error, ERROR_INCORECT_DOMEN);
        return -1;
    }
    return 0;   
}