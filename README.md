dmc (WIP)
========
Command-line user interface

Run dmc, it's not so tricky.

The name is an acronym for: dynamic manual commands.
Just kidding! It's a misspelling of cmd.

Goal
----
Support a bare minimum subset of sh interactive mode.
Redirection, pipes, etc should be separate programs where possible.
Builtins may end up with nothing but `cd`.

It is only meant to launch programs.
Scripting can be done in proper scripting languages, not the shell.
