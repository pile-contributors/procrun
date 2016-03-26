ProcRun
=======

Support for running and organizing processes.

Running a program is mediated by the ProcRun class that
builds on Qt's QProcess. It allows the user to specify
the program and the arguments, the working directory and the
input that should be written to standard input of the process.
The class accumulates information about
the run including the time it took, standard output and
error (both independently and combined) and
the list of states and errors the process experienced.

The process may be runned synchronously using an internal event loop
or it may be run asynchronously with the usual QProcess means.
ProcRun may also inform the caller about the completion of the
run using a callback along with user data and
using procRunFinish() Qt signal.

The pile also provides a model - ProcRunModel - for
presenting a number of process invocations in a tree fashion that
may be saved and reloaded using QSettings. The model uses
ProcRunItemBase for its items (groups are ProcRunGroup and
actual items are ProcRunItem).

ProcRunGui pile provides a graphical user interface that
makes use of the classes of this pile.
