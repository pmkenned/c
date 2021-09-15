#ifndef BOX_H
#define BOX_H

#define BOX_HOR     "\u2500"
#define BOX_VER     "\u2502"
#define BOX_T       "\u252c"
#define BOX_CCT     "\u251c"
#define BOX_CT      "\u2524"
#define BOX_UPT     "\u2534"
#define BOX_CROSS   "\u253c"
#define BOX_LLC     "\u2514"
#define BOX_LRC     "\u2518"
#define BOX_ULC     "\u250c"
#define BOX_URC     "\u2510"

#define BOX_DHOR    "\u2550"
#define BOX_DVER    "\u2551"
#define BOX_DT      "\u2566"
#define BOX_DCCT    "\u2560"
#define BOX_DCT     "\u2563"
#define BOX_DUPT    "\u2569"
#define BOX_DCROSS  "\u256c"
#define BOX_DLLC    "\u255a"
#define BOX_DLRC    "\u255d"
#define BOX_DULC    "\u2554"
#define BOX_DURC    "\u2557"

extern void printf_in_box(const char * fmt, ...);
extern void printf_in_dbox(const char * fmt, ...);

#endif /* BOX_H */
