# c
Practicing C

## Projects

* bug database
* pkgg: game
* ped: text editor
* duphunt: find and delete duplicate files
* hltdb: half-life texture database

## TODO:

* modularity
* portability
* documentation
* logging
* timing, profiling & instrumenting
* testing
* debugging, coredump
* performance
* malloc debug
* static linking
* dynamic linking (shared libs, plugins, interactive programming)
* coding standards
* serialization
* regex
* IPC, mm files, named pipes, mmap
* error handling
* multithreading, child processes
* network
* graphics
* gui
* sound, images
* signals
* data structures
    * AOS vs SOA
    * doubly linked list
    * dynamic array
    * strings
    * hashmap
    * graph
    * tree

## misc:

* binary tree
* opaque structures
* const *
* typedef structs, []
* char [1] trick
* struct composition
* typedef type
* `##` token concatenation
* map of maps
* sorted?
* `static_assert`
* termios, ncurses

## Tools to learn:

* splint
* valgrind
* make
* git
* makeprg (vim)

## File Structure

```
src/
  main.c
  pkcl.h
  pkcl_dyn_arr.c
  pkcl_str.c
  pkcl_map.c
  pkcl_tree.c
  pkcl_graph.c
test/
  data/
    hello.txt
  test.c
doc/
examples/
  example1.c
Makefile
README.md
```

## References:

* [OS detecting makefile](https://stackoverflow.com/questions/714100/os-detecting-makefile)
* [Practical Makefiles, by example](http://nuclear.mutantstargoat.com/articles/make/#writing-install-uninstall-rules)
