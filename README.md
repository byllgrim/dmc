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
I will do my best to separate each functionality out of this program.

To be considered
----------------
Looking for stuff to throw in the bin:

* Redirection
* Pipes
* Line editing
* History
* Job control
* Aliases
* Quoting
* Builtins
* Word expansion
  - Globbing
  - Regex
  - Tilde expansion
  - Parameter expansion
  - Command substitution
  - Field splitting
* Environment
  - Variables
  - umask
  - trap
* Exit status
* Lists
* Operators
  - `&&`
  - `||`
* Scripting
  - for loop
  - case conditional
  - if conditional

http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
