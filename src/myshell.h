/*
myshell.h
Author: Ben Guiden
*/

/* Function prototypes */
void my_prompt();
void do_clear();
int word_counter(char*  line);
void do_quit();
void do_help();
void do_echo(char ** tokens, int count);
void do_dir(char * dest);
char * curr_dir();
void do_cd();
void do_environ();
void set_env_var(char *var);
void do_pause();
void execute(char ** tokens, int count, int background);