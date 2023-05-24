#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>

/* for buffers */
#define WR_BUFFER_SIZE 1024
#define RD_BUFFER_SIZE 1024
#define FLUSH -1

/*for commmands with logical operator and command chaining */
#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* flags for itoa */
#define LOWERCASE 1
#define UNSIGNED 2


/* acessing the environment variables */
extern char **environ;


/**
 * struct list - singly linked list
 * @num: integer
 * @data: string
 * @next: pointer to the next node
 */
typedef struct list
{
	int num;
	char *data;
	struct list *next;
} list_t;


/**
 * struct shell_info - contains all information we need for our shell
 * @args: string generated from the getline
 * @path: contains path of the command
 * @fname: our program filename
 * @argv: tokenized string generated from args
 * @cmd_buf: buffer where the command gen from getine is stored
 * @environ: modified copy of the environment
 * @argc: number or argument/tokens
 * @err_num: error code/status for exit
 * @linecount_flag: this is on if the input is valid
 * @env_changed: on if enviroment was changed/updated
 * @status: return status of the last exit commnd
 * @cmd_buf_type: command type(CMD_OR, CMD_AND, CMD_CHAIN)...
 * @readfd: file descriptor to read from
 * @linecount: line count for each suceesful command used for errs
 * @env: copy of the env stored in a linked list
 * @alias: user defined alias are stored here
 */
typedef struct shell_info
{
	char *args;
	char *path;
	char *fname;
	char **argv;
	char **cmd_buf;
	char **environ;
	int argc;
	int err_num;
	int linecount_flag;
	int env_changed;
	int status;
	int cmd_buf_type;
	int readfd;
	unsigned int linecount;
	list_t *env;
	list_t *alias;
} shell_data;


/**
 * struct builtin - used to map out a buitin to respective fxn
 * @type: builtin command
 * @func: funtion pointer
 */
typedef struct builtin
{
	char *type;
	int (*func)(shell_data *);
} builtin_t;



/*
 * This initializes all the shell_data field to default value
 * and prevent the program from accessing uninitialized memory.
 */
#define SHELL_DATA_INIT \
{NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, \
	NULL, NULL}

/* string.c */
int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
int _strcmp(char *s1, char *s2);
size_t _strcspn(const char *src, const char *rej);

/* string1.c */
char *_strdup(const char *str);
void _memcpy(void *dest, const void *src, int n);
char *_memset(char *ptr, char val, unsigned int n);
char *_strncat(char *dest, char *src, int n);
char *_strncpy(char *dest, char *src, int n);

/* string2.c */
char *itoa(long int number, int base, int flags);
char *str_str(const char *haystack, const char *needle);
char *_strchr(char *s, char c);
char **_strtok(char *str, char *c);

/* stdio.c */
int _puts(char *str);
void putstr(char *str);
int putchr(char chr);

/* env.c */
int my_unsetenv(shell_data *data);
int my_env(shell_data *data);
int _setenv(shell_data *data, char *var, char *val);
char *_getenv(shell_data *data, const char *var);
int set_env_list(shell_data *data);

/* env1.c */
char **get_environ(shell_data *data);

/* linked_list.c */
char **list_to_strings(list_t *head);
list_t *add_node_end(list_t **head, const char *str, int num);
size_t print_list_data(const list_t *node);
int delete_nodeint(list_t **head, unsigned int indx);
ssize_t get_node_index(list_t *head, list_t *node);

/* linked_list1.c */
void free_list(list_t **head_adr);
size_t list_len(const list_t *h);

/* shell_loop.c */
void fork_cmd(shell_data *data);
void find_command(shell_data *data);
int prompt(shell_data *data, char **argv);

/* getline.c */
int _getline(shell_data *data, char **buffer, size_t *len);
void check_comments(char *buffer);
void sigint_handler(int sigint);
ssize_t input_buf(shell_data *data, char **buffer, size_t *len);
ssize_t get_input(shell_data *data);

/* getline1.c */
ssize_t read_line(shell_data *data, char *buffer, size_t *len);
void *_realloc(void *ptr, unsigned int old, unsigned int new);

/* boolean.c */
int is_command(shell_data *data, char *file_path);
int is_delimiter(char d, char *delimiter);
int is_interactive(shell_data *data);
int is_chain(shell_data *data, char *buffer, size_t *ptr);

/* functions.c */
int expand_alias(shell_data *data);
int expand_vars(shell_data *data);
void check_chain(shell_data *data, char *buffer, size_t *ptr, size_t i,
		size_t len);
void eff_puts(char *str);
int eff_putchr(char chr);

/* functions1.c */
char *dup_str(char *path, int start, int end);
char *find_path(shell_data *data, char *path, char *command);
int replace_var(char **var, char *val);
list_t *find_alias(list_t *node, char *command, char c);

/* data.c */
int free_cmd_buf(void **ptr);
void free_str_array(char **argv);
void free_data(shell_data *data, int all);
void clear_data(shell_data *data);
void set_data(shell_data *data, char **argv);

/* builtins1.c */
int my_setenv(shell_data *data);
int _unsetenv(shell_data *data, char *var);
int setalias(shell_data *data, char *s);
int unsetalias(shell_data *data, char *s);

/* builtins.c */
int printalias(list_t *node);
int my_alias(shell_data *data);
int my_exit(shell_data *data);
int my_cd(shell_data *data);
int check_builtin(shell_data *data);

/* errors.c */
int erratoi(char *str);
int put_num(int num, int fd);
void print_err(shell_data *data, char *str);


#endif
