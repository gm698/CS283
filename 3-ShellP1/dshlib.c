#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist) {
    clist->num = 0;
    char *cmd_token;
    char *saveptr1;
    char *arg_token;
    char *saveptr2;

    cmd_token = strtok_r(cmd_line, PIPE_STRING, &saveptr1);
    while (cmd_token != NULL && clist->num < CMD_MAX) {
        // Remove leading/trailing spaces from command token
        while (isspace((unsigned char)*cmd_token)) cmd_token++;
        char *end = cmd_token + strlen(cmd_token) - 1;
        while (end > cmd_token && isspace((unsigned char)*end)) end--;
        end[1] = '\0';


        arg_token = strtok_r(cmd_token, " ", &saveptr2);
        if (arg_token != NULL) {
            strncpy(clist->commands[clist->num].exe, arg_token, EXE_MAX - 1);
            clist->commands[clist->num].exe[EXE_MAX - 1] = '\0';

            char args_str[ARG_MAX] = "";
            arg_token = strtok_r(NULL, " ", &saveptr2);
            while (arg_token != NULL) {
                strncat(args_str, arg_token, ARG_MAX - strlen(args_str) - 1);
                strncat(args_str, " ", ARG_MAX - strlen(args_str) - 1);

                arg_token = strtok_r(NULL, " ", &saveptr2);

            }
            if (strlen(args_str) > 0) {
              args_str[strlen(args_str) - 1] = '\0'; // Remove trailing space
            }

            strncpy(clist->commands[clist->num].args, args_str, ARG_MAX - 1);
            clist->commands[clist->num].args[ARG_MAX - 1] = '\0';

        } else {
          strcpy(clist->commands[clist->num].exe, "");
          strcpy(clist->commands[clist->num].args, "");

        }



        clist->num++;
        cmd_token = strtok_r(NULL, PIPE_STRING, &saveptr1);
    }

    if (clist->num == 0) {
        return WARN_NO_CMDS;
    } else if (cmd_token != NULL) {
        return ERR_TOO_MANY_COMMANDS;
    } else {
        return OK;
    }
}