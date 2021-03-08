/*
myshell.h
Author: Ben Guiden
*/

/* Function prototypes */
void my_prompt();
void do_clear(char ** tokens, int background, int count);
int word_counter(char*  line);
void do_quit();
void do_help(char ** tokens, char * path, int background, int count);
void do_echo(char ** tokens, int count, int background);
void execute_echo(char ** args, int count, int background);
void do_dir(char ** tokens, int count, int background);
char * curr_dir();
void do_cd(char * dest);
void execute_cd(char ** tokens, int count, int background);
void do_environ();
void execute_environ(char ** tokens, int count, int background);
void set_env_var(char *var);
void do_pause();
void execute_pause(char ** tokens, int count, int background);
void fork_execute(char ** tokens, int count, int background);