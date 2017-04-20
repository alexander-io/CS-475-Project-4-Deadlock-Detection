CS 475 Project 3 : Time Sharing and Synchronization
David Olson & Alex Harris & Leslie Joe

Implemented the hardware interrupt clock/timer.

Solved dining philosophers using spin locks where a philosopher can eat if and only if they have both forks available to them.

Using test_and_set in main to atomically acquire a lock if it's free, or if it is not free to tell the thread to release its previous held lock. 

Usage :
  Navigate to the compile/ directory and issue :
    $ make clean
    $ make
    $ ./upload.sh
    $ sudo minicom

  Navigate to the virtual-box window and fire up the back-end. In the minicom console window the dining philosopher's sleeping/eating schedule will be printed.
