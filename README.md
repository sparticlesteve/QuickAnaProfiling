# QuickAnaProfiling
A package for profiling QuickAna with valgrind

## Checkout the package
with git:

    git clone git git@github.com:sparticlesteve/QuickAnaProfiling.git

with svn:

    svn checkout https://github.com/sparticlesteve/QuickAnaProfiling/trunk QuickAnaProfiling

## Preparation
_Before_ you compile, you need to setup valgrind and an environment variable
that specifies the path to the valgrind header files for includes. You might
be able to use the valgrind installation on CVMFS:

    localSetupSFT valgrind/3.8.0

This command will setup the environment completely. However, there may be some
compatibility issues with ROOT 6 requiring a newer version of valgrind. I have
tested usage of 3.10.0 on lxplus. An example setup script is at
[](scripts/setup_lxplus.sh).

## Running

At the moment there's just one executable, which you can run with

    quickana_profile -i [input-file] -n [num-events] ...

Some useful command line options:
* Turn on systematics with ```--sys```
* Use the optimized scheduler with ```--scheduler optimized```
Run with `-h` to see the full list of command line options.
I plan to add more functionality incrementally.
