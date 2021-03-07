/*
myshell.h
Author: Ben Guiden
*/

/* Function prototypes */
void my_prompt();
void do_clear(int background);
int word_counter(char*  line);
void do_quit();
void do_help(char ** tokens, int background, int count);
void do_echo(char ** tokens, int count, int background);
void execute_echo(char ** args, int count, int background);
void do_dir(char * dest, int background);
char * curr_dir();
void do_cd();
void do_environ();
void set_env_var(char *var);
void do_pause();
void execute_pause(int background);
void fork_execute(char ** tokens, int count, int background);