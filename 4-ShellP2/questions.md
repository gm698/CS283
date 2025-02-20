**Questions and Answers**

**1. Fork/Execvp vs. Execvp Directly:**

* **Question:** Can you think of why we use fork/execvp instead of just calling execvp directly? What value do you think the fork provides?
* **Answer:**
    * We use `fork/execvp` because `execvp` replaces the current process image with a new one. If we called `execvp` directly in our shell, the shell itself would be replaced by the executed command, and the shell would terminate.
    * `fork` creates a new child process that is a copy of the parent (the shell). `execvp` is then called within the child process, replacing the child's image with the command's image. This allows the parent shell process to continue running and accept more commands after the child process finishes. The `fork` provides process isolation, allowing the shell to stay alive.

**2. Fork() System Call Failure:**

* **Question:** What happens if the fork() system call fails? How does your implementation handle this scenario?
* **Answer:**
    * If `fork()` fails, it returns a negative value. This typically indicates that the system has insufficient resources to create a new process (e.g., insufficient memory or process limit reached).
    * My implementation handles this scenario by checking the return value of `fork()`. If it's negative, an error message ("fork: ...") is printed using `perror()`, and the `exec_cmd()` function returns an error code.

**3. Execvp() Command Location:**

* **Question:** How does execvp() find the command to execute? What system environment variable plays a role in this process?
* **Answer:**
    * `execvp()` searches for the executable file in the directories specified by the `PATH` environment variable.
    * The `PATH` variable is a colon-separated list of directories. `execvp()` iterates through these directories, attempting to execute the command from each directory until it finds a match.

**4. Wait() in Parent Process:**

* **Question:** What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?
* **Answer:**
    * The purpose of `wait()` is to make the parent process wait for the child process to terminate. This prevents the child process from becoming a "zombie" process (a process that has terminated but still has an entry in the process table).
    * If we didn't call `wait()`, the child process would become a zombie until its parent process terminates or calls `wait()`. If the parent process creates many child processes without waiting, the system's process table could become filled with zombie processes, potentially causing resource issues.

**5. WEXITSTATUS():**

* **Question:** In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?
* **Answer:**
    * `WEXITSTATUS()` extracts the exit status of the child process from the status information returned by `wait()`. The exit status is the value passed to `exit()` by the child process.
    * It's important because it allows the parent process to determine if the child process terminated successfully or encountered an error. This is used to set the shells `last_rc` variable.

**6. Build_cmd_buff() and Quoted Arguments:**

* **Question:** Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?
* **Answer:**
    * My implementation of `build_cmd_buff()` iterates through the command line, looking for quote characters (`"`). When a quote is encountered, a boolean flag `in_quotes` is toggled. While `in_quotes` is true, spaces are treated as part of the argument, not as argument separators. The quote characters themselves are removed from the argument.
    * This is necessary to allow arguments to contain spaces. For example, if a user enters `"hello world"`, the shell should treat it as a single argument, not two separate arguments.

**7. Parsing Logic Changes:**

* **Question:** What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?
* **Answer:**
    * The parsing logic was modified to handle quoted arguments. This involved adding a flag to track whether the parser was inside a quoted string and adjusting the argument separation logic accordingly.
    * One challenge was refactoring the code to handle the removal of the quote characters themselves from the parsed arguments.
    * Another challenge was ensuring the logic correctly handled edge cases, such as empty quoted strings or quotes within quotes.

**8. Linux Signals Purpose:**

* **Question:** What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?
* **Answer:**
    * Signals are a form of IPC used to notify a process of an event. They are asynchronous and can be sent from the kernel or other processes.
    * Signals differ from other forms of IPC (e.g., pipes, shared memory, message queues) in that they are primarily used for event notification and control, rather than data exchange. Signals are very simple, and are not designed to carry large amounts of data.

**9. Commonly Used Signals:**

* **Question:** Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?
* **Answer:**
    * **SIGINT (2):** Interrupt signal. Sent to a process when the user presses Ctrl+C. Typically used to terminate a foreground process.
    * **SIGTERM (15):** Termination signal. Sent to a process to request its graceful termination. Can be caught and handled by the process.
    * **SIGKILL (9):** Kill signal. Sent to a process to force its immediate termination. Cannot be caught or ignored.

**10. SIGSTOP Reception:**

* **Question:** What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?
* **Answer:**
    * When a process receives SIGSTOP, it is immediately paused.
    * SIGSTOP cannot be caught or ignored. This is because it's intended to provide a reliable way to stop a process, regardless of its state.