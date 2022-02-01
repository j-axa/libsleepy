# libsleepy
cross platform sleep function that tries to not overshoot sleep duration

requests a slightly shorter sleep duration from the system than was passed to the sleep function and then spins in a loop the rest of the way

## windows
uses NtSetTimerResolution to lower timer resolution and WaitForSingleObject to sleep

## *nix
looks for nanosleep or usleep and uses whichever it finds

