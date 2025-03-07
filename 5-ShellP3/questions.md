1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?


Basically, my shell's gotta play the patient parent. After it sends out all those child processes to do their piped tasks, it just sits there and waits. It uses waitpid() to do that. It's like saying, "Okay, kids, finish up, and then we'll move on."
If I forgot waitpid(), it'd be a mess. Those child processes would just run off, and the shell wouldn't know when they were done. You'd get zombie processes hanging around, and the shell might start taking new commands before the old ones were finished. Total chaos.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?



dup2() is like rerouting pipes in your house. Once you've got the water flowing where you want it, you gotta close off the old pipes. Otherwise, you'll have leaks and clogs.
If you leave those pipe ends open, you get all sorts of weird stuff. Programs might get stuck waiting for input that's never coming, or they might not know when they're done reading. It's just bad plumbing, basically.
3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?


cd has to change my working directory, not some other process's. It's like, I can't tell my shadow to move my stuff; I gotta do it myself.
If cd were external, it'd be a nightmare. You'd have to figure out some crazy way for the child process to tell the parent, "Hey, I moved, now you move too!" Way too complicated.
4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?


Right now, it's like having a limited number of slots on a train. To make it unlimited, I'd have to use dynamic memory allocation. Basically, just grab more space when I need it.
But, yeah, memory stuff is tricky. You gotta be careful not to waste it, and you gotta clean up after yourself. Plus, even with that, there are still limits on how much memory you can use. So, it's more like "practically unlimited" than truly unlimited.