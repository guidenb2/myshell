/*
myshell.h
Author: Ben Guiden
*/

/* Function prototypes */
void my_prompt();
void do_clear();
void do_quit();
void do_help();
void do_echo(char * tokens);
void do_dir();
char * curr_dir();
void do_cd();
void do_environ();
void set_env_var(char *var);