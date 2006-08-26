#ifndef __SHELL_H
#define __SHELL_H

#define CMD_EXIT	0
#define CMD_HELP	1
#define CMD_LS		2
#define CMD_CD		3
#define CMD_STAT	4

#define NCMDS		5

#ifdef __cplusplus
extern "C"{
#endif

void start_shell(char *mpoint);
void cmd_help();
void cmd_ls(char cmd[][256]);
void cmd_cd(char cmd[][256]);
void cmd_stat(char cmd[][256]);

#ifdef __cplusplus
}
#endif

#endif //__SHELL_H
