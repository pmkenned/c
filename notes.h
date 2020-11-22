#if NOTES

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
#endif
