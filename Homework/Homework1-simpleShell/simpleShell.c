#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

/*
    Function Declarations for shell commands
*/
int shellCd(char **args);
int shellExit(char **args);
int myLs(char **args);
int myCat(char **args);

/*
    Command list that execute in parent process
    and thier corresponding functions
*/
char *builtin[] = {
    "cd",
    "exit"};

int (*builtinFunction[])(char **) = {
    &shellCd,
    &shellExit};

size_t builtinLength()
{
    return sizeof(builtin) / sizeof(char *);
}

/*
    Command list that execute in child process
    and thier corresponding functions
*/
char *executable[] = {
    "ls",
    "cat"};

int (*executableFunction[])(char **) = {
    &myLs,
    &myCat};

size_t executableLength()
{
    return sizeof(executable) / sizeof(char *);
}

/*
    Command function implementations
*/

/**
 @brief Change working directory.
 @param args Argument list where the 1st arg is "cd" and next arg is the directory.
 @return Always returns 1 to continue shell loop.
 */
int shellCd(char **args)
{
    if (args[1] == NULL)
        fprintf(stderr, "cd: expected an argument\n");
    else if (args[2] != NULL)
        fprintf(stderr, "cd: too many arguments\n");
    else
    {
        if (chdir(args[1]) != 0)
            perror("cd");
    }
    return 1;
}

/**
 @brief Terminates the shell.
 @param args Argument list (not used).
 @return Always returns 0 to stop shell loop.
 */
int shellExit(char **args)
{
    return 0;
}

/**
 @brief List files or directories.
 @param args Null terminated argument list where the 1st arg is "ls", next arg is the directory (optional, defaults to current working directory), next arg is ">" for redirection and followed by arg indicating output file (both optional).
 @return returns 1 on success or negative value on error.
 */
int myLs(char **args)
{
    struct dirent *pDirent;
    DIR *pDir = NULL;
    FILE *out = NULL;
    char *workingDir = NULL;
    size_t i = 1;

    // directory to read from
    if (args[i] == NULL || strcmp(args[i], ">") == 0)
        workingDir = "./";
    else
        workingDir = args[i++];

    // output stream
    if (args[i] && strcmp(args[i], ">") == 0)
    {
        if(args[++i] == NULL)
        {
            fprintf(stderr, "ls: expected output file after '>'\n");
            return -1;
        }
        out = fopen(args[i], "w");
        if (out == NULL)
        {
            fprintf(stderr, "ls: cannot open file for write: '%s'\n", args[i]);
            return -1;
        }
    }
    else if (args[i])
    {
        fprintf(stderr, "ls: incorrect arguments format. (ls 'folder'[optional] > 'output-file') -redirecting output is optional\n");
        return -2;
    }

    pDir = opendir(workingDir);
    if (pDir == NULL)
    {
        fprintf(stderr, "ls: cannot open directory '%s'\n", workingDir);
        return -3;
    }

    if (out == NULL)
        out = stdout;

    while ((pDirent = readdir(pDir)) != NULL)
        fprintf(out, "[%s]\n", pDirent->d_name);

    closedir(pDir);
    if (out != stdout)
        fclose(out);

    if (errno != 0) // stream ended on an error
    {
        perror("ls: readdir");
        return -4;
    }

    return 1;
}

/**
 @brief Print given file's content.
 @param args Null terminated argument list where the 1st arg is "cat" and 2nd arg is the file, next arg is ">" for redirection and followed by arg indicating output file (both optional).
 @return returns 1 on success or negative value on error.
 */
int myCat(char **args)
{
    FILE *in = NULL, *out = NULL;
    char c;

    if (args[1] == NULL)
    {
        fprintf(stderr, "cat: expected an argument to 'cat'\n");
        return -1;
    }

    in = fopen(args[1], "r");
    if (in == NULL)
    {
        fprintf(stderr, "cat: cannot open file for read: '%s'\n", args[1]);
        return -3;
    }

    if (args[2] && strcmp(args[2], ">") == 0)
    {
        if (args[3] == NULL)
        {
            fprintf(stderr, "cat: expected file name after '>'\n");
            fclose(in);
            return -4;
        }

        out = fopen(args[3], "w");
        if (out == NULL)
        {
            fprintf(stderr, "cat: cannot open file for write: '%s'\n", args[3]);
            fclose(in);
            return -5;
        }
    }

    if (out == NULL)
        out = stdout;

    while ((c = getc(in)) != EOF) // print input stream into output stream
    {
        putc(c, out);
    }

    fclose(in);
    if (out != stdout)
        fclose(out);
    else
        putchar('\n');
    return 1;
}

/**
   @brief Execute shell commands.
   @param args Null terminated argument list.
   @return 1 if shell should keep running, 0 if it should terminate.
 */
int executeCommand(char **args)
{
    pid_t pid;
    int status;

    // no commands given (empty args list)
    if (args[0] == NULL)
        return 1;

    // command to be executed in parent's process
    for (size_t i = 0; i < builtinLength(); ++i)
    {
        if (strcmp(args[0], builtin[i]) == 0)
            return (*builtinFunction[i])(args);
    }

    // command to be executed in child's process
    for (size_t i = 0; i < executableLength(); ++i)
    {
        if (strcmp(args[0], executable[i]) == 0)
        {
            if ((pid = fork()) < 0)
                perror("fork");
            else if (pid == 0)
            {
                pid = getpid();
                printf("Child process PID: %d\n", pid);
                if ((status = (*executableFunction[i])(args)) < 0)
                {
                    // perror(executable[i]);
                    exit(status);
                }
                return 0;
            }
            else
            {
                if ((pid = waitpid(pid, &status, 0) < 0))
                {
                    perror("waitpid");
                    return 0;
                }
                return 1;
            }
        }
    }

    printf("Unkown command '%s'\n", args[0]);
    return 1;
}

#define TOKEN_DELIM " \t\r\n"
#define ARGS_SIZE 32
/**
   @brief Split a line into tokens.
   @param line The line.
   @return Null terminated token list (dynamically located).
 */
char **splitLine(char *line)
{
    size_t size = ARGS_SIZE, i = 0;
    char **args = malloc(size * sizeof(char *));
    char *token, **tempArgs;

    if (!args)
    {
        fprintf(stderr, "splitLine: Allocation error\n");
        exit(1);
    }

    token = strtok(line, TOKEN_DELIM);
    while (token)
    {
        args[i++] = token;
        if (i >= size)
        {
            size += ARGS_SIZE;
            tempArgs = realloc(args, size * sizeof(char *));
            if (!tempArgs)
            {
                free(args);
                free(line);
                fprintf(stderr, "splitLine: Allocation error\n");
                exit(2);
            }
            args = tempArgs;
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    args[i] = NULL;
    return args;
}

#define LINE_MAX_LENGTH 1024
/**
   @brief Read an input line from stdin.
   @return The line (dynamically located).
 */
char *readLine(ssize_t *readBytes)
{
    // this block of code acts weird when stdin is a file where eof is not a new line
    // char *line = NULL;
    // size_t lineSize = 0;
    // if (getline(&line, &lineSize, stdin) == -1)
    // {
    //     if (feof(stdin))
    //         exit(0);
    //     else
    //     {
    //         perror("readLine");
    //         exit(1);
    //     }
    // }

    char *line, c;
    size_t size = LINE_MAX_LENGTH, i = 0;

    line = malloc(LINE_MAX_LENGTH);
    if (!line)
    {
        perror("readLine");
        exit(1);
    }

    while ((*readBytes = read(STDIN_FILENO, &c, 1)) > 0 && c != '\n')
    {
        line[i++] = c;
        if (i >= size)
        {
            char *tmp;
            size += LINE_MAX_LENGTH;
            tmp = malloc(size);
            if (!tmp)
            {
                free(line);
                perror("readLine");
                exit(1);
            }
            line = tmp;
        }
    }
    if (*readBytes < 0)
    {
        free(line);
        perror("readLine");
        exit(1);
    }
    line[i] = 0;
    return line;
}

/**
   @brief Shell loop, getting input and executing it.
 */
void loop()
{
    char *line;
    char **args;
    int status;
    ssize_t readBytes;

    do
    {
        printf("> ");
        fflush(stdout);
        line = readLine(&readBytes);
        args = splitLine(line);
        status = executeCommand(args);

        free(line);
        free(args);
    } while (status && readBytes);
}

/**
   @brief Main program Function.
   @param argc Argument count.
   @param argv Argument array.
   @return Returns 0.
 */
int main(int argc, char const *argv[])
{
    loop();
    return 0;
}