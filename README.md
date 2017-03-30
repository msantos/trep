------------------------------------------------------------------------

TREP(1) BSD General Commands Manual TREP(1)

**NAME**

**trep** — Selectively stream stdin to stdout/stderr based on regular expressions

**SYNOPSIS**

**trep** \[**−abcEFGHhIiLlnoqRsUVvwx**\] \[**−A ***num*\] \[**−B ***num*\] \[**−C**\[*num*\]\] \[**−e ***pattern*\] \[**−f ***file*\] \[**−-binary-files**=*value*\] \[**−-context**\[=*num*\]\] \[**−-line-buffered**\] \[**−-stream-with-match**=\[*stdout *| *stderr *| *null*\]\] \[**−-stream-without-match**=\[*stdout *| *stderr *| *null*\]\] \[*pattern*\] \[*file ...*\]

**DESCRIPTION**

**trep** (tee-rep) combines tee(1) and grep(1) to select input using a regular expression and output to stdout or stderr based on the match. **trep** is a fork of OpenBSD grep and so supports all the usual grep(1) options.

The **trep** utility searches any given input files, selecting lines that match one or more patterns. By default, a pattern matches an input line if the regular expression (RE) in the pattern matches the input line without its trailing newline. An empty expression matches every line. Each input line that matches at least one of the patterns is written to the standard output. If no file arguments are specified, the standard input is used.

**trep** is used for simple patterns and basic regular expressions (BREs); **etrep** can handle extended regular expressions (EREs). See re\_format(7) for more information on regular expressions. **ftrep** is quicker than both **trep** and **etrep**, but can only handle fixed patterns (i.e. it does not interpret regular expressions). Patterns may consist of one or more lines, allowing any of the pattern lines to match a portion of the input.

The following options are available:

**−A** *num*

Print *num* lines of trailing context after each match. See also the **−B** and **−C** options.

**−a**

Treat all files as ASCII text. Normally **trep** will simply print ’’Binary file ... matches’’ if files contain binary characters. Use of this option forces **trep** to output lines matching the specified pattern.

**−B** *num*

Print *num* lines of leading context before each match. See also the **−A** and **−C** options.

**−b**

The offset in bytes of a matched pattern is displayed in front of the respective matched line.

**−C**\[*num*\]

Print *num* lines of leading and trailing context surrounding each match. The default is 2 and is equivalent to **−A** *2* **−B** *2*. Note: no whitespace may be given between the option and its argument.

**−c**

Only a count of selected lines is written to standard output.

**−E**

Interpret *pattern* as an extended regular expression (i.e. force **trep** to behave as **etrep**).

**−e** *pattern*

Specify a pattern used during the search of the input: an input line is selected if it matches any of the specified patterns. This option is most useful when multiple **−e** options are used to specify multiple patterns, or when a pattern begins with a dash (’-’).

**−F**

Interpret *pattern* as a set of fixed strings (i.e. force **trep** to behave as **ftrep**).

**−f** *file*

Read one or more newline separated patterns from *file*. Empty pattern lines match every input line. Newlines are not considered part of a pattern. If *file* is empty, nothing is matched.

**−G**

Interpret *pattern* as a basic regular expression (i.e. force **trep** to behave as traditional **trep**).

**−H**

Always print filename headers (i.e. filenames) with output lines.

**−h**

Never print filename headers (i.e. filenames) with output lines.

**−I**

Ignore binary files.

**−i**

Perform case insensitive matching. By default, **trep** is case sensitive.

**−L**

Only the names of files not containing selected lines are written to standard output. Pathnames are listed once per file searched. If the standard input is searched, the string ’’(standard input)’’ is written.

**−l**

Only the names of files containing selected lines are written to standard output. **trep** will only search a file until a match has been found, making searches potentially less expensive. Pathnames are listed once per file searched. If the standard input is searched, the string ’’(standard input)’’ is written.

**−n**

Each output line is preceded by its relative line number in the file, starting at line 1. The line number counter is reset for each file processed. This option is ignored if **−c**, **−L**, **−l**, or **−q** is specified.

**−o**

Print each match, but only the match, not the entire line.

**−q**

Quiet mode: suppress normal output. **trep** will only search a file until a match has been found, making searches potentially less expensive.

**−R**

Recursively search subdirectories listed.

**−s**

Silent mode. Nonexistent and unreadable files are ignored (i.e. their error messages are suppressed).

**−U**

Search binary files, but do not attempt to print them.

**−V**

Display version information. All other options are ignored.

**−v**

Selected lines are those *not* matching any of the specified patterns.

**−w**

The expression is searched for as a word (as if surrounded by ’\[\[:&lt;:\]\]’ and ’\[\[:&gt;:\]\]’; see re\_format(7)).

**−x**

Only input lines selected against an entire fixed string or regular expression are considered to be matching lines.

**−−binary-files**=*value*

Controls searching and printing of binary files. Options are *binary*, the default: search binary files but do not print them; *without-match*: do not search binary files; and *text*: treat all files as text.

**−−context**\[=*num*\]

Print *num* lines of leading and trailing context. The default is 2.

**−−line-buffered**

Force output to be line buffered. By default, output is line buffered when standard output is a terminal and block buffered otherwise.

**−−stream-with-match**=\[*stdout*|*stderr*|*null*\]

Send matching lines to a stream. The default stream is *stdout.*

**−−stream-without-match**=\[*stdout*|*stderr*|*null*\]

Send unmatched lines to a stream. The default stream is *null.*

**EXIT STATUS**

The **trep** utility exits with one of the following values:

0

One or more lines were selected.

1

No lines were selected.

&gt;1

An error occurred.

**EXAMPLES**

To monitor syslog and send all occurrences of the word ’CRITICAL’ to stderr and everything else to stdout:

$ tail -f syslog | trep --stream-with-match=stderr --stream-without-match=stdout ’CRITICAL’

To find all occurrences of the word ’patricia’ in a file:

$ trep ’patricia’ myfile

To find all occurrences of the pattern ’.Pp’ at the beginning of a line:

$ trep ’^\\.Pp’ myfile

The apostrophes ensure the entire expression is evaluated by **trep** instead of by the user’s shell. The caret ’^’ matches the null string at the beginning of a line, and the ’\\’ escapes the ’.’, which would otherwise match any character.

To find all lines in a file which do not contain the words ’foo’ or ’bar’:

$ trep -v -e ’foo’ -e ’bar’ myfile

A simple example of an extended regular expression:

$ etrep ’19|20|25’ calendar

Peruses the file ’calendar’ looking for either 19, 20, or 25.

**SEE ALSO**

ed(1), ex(1), gzip(1), sed(1), re\_format(7)

**STANDARDS**

The **trep** utility is compliant with the IEEE Std 1003.1-2008 (’’POSIX.1’’) specification.

The flags \[**−AaBbCGHhILoRUVwZ**\] are extensions to that specification, and the behaviour of the **−f** flag when used with an empty pattern file is left undefined.

All long options are provided for compatibility with GNU versions of this utility.

Historic versions of the **grep** utility also supported the flags \[**−ruy**\]. This implementation supports those options; however, their use is strongly discouraged.

**HISTORY**

The **grep** command first appeared in Version 4 AT&T UNIX.

BSD March 08, 2017 BSD

------------------------------------------------------------------------
