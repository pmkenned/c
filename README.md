# c
Practicing C

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

serialization
coredumps
SOA vs AOS
binary tree
opaque structures
const *
typedef structs, []
char [1]
struct composition
typedef type
error handling
profiling
`##` token concatenation
map of maps
malloc debug
sorted?

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

* sockets
* threads
* mmap
* signals
* GUI
* graphics
* sound, images
* termios

Things to practice/think about:

* Moluarity, Interface vs. implementation
* Performance, profiling
* Testing
* Debugging
* Error-handling
* C++ linkage
* `static_assert`
