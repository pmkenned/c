#define _XOPEN_SOURCE   700

#include <assert.h>         /*          Conditionally compiled macro that compares its argument to zero */
#include <ctype.h>          /*          Functions to determine the type contained in character data */
#include <errno.h>          /*          Macros reporting error conditions */
#include <float.h>          /*          Limits of float types */
#include <limits.h>         /*          Sizes of basic types */
#include <locale.h>         /*          Localization utilities */
#include <math.h>           /*          Common mathematics functions */
#include <setjmp.h>         /*          Nonlocal jumps */
#include <signal.h>         /*          Signal handling */
#include <stdarg.h>         /*          Variable arguments */
#include <stddef.h>         /*          Common macro definitions */
#include <stdio.h>          /*          Input/output */
#include <stdlib.h>         /*          misc: mem management, utilities, str conversions, rand numbers, algo */
#include <string.h>         /*          String handling */
#include <time.h>           /*          Time/date utilities */

/* C95 */
#include <iso646.h>         /* (C95)    Alternative operator spellings */
#include <wchar.h>          /* (C95)    Extended multibyte and wide character utilities */
#include <wctype.h>         /* (C95)    Functions to determine the type contained in wide character data */

/* C99 */
#include <complex.h>        /* (C99)    Complex number arithmetic */
#include <fenv.h>           /* (C99)    Floating-point environment */
#include <inttypes.h>       /* (C99)    Format conversion of integer types */
#include <stdbool.h>        /* (C99)    Macros for boolean type */
#include <stdint.h>         /* (C99)    Fixed-width integer types */
#include <tgmath.h>         /* (C99)    Type-generic math (macros wrapping math.h and complex.h) */

/* C11 */
#include <stdalign.h>       /* (C11)    alignas and alignof convenience macros */
#include <stdatomic.h>      /* (C11)    Atomic types */
#include <stdnoreturn.h>    /* (C11)    noreturn convenience macros */
#include <threads.h>        /* (C11)    Thread library */
#include <uchar.h>          /* (C11)    UTF-16 and UTF-32 character utilities */

/* POSIX specific */
#include <aio.h>            /* Asynchronous input and output */
#include <arpa/inet.h>      /* Functions for manipulating numeric IP addresses (part of Berkeley sockets) */
#include <cpio.h>           /* Magic numbers for the cpio archive format */
#include <dirent.h>         /* Allows the opening and listing of directories */
#include <dlfcn.h>          /* Dynamic linking */
#include <fcntl.h>          /* File opening, locking and other operations */
#include <fmtmsg.h>         /* Message display structures */
#include <fnmatch.h>        /* Filename matching */
#include <ftw.h>            /* File tree traversal */
#include <glob.h>           /* Pathname "globbing" (pattern-matching) */
#include <grp.h>            /* User group information and control */
#include <iconv.h>          /* Codeset conversion facility */
#include <langinfo.h>       /* Language information constants – builds on C localization functions */
#include <libgen.h>         /* Pathname manipulation */
#include <monetary.h>       /* String formatting of monetary units */
#include <mqueue.h>         /* Message queue */
#if 0
#include <ndbm.h>           /* NDBM database operations */
#endif
#include <net/if.h>         /* Listing of local network interfaces */
#include <netdb.h>          /* Translating protocol and host names into numeric addresses (part of Berkeley sockets) */
#include <netinet/in.h>     /* Defines Internet protocol and address family (part of Berkeley sockets) */
#include <netinet/tcp.h>    /* Additional TCP control options (part of Berkeley sockets) */
#include <nl_types.h>       /* Localization message catalog functions */
#include <poll.h>           /* Asynchronous file descriptor multiplexing */
#include <pthread.h>        /* Defines an API for creating and manipulating POSIX threads */
#include <pwd.h>            /* passwd (user information) access and control */
#include <regex.h>          /* Regular expression matching */
#include <sched.h>          /* Execution scheduling */
#include <search.h>         /* Search tables */
#include <semaphore.h>      /* POSIX semaphores */
#include <spawn.h>          /* Process spawning */
#include <strings.h>        /* Case-insensitive string comparisons */
#if 0
#include <stropts.h>        /* Stream manipulation, including ioctl */
#endif
#include <sys/ipc.h>        /* Inter-process communication (IPC) */
#include <sys/mman.h>       /* Memory management, including POSIX shared memory and memory mapped files */
#include <sys/msg.h>        /* POSIX message queues */
#include <sys/resource.h>   /* Resource usage, priorities, and limiting */
#include <sys/select.h>     /* Synchronous I/O multiplexing */
#include <sys/sem.h>        /* XSI (SysV style) semaphores */
#include <sys/shm.h>        /* XSI (SysV style) shared memory */
#include <sys/socket.h>     /* Main Berkeley sockets header */
#include <sys/stat.h>       /* File information (stat et al.) */
#include <sys/statvfs.h>    /* File System information */
#include <sys/time.h>       /* Time and date functions and structures */
#include <sys/times.h>      /* File access and modification times */
#include <sys/types.h>      /* Various data types used elsewhere */
#include <sys/uio.h>        /* Vectored I/O operations */
#include <sys/un.h>         /* Unix domain sockets */
#include <sys/utsname.h>    /* Operating system information, including uname */
#include <sys/wait.h>       /* Status of terminated child processes (see wait) */
#include <syslog.h>         /* System error logging */
#include <tar.h>            /* Magic numbers for the tar archive format */
#include <termios.h>        /* Allows terminal I/O interfaces */
#if 0
#include <trace.h>          /* Tracing of runtime behavior (DEPRECATED) */
#endif
#include <ulimit.h>         /* Resource limiting (DEPRECATED in favor of <sys/resource.h>) */
#include <unistd.h>         /* Various essential POSIX functions and constants */
#include <utime.h>          /* inode access and modification times */
#include <utmpx.h>          /* User accounting database functions */
#include <wordexp.h>        /* Word-expansion like the shell would perform */

void assert_function()
{
    assert(1);
}

void ctype_function()
{
    assert(isalnum('A'));
    assert(isalpha('A'));
    assert(iscntrl(0x10));
    assert(isdigit('0'));
    assert(isgraph('&'));
    assert(islower('a'));
    assert(isprint(' '));
    assert(ispunct('?'));
    assert(isspace('\t'));
    assert(isupper('A'));
    assert(isxdigit('f'));
    assert(toascii('a' + 0x80) == 'a');
    assert(toupper('a') == 'A');
    assert(tolower('A') == 'a');
}

void errno_function()
{
    // TODO
}

void float_function()
{
#if defined(DECIMAL_DIG     ) && \
    defined(DBL_DIG         ) && \
    defined(DBL_EPSILON     ) && \
    defined(DBL_MANT_DIG    ) && \
    defined(DBL_MAX         ) && \
    defined(DBL_MAX_10_EXP  ) && \
    defined(DBL_MAX_EXP     ) && \
    defined(DBL_MIN         ) && \
    defined(DBL_MIN_10_EXP  ) && \
    defined(DBL_MIN_EXP     ) && \
    defined(DECIMAL_DIG     ) && \
    defined(FLT_DIG         ) && \
    defined(FLT_EPSILON     ) && \
    defined(FLT_EVAL_METHOD ) && \
    defined(FLT_MANT_DIG    ) && \
    defined(FLT_MAX         ) && \
    defined(FLT_MAX_10_EXP  ) && \
    defined(FLT_MAX_EXP     ) && \
    defined(FLT_MIN         ) && \
    defined(FLT_MIN_10_EXP  ) && \
    defined(FLT_MIN_EXP     ) && \
    defined(FLT_RADIX       ) && \
    defined(FLT_ROUNDS      ) && \
    defined(LDBL_DIG        ) && \
    defined(LDBL_EPSILON    ) && \
    defined(LDBL_MANT_DIG   ) && \
    defined(LDBL_MAX        ) && \
    defined(LDBL_MAX_10_EXP ) && \
    defined(LDBL_MAX_EXP    ) && \
    defined(LDBL_MIN        ) && \
    defined(LDBL_MIN_10_EXP ) && \
    defined(LDBL_MIN_EXP    )
#else
    assert(0);
#endif
}

void limits_function()
{
    // TODO
}

void locale_function()
{
    locale_t l1 = newlocale(LC_TIME_MASK, "POSIX", (locale_t) 0);

    locale_t l2 = duplocale(l1);
    freelocale(l2);

    locale_t prev = uselocale(l1);

    struct lconv * lc = localeconv();

    char * currency_symbol      = lc->currency_symbol;
    char * decimal_point        = lc->decimal_point;
    char   frac_digits          = lc->frac_digits;
    char * grouping             = lc->grouping;
    char * int_curr_symbol      = lc->int_curr_symbol;
    char   int_frac_digits      = lc->int_frac_digits;
    char   int_n_cs_precedes    = lc->int_n_cs_precedes;
    char   int_n_sep_by_space   = lc->int_n_sep_by_space;
    char   int_n_sign_posn      = lc->int_n_sign_posn;
    char   int_p_cs_precedes    = lc->int_p_cs_precedes;
    char   int_p_sep_by_space   = lc->int_p_sep_by_space;
    char   int_p_sign_posn      = lc->int_p_sign_posn;
    char * mon_decimal_point    = lc->mon_decimal_point;
    char * mon_grouping         = lc->mon_grouping;
    char * mon_thousands_sep    = lc->mon_thousands_sep;
    char * negative_sign        = lc->negative_sign;
    char   n_cs_precedes        = lc->n_cs_precedes;
    char   n_sep_by_space       = lc->n_sep_by_space;
    char   n_sign_posn          = lc->n_sign_posn;
    char * positive_sign        = lc->positive_sign;
    char   p_cs_precedes        = lc->p_cs_precedes;
    char   p_sep_by_space       = lc->p_sep_by_space;
    char   p_sign_posn          = lc->p_sign_posn;
    char * thousands_sep        = lc->thousands_sep;

    char * s = setlocale(LC_TIME, NULL); 
}

void math_function()
{
    float_t f;
    double_t d;

    int rv;

    rv = fpclassify(f);
    rv = isfinite(f);
    rv = isgreater(f, d);
    rv = isgreaterequal(f, d);
    rv = isinf(f);
    rv = isless(f, d);
    rv = islessequal(f, d);
    rv = islessgreater(f, d);
    rv = isnan(f);
    rv = isnormal(f);
    rv = isunordered(f, d);
    rv = signbit(f);

#if defined(M_E         ) && \
    defined(M_LOG2E     ) && \
    defined(M_LOG10E    ) && \
    defined(M_LN2       ) && \
    defined(M_LN10      ) && \
    defined(M_PI        ) && \
    defined(M_PI_2      ) && \
    defined(M_PI_4      ) && \
    defined(M_1_PI      ) && \
    defined(M_2_PI      ) && \
    defined(M_2_SQRTPI  ) && \
    defined(M_SQRT2     ) && \
    defined(M_SQRT1_2   )
#else
    assert(0);
#endif

    assert(MAXFLOAT == FLT_MAX);

#if defined(HUGE_VAL    ) && \
    defined(HUGE_VALF   ) && \
    defined(HUGE_VALL   ) && \
    defined(INFINITY    ) && \
    defined(NAN         )
#else
    assert(0);
#endif

// TODO
#if 0
FP_INFINITE
FP_NAN
FP_NORMAL
FP_SUBNORMAL
FP_ZERO
FP_FAST_FMA
FP_FAST_FMAF
FP_FAST_FMAL
FP_ILOGB0
FP_ILOGBNAN
MATH_ERRNO
MATH_ERREXCEPT
math_errhandling
#endif

#if 0
    double      acos(double);
    float       acosf(float);
    double      acosh(double);
    float       acoshf(float);
    long double acoshl(long double);
    long double acosl(long double);
    double      asin(double);
    float       asinf(float);
    double      asinh(double);
    float       asinhf(float);
    long double asinhl(long double);
    long double asinl(long double);
    double      atan(double);
    double      atan2(double, double);
    float       atan2f(float, float);
    long double atan2l(long double, long double);
    float       atanf(float);
    double      atanh(double);
    float       atanhf(float);
    long double atanhl(long double);
    long double atanl(long double);
    double      cbrt(double);
    float       cbrtf(float);
    long double cbrtl(long double);
    double      ceil(double);
    float       ceilf(float);
    long double ceill(long double);
    double      copysign(double, double);
    float       copysignf(float, float);
    long double copysignl(long double, long double);
    double      cos(double);
    float       cosf(float);
    double      cosh(double);
    float       coshf(float);
    long double coshl(long double);
    long double cosl(long double);
    double      erf(double);
    double      erfc(double);
    float       erfcf(float);
    long double erfcl(long double);
    float       erff(float);
    long double erfl(long double);
    double      exp(double);
    double      exp2(double);
    float       exp2f(float);
    long double exp2l(long double);
    float       expf(float);
    long double expl(long double);
    double      expm1(double);
    float       expm1f(float);
    long double expm1l(long double);
    double      fabs(double);
    float       fabsf(float);
    long double fabsl(long double);
    double      fdim(double, double);
    float       fdimf(float, float);
    long double fdiml(long double, long double);
    double      floor(double);
    float       floorf(float);
    long double floorl(long double);
    double      fma(double, double, double);
    float       fmaf(float, float, float);
    long double fmal(long double, long double, long double);
    double      fmax(double, double);
    float       fmaxf(float, float);
    long double fmaxl(long double, long double);
    double      fmin(double, double);
    float       fminf(float, float);
    long double fminl(long double, long double);
    double      fmod(double, double);
    float       fmodf(float, float);
    long double fmodl(long double, long double);
    double      frexp(double, int *);
    float       frexpf(float, int *);
    long double frexpl(long double, int *);
    double      hypot(double, double);
    float       hypotf(float, float);
    long double hypotl(long double, long double);
    int         ilogb(double);
    int         ilogbf(float);
    int         ilogbl(long double);
    double      j0(double);
    double      j1(double);
    double      jn(int, double);
    double      ldexp(double, int);
    float       ldexpf(float, int);
    long double ldexpl(long double, int);
    double      lgamma(double);
    float       lgammaf(float);
    long double lgammal(long double);
    long long   llrint(double);
    long long   llrintf(float);
    long long   llrintl(long double);
    long long   llround(double);
    long long   llroundf(float);
    long long   llroundl(long double);
    double      log(double);
    double      log10(double);
    float       log10f(float);
    long double log10l(long double);
    double      log1p(double);
    float       log1pf(float);
    long double log1pl(long double);
    double      log2(double);
    float       log2f(float);
    long double log2l(long double);
    double      logb(double);
    float       logbf(float);
    long double logbl(long double);
    float       logf(float);
    long double logl(long double);
    long        lrint(double);
    long        lrintf(float);
    long        lrintl(long double);
    long        lround(double);
    long        lroundf(float);
    long        lroundl(long double);
    double      modf(double, double *);
    float       modff(float, float *);
    long double modfl(long double, long double *);
    double      nan(const char *);
    float       nanf(const char *);
    long double nanl(const char *);
    double      nearbyint(double);
    float       nearbyintf(float);
    long double nearbyintl(long double);
    double      nextafter(double, double);
    float       nextafterf(float, float);
    long double nextafterl(long double, long double);
    double      nexttoward(double, long double);
    float       nexttowardf(float, long double);
    long double nexttowardl(long double, long double);
    double      pow(double, double);
    float       powf(float, float);
    long double powl(long double, long double);
    double      remainder(double, double);
    float       remainderf(float, float);
    long double remainderl(long double, long double);
    double      remquo(double, double, int *);
    float       remquof(float, float, int *);
    long double remquol(long double, long double, int *);
    double      rint(double);
    float       rintf(float);
    long double rintl(long double);
    double      round(double);
    float       roundf(float);
    long double roundl(long double);
    double      scalbln(double, long);
    float       scalblnf(float, long);
    long double scalblnl(long double, long);
    double      scalbn(double, int);
    float       scalbnf(float, int);
    long double scalbnl(long double, int);
    double      sin(double);
    float       sinf(float);
    double      sinh(double);
    float       sinhf(float);
    long double sinhl(long double);
    long double sinl(long double);
    double      sqrt(double);
    float       sqrtf(float);
    long double sqrtl(long double);
    double      tan(double);
    float       tanf(float);
    double      tanh(double);
    float       tanhf(float);
    long double tanhl(long double);
    long double tanl(long double);
    double      tgamma(double);
    float       tgammaf(float);
    long double tgammal(long double);
    double      trunc(double);
    float       truncf(float);
    long double truncl(long double);
    double      y0(double);
    double      y1(double);
    double      yn(int, double);
#endif
}

void setjmp_function()
{
#if 0
    void   longjmp(jmp_buf, int);
    int    setjmp(jmp_buf);
    void   siglongjmp(sigjmp_buf, int);
    int    sigsetjmp(sigjmp_buf, int);

    void   _longjmp(jmp_buf, int);
    int    _setjmp(jmp_buf);
#endif
}

void signal_function()
{
}

void stdarg_function()
{
}

void stddef_function()
{
}

void stdio_function()
{
#if 0
    /* file operations */
    fopen();
    freopen();
    fflush();
    fclose();
    remove();
    rename();
    tmpfile();
    tmpnam();
    setvbuf();
    setbuf();
    /* formatted output */
    fprintf();
    printf();
    sprintf();
    vprintf();
    vfprintf();
    vsprintf();
    /* formatted input */
    fscanf();
    scanf();
    sscanf();
    /* character input and output functions */
    fgetc();
    fgets();
    fputc();
    fputs();
    getc();
    getchar();
    gets();
    putc();
    putchar();
    puts();
    ungetc();
    /* direct input and output functions */
    fread();
    fwrite();
    /* file positioning functions */
    fseek();
    ftell();
    rewind();
    fgetpos();
    fsetpos();
    /* error functions */
    clearerr();
    feof();
    ferror();
    perror();
#endif
}

void stdlib_function()
{
#if 0
#endif
}

void string_function()
{
#if 0
    /* */
#endif
}

void time_function()
{
#if 0
#endif
}

/* C95 */

void iso646_function()
{
}

void wchar_function()
{
}

void wctype_function()
{
}

/* C99 */

void complex_function()
{
}

void fenv_function()
{
}

void inttypes_function()
{
}

void stdbool_function()
{
}

void stdint_function()
{
}

void tgmath_function()
{
}

/* C11 */

void stdalign_function()
{
}

void stdatomic_function()
{
}

void stdnoreturn_function()
{
}

void threads_function()
{
}

void uchar_function()
{
}

/* POSIX */

void aio_function() {}
void arpa_inet_function() {}
void cpio_function() {}
void dirent_function() {}
void dlfcn_function() {}
void fcntl_function() {}
void fmtmsg_function() {}
void fnmatch_function() {}
void ftw_function() {}
void glob_function() {}
void grp_function() {}
void iconv_function() {}
void langinfo_function() {}
void libgen_function() {}
void monetary_function() {}
void mqueue_function() {}
/*void ndbm_function() {}*/
void net_if_function() {}
void netdb_function() {}
void netinet_in_function() {}
void netinet_tcp_function() {}
void nl_types_function() {}
void poll_function() {}
void pthread_function() {}
void pwd_function() {}
void regex_function() {}
void sched_function() {}
void search_function() {}
void semaphore_function() {}
void spawn_function() {}
void strings_function() {}
/*void stropts_function() {}*/
void sys_ipc_function() {}
void sys_mman_function() {}
void sys_msg_function() {}
void sys_resource_function() {}
void sys_select_function() {}
void sys_sem_function() {}
void sys_shm_function() {}
void sys_socket_function() {}
void sys_stat_function() {}
void sys_statvfs_function() {}
void sys_time_function() {}
void sys_times_function() {}
void sys_types_function() {}
void sys_uio_function() {}
void sys_un_function() {}
void sys_utsname_function() {}
void sys_wait_function() {}
void syslog_function() {}
void tar_function() {}
void termios_function() {}
#if 0
void trace_function() {}
#endif
void ulimit_function() {}
void unistd_function() {}
void utime_function() {}
void utmpx_function() {}
void wordexp_function() {}

int main(int argc, char * argv[])
{
    (void)argc;
    (void)argv;
    assert_function();
    ctype_function();
    errno_function();
    float_function();
    limits_function();
    locale_function();
    math_function();
    setjmp_function();
    signal_function();
    stdarg_function();
    stddef_function();
    stdio_function();
    stdlib_function();
    string_function();
    time_function();
    /* C95 */
    iso646_function();
    wchar_function();
    wctype_function();
    /* C99 */
    complex_function();
    fenv_function();
    inttypes_function();
    stdbool_function();
    stdint_function();
    tgmath_function();
    /* C11 */
    stdalign_function();
    stdatomic_function();
    stdnoreturn_function();
    threads_function();
    uchar_function();
    /* POSIX */
    aio_function();
    arpa_inet_function();
    cpio_function();
    dirent_function();
    dlfcn_function();
    fcntl_function();
    fmtmsg_function();
    fnmatch_function();
    ftw_function();
    glob_function();
    grp_function();
    iconv_function();
    langinfo_function();
    libgen_function();
    monetary_function();
    mqueue_function();
#if 0
    /*ndbm_function();*/
#endif
    net_if_function();
    netdb_function();
    netinet_in_function();
    netinet_tcp_function();
    nl_types_function();
    poll_function();
    pthread_function();
    pwd_function();
    regex_function();
    sched_function();
    search_function();
    semaphore_function();
    spawn_function();
    strings_function();
#if 0
    /*stropts_function();*/
#endif
    sys_ipc_function();
    sys_mman_function();
    sys_msg_function();
    sys_resource_function();
    sys_select_function();
    sys_sem_function();
    sys_shm_function();
    sys_socket_function();
    sys_stat_function();
    sys_statvfs_function();
    sys_time_function();
    sys_times_function();
    sys_types_function();
    sys_uio_function();
    sys_un_function();
    sys_utsname_function();
    sys_wait_function();
    syslog_function();
    tar_function();
    termios_function();
#if 0
    trace_function();
#endif
    ulimit_function();
    unistd_function();
    utime_function();
    utmpx_function();
    wordexp_function();

    return 0;
}
