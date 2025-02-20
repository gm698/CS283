#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include "dshlib.h"

int last_rc = 0;

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer != NULL) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    if (cmd_buff->_cmd_buffer != NULL) {
        memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    }
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);
    if (alloc_cmd_buff(cmd_buff) != OK) {
        return ERR_MEMORY;
    }
    strcpy(cmd_buff->_cmd_buffer, cmd_line);

    char *str = cmd_buff->_cmd_buffer;
    char *start = str;
    int arg_index = 0;
    bool in_quotes = false;

    while (*str) {
        if (*str == '"') {
            in_quotes = !in_quotes;
            memmove(str, str + 1, strlen(str)); // Remove the quote
            continue;
        }

        if (!in_quotes && isspace((unsigned char)*str)) {
            if (str > start) {  // End of argument
                *str = '\0';
                cmd_buff->argv[arg_index++] = start;
            }
            start = str + 1;
        }

        str++;
    }

    if (start != str) {  // Last argument
        cmd_buff->argv[arg_index++] = start;
    }

    cmd_buff->argc = arg_index;
    cmd_buff->argv[arg_index] = NULL;

    return OK;
}


Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "rc") == 0) {
        return BI_RC;
    }
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds cmd_type = match_command(cmd->argv[0]);

    if (cmd_type == BI_CMD_EXIT) {
        exit(0);
    } else if (cmd_type == BI_CMD_CD) {
        if (cmd->argc > 1) {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd");
            }
        }
        return BI_EXECUTED;
    } else if (cmd_type == BI_RC) {
        printf("%d\n", last_rc);
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}

int exec_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0], "echo") == 0) {
        if (cmd->argc > 1) {
            for (int i = 1; i < cmd->argc; i++) {
                printf("%s", cmd->argv[i]);
                if (i < cmd->argc - 1) {
                    printf(" ");
                }
            }
            printf("\n");
        } else {
            printf("\n");
        }
        return OK;
    }
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return ERR_EXEC_CMD;
    } else if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        last_rc = errno;
        exit(errno);
    } else {
        int status;
        waitpid(pid, &status, 0);
        last_rc = WEXITSTATUS(status);
        if (last_rc != 0) {
            if (last_rc == ENOENT) {
                printf("Command not found in PATH\n");
            } else if (last_rc == EACCES) {
                printf("Permission denied\n");
            } else {
                printf("Error executing command\n");
            }
        }
        return OK;
    }
}

int exec_local_cmd_loop() {
    char cmd_line[SH_CMD_MAX];
    cmd_buff_t cmd;

    printf("%s", SH_PROMPT); // Print the initial prompt

    while (1) {
        if (fgets(cmd_line, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_line[strcspn(cmd_line, "\n")] = '\0';

        if (strlen(cmd_line) == 0) continue;

        if (build_cmd_buff(cmd_line, &cmd) != OK) {
            fprintf(stderr, "Error building command buffer\n");
            continue;
        }

        if (cmd.argc == 0) {
            continue;
        }

        if (exec_built_in_cmd(&cmd) == BI_NOT_BI) {
            exec_cmd(&cmd);
        }

        free_cmd_buff(&cmd);
        printf("%s", SH_PROMPT); // Print prompt for the next command
    }
    return OK;
}