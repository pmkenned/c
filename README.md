# c
Practicing C

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
* termios

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

## Code Samples

```
typedef enum {
    MST_INT,
    MST_FLOAT,
    MST_COUNT
} MyStructType;

typedef struct {
    char  name[32];
    MyStructType type;
} MyHeader;

typedef struct {
    MyHeader head;
    int value;
} MyStructInt;

typedef struct {
    MyHeader head;
    float vlaue;
} MyStructFloat;

void my_set_value(MyHeader * h, double value)
{
    if (h->type == MST_INT)
        ((MyStructInt *)h)->value = (int) value;
    else if (h->type == MST_FLOAT)
        ((MyStructFloat *)h)->value = (float) value;
}
```

## References:

* [OS detecting makefile](https://stackoverflow.com/questions/714100/os-detecting-makefile)
* [Practical Makefiles, by example](http://nuclear.mutantstargoat.com/articles/make/#writing-install-uninstall-rules)
