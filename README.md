dmc
========
Command-line user interface

Run dmc, it's not so tricky.

The name is an acronym for dynamic manual commands.
Just kidding! It's a misspelling of cmd.

Goal
--------
Support a bare minimum subset of sh interactive mode.
Redirection, builtins, etc should be separate programs where possible.

dmc is only meant to launch programs.
I will do my best to separate each functionality out of this program.

    ∃ scriptlang | scriptlang ∪ shell = ∅

Syntax
--------
```
%token WORD
%token NUMBER

%start sequence
%%
sequence    : command
            | command '&'
            | command pipe sequence
            ;
pipe        : '|'
            | '|' redirection
            ;
redirection : '[' NUMBER ']'
            | '[' NUMBER '=' NUMBER ']'
            ;
command     : WORD
            | WORD suffix
            ;
suffix      : WORD
            | WORD suffix
            ; /* TODO rule is superfluous? */
```

To be considered
----------------
Looking for stuff to throw in the bin:

* Redirection
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
http://doc.cat-v.org/plan_9/4th_edition/papers/rc
