1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**: fgets() is a good choice because it reads input line by line, up to a specified maximum length, it includes the newline character in the buffer if it is encountered before the maximum length is reached. This is crucial for a shell, as commands are typically entered oneline at a time. It also prevents buffer overflows by limiting the amount of data read.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  In my solution, cmd_buff is actually a fixed-size array declared as char cmd_buff[SH_CMD_MAX];. Dynamic memory allocation with malloc() would be necessary only if the size of the command buffer needed to be determined at runtime or if it could potentially be very large.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing spaces is crucial to ensure correct command parsing and execution. If these spaces were not trimmed, the shell could misinterpret commands and arguments. For example, " ls -l" might be treated differently from "ls -l". The extra spaces could be incorrectly considered part of the command or arguments, leading to errors such as unrecognized commands or unexpected behavior.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Here are some redirection examples to consider:
                    command > output.txt: Redirects the standard output (STDOUT) of command to the file output.txt. If the file exists, it will be overwritten. Implementing this involves handling file creation, write permissions, and the decision of whether to overwrite or append to the file.
                    command >> output.txt: Redirects STDOUT to output.txt, appending the output to the file if it already exists. The challenge lies in managing the append operation correctly.
                    command < input.txt: Redirects the standard input (STDIN) of command from the file input.txt. This requires handling file opening and read permissions.
                    command 2> error.txt: Redirects the standard error (STDERR) of command to the file error.txt. The challenge is to correctly separate STDERR from STDOUT.
                    command 2>&1: Redirects STDERR to the same location as STDOUT. This involves merging the two output streams.



- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection focuses on altering the source or destination of input/output for a single command, typically involving files. In contrast, piping connects the STDOUT of one command to the STDIN of another, creating a chain where the output of one becomes the input of the next. This involves inter-process communication.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Separating STDERR and STDOUT allows users to distinguish between normal program output and error messages. This is crucial for debugging, logging, and script processing. It enables users to filter or handle error messages independently from regular output, such as redirecting only errors to a log file while displaying regular output on the terminal.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  The custom shell should provide flexibility in handling STDOUT and STDERR:

                    Separate Streams: Allow users to redirect STDOUT and STDERR to different files or devices. This is essential for debugging and logging.
                    Merged Stream: Offer the option to merge STDOUT and STDERR into a single stream, useful for viewing all output together or piping it to another command. This can be achieved through redirection like 2>&1.
                    Error Codes: Capture and report the exit status (error code) of each command to inform the user about success or failure.
                    Default Behavior: A reasonable default is to display both STDOUT and STDERR on the terminal but allow users to redirect or merge them as needed.
                    Merging Implementation: Merging can be done by redirecting STDERR to the same file descriptor as STDOUT, effectively treating them as a single stream.