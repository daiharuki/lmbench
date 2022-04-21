#ifndef _PERFMON_H
#define _PERFMON_H

#ifndef __ASSEMBLER__
#ifdef __GNUC__

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
    asm volatile ("csrw " #reg ", %0" :: "r"(val)); })

#define set_csr(reg, bit) ({ unsigned long __tmp; \
    asm volatile ("csrrs %0, " #reg ", %1" : "=r"(__tmp) : "r"(bit)); \
  __tmp; })

#define clear_csr(reg, bit) ({ unsigned long __tmp; \
    asm volatile ("csrrc %0, " #reg ", %1" : "=r"(__tmp) : "r"(bit)); \
  __tmp; })

#define rdtime() read_csr(time)
#define rdcycle() read_csr(cycle)
#define rdinstret() read_csr(instret)
#define rdpm3() read_csr(hpmcounter3)
#define rdpm4() read_csr(hpmcounter4)
#define rdpm5() read_csr(hpmcounter5)
#define rdpm6() read_csr(hpmcounter6)


#define CNT_CSR 7
#define CNT_PL2 0
#define CNT_CL3 0
#define CNT_ALL (CNT_CSR+CNT_PL2+CNT_CL3)
//#define CNT_ALL 24

#define OFFSET_EVENT_SEL 0x2000
#define OFFSET_EVENT_CNT 0x3000

void init_pmon(void);
void init_pmoncnt(void);
void dump_pmoncnt(char*);
void store_pmoncnt(int);

extern uint64_t pmoncnt[2][CNT_ALL]; // aligned to 64B.
extern uint64_t pl2_base_p;
extern uint64_t* pl2_base_v;
extern uint64_t* pl2_pmon_cnt_v;
extern uint64_t cl3_base_p;
extern uint64_t* cl3_base_v;
extern uint64_t* cl3_pmon_cnt_v;
extern uint64_t mapsize;

#endif
#endif
#endif
