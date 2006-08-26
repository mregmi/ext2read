/**
 * Ext2read
 * File: shell.c
 **/
/**
 * Copyright (C) 2005 by Manish Regmi   (regmi dot manish at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 **/

 #include <stdio.h>
 #include <string.h>

 #include "ext2read.h"
 #include "shell.h"

extern struct sys sys;
struct ext2partition ext2part;

static char shell_prefix[256] = "/>";
static char cur_cmd[128][256];
static char cmd_cpy[512];

static const char *commands[NCMDS] = {
	  "exit",
	  "help",
	  "fsls",
	  "fscd",
	  "fsstat"
	  };

static INLINE void cmd_to_array(char *cmd)
{
	char delim[] = " ";
	char *option;
	int idx = 1, i;

	option = strtok(cmd, delim);
	if(!option)
	{
		strcpy(cur_cmd[0], cmd);
		return;
	}

	printf("%s\n", option);
	strcpy(cur_cmd[0], option);

	while((option = strtok(NULL, delim)) != NULL)
	{
		strcpy(cur_cmd[idx], option);
		printf("idx = %d %s\n",idx ,cur_cmd[idx]);
		idx++;
		if(idx >= 128)
			break;
	}

	cur_cmd[idx][0] = '\0';
	printf("idx = %d %s\n",idx ,cur_cmd[idx]);
}

int get_cur_command()
{
	static char command[512];
	int i;
	char *tmp;

    fgets(command, 512, stdin);
    tmp = strchr(command, '\n');
    if(tmp)
    		 *tmp = '\0';
    		 
    strcpy(cmd_cpy, command);
    cmd_to_array(command);
    
    for(i = 0; i < NCMDS; i++)
    {
 		if(!strcmp(commands[i], cur_cmd[0]))
			return i;
	}

	return -1;
}

void start_shell(char *mpoint)
{
	int cmd;

 	strcpy(shell_prefix, mpoint);
 	printf("%s", shell_prefix);
 	putchar('>');
 	while((cmd = get_cur_command()) != CMD_EXIT)
	{
		if(cmd == -1)
			system(cmd_cpy);

		switch(cmd)
		{
			case CMD_HELP:
				cmd_help();
				break;
			case CMD_LS:
				cmd_ls(cur_cmd);
				break;				
			case CMD_CD:
				cmd_cd(cur_cmd);
				break;				
			case CMD_STAT:
				cmd_stat(cur_cmd);
				break;
		}
		printf("%s%s>", shell_prefix, ext2part.path);
	}
}

