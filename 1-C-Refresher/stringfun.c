#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

// Function prototypes
void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);
int count_words(char *, int, int);
void reverse_string(char *, int);
void print_Words(char *, int);

/**
 * @brief Processes the user-provided string into a fixed-size buffer.
 *        Removes extra spaces and fills unused buffer space with periods.
 * 
 * @param buff Pointer to the destination buffer
 * @param user_str Pointer to the user-provided input string
 * @param len Maximum length of the buffer
 * @return int Length of the processed string, -1 if input is too large, -2 if input is invalid
 */
int setup_buff(char *buff, char *user_str, int len) {
    if (!buff || !user_str || len <= 0) {
        return -2;
    }

    char *dst = buff;
    char *src = user_str;
    int char_count = 0;
    int in_whitespace = 0;

    while (*src != '\0') {
        if (*src == ' ' || *src == '\t') {
            if (!in_whitespace) {
                if (char_count >= len) {
                    return -1;
                }
                *dst++ = ' ';
                char_count++;
                in_whitespace = 1;
            }
        } else {
            if (char_count >= len) {
                return -1;
            }
            *dst++ = *src;
            char_count++;
            in_whitespace = 0;
        }
        src++;
    }

    while (char_count < len) {
        *dst++ = '.';
        char_count++;
    }

    return char_count;
}

/**
 * @brief Prints the contents of the buffer.
 * 
 * @param buff Pointer to the buffer
 * @param len Length of the buffer
 */
void print_buff(char *buff, int len) {
    printf("Buffer:  ");
    for (int i = 0; i < len; i++) {
        putchar(*(buff + i));
    }
    putchar('\n');
}

/**
 * @brief Displays usage instructions for the program.
 * 
 * @param exename Name of the executable
 */
void usage(char *exename) {
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

/**
 * @brief Counts the number of words in the buffer.
 * 
 * @param buff Pointer to the buffer
 * @param len Maximum length of the buffer
 * @param str_len Actual length of the user string in the buffer
 * @return int The number of words found in the buffer
 */
int count_words(char *buff, int len, int str_len) {
    int count = 0;
    int in_word = 0;
    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ') {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    return count;
}

/**
 * @brief Reverses the string stored in the buffer.
 * 
 * @param buff Pointer to the buffer containing the string
 * @param str_len Length of the user-provided string in the buffer
 */
void reverse_string(char *buff, int str_len) {
    char *start = buff;
    char *end = buff + str_len - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    printf("Reversed String: %.*s\n", str_len, buff);
}

/**
 * @brief Prints each word in the buffer on a new line along with its position and length.
 * 
 * @param buff Pointer to the buffer
 * @param str_len Length of the user string in the buffer
 */
void print_Words(char *buff, int str_len) {
    printf("Word Print\n----------\n");
    int word_num = 1;
    char *start = buff;
    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) == ' ' || i == str_len - 1) {
            if (i == str_len - 1 && *(buff + i) != ' ') {
                i++;
            }
            printf("%d. %.*s (%d)\n", word_num++, (int)(buff + i - start), start, (int)(buff + i - start));
            start = buff + i + 1;
        }
    }
}

int main(int argc, char *argv[]) {
    char *buff;
    char *input_string;
    char opt;
    int rc;
    int user_str_len;

    if ((argc < 2) || (*argv[1] != '-')) {
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1] + 1);

    if (opt == 'h') {
        usage(argv[0]);
        exit(0);
    }

    if (argc < 3) {
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2];

    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (!buff) {
        printf("Error: Memory allocation failed.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);
    if (user_str_len < 0) {
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt) {
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);
            if (rc < 0) {
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        case 'x':
            if (argc < 5) {
                printf("Error: Missing arguments for -x option.\n");
                exit(3);
            }
            printf("Not Implemented!\n");
            exit(3);
            break;
        case 'r':
            reverse_string(buff, user_str_len);
            break;
        case 'w':
            print_Words(buff, user_str_len);
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    print_buff(buff, BUFFER_SZ);
    free(buff);
    exit(0);
}
