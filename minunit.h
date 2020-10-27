#ifndef MINUNIT_H
#define MINUNIT_H

#define ERR_MSG_LEN 1024
extern char err_msg[ERR_MSG_LEN];

#define mu_assert(test, fmt, ...) \
    do { \
        if (!(test)) { \
            snprintf(err_msg, ERR_MSG_LEN, __FILE__ "#%d: " fmt, __LINE__, __VA_ARGS__); \
            return err_msg; \
        } \
    } while (0)

/* #define mu_assert(message, test) do { if (!(test)) return message; } while (0) */
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                               if (message) return message; } while (0)
#define mu_run_test_suite(test) do { char *message = test(); \
                               if (message) return message; } while (0)
extern int tests_run;

#endif
