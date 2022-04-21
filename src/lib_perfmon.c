
#include "bench.h"
#include "timing.h"

#include "perfmon.h"

/* Only work with -P 1 for now
 2  for beginning and end
 24 for ceiling 8 of (3+4+6+8)
 3 for mcycle, minstret, mtime
 4 for mhpmcounter3,4,5,6
 6 for pL2 counters
 8 for cL3 counters
 need csr read,write,set
*/

#ifdef _DEBUG
#define dbgprintf(...) printf(__VA_ARGS__)
#else
#define dbgprintf(...) 
#endif

void init_pmoncnt(void)
{
  long i,j;
  for (i=0; i<2; i++) {
    for (j=0; j<CNT_ALL; j++) {
      pmoncnt[i][j] = 0;
    }
  }
}

void init_pmon(void)
{
  int fd;
  uint64_t pagesize = (uint64_t)getpagesize(); /* or sysconf(_SC_PAGESIZE)  */
  uint64_t offset,base;
  uint64_t mask;

  dbgprintf("%s-%d\n",__FUNCTION__,__LINE__);

  if (CNT_PL2+CNT_CL3) {
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
      fprintf(stderr,"Error opening /dev/mem\n");
      exit(1);
    }
  /* pl2_base = 0x10104000
     cl3_base = 0x02010000
   */
    mapsize = pagesize;
    mask = ~(pagesize-1);
    if (CNT_PL2) {
      base   = (pl2_base_p + OFFSET_EVENT_CNT) & mask;
      offset = (pl2_base_p + OFFSET_EVENT_CNT) & ~mask;
      pl2_base_v = mmap(0, pagesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, base );
      if (pl2_base_v == (void*)-1) {
	fprintf(stderr,"map error (pl2)\n");
      }
      pl2_pmon_cnt_v = pl2_base_v + offset;
    }
    if (CNT_CL3) {
      base   = (cl3_base_p + OFFSET_EVENT_CNT) & mask;
      offset = (cl3_base_p + OFFSET_EVENT_CNT) & ~mask;
      cl3_base_v = mmap(0, pagesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, base );
      if (cl3_base_v == (void*)-1) {
	fprintf(stderr,"map error (cl3)\n");
      }
      cl3_pmon_cnt_v = cl3_base_v + offset;
    }
    dbgprintf("pl2_pmon_cnt_v(%p),cl3_pmon_cnt_v(%p)\n",
	      pl2_pmon_cnt_v,cl3_pmon_cnt_v); 
    close(fd);
  }
  
  init_pmoncnt();
}

void dump_pmoncnt(char* str)
{
  long i,j;
  uint64_t var;
  dbgprintf("%s-%d,",__FUNCTION__,__LINE__);
  dbgprintf("pmoncnt(%p),pl2_pmon_cnt_v(%p),cl3_pmon_cnt_v(%p)\n",
	    pmoncnt,pl2_pmon_cnt_v,cl3_pmon_cnt_v); 

  /*  for (i=0; i<2; i++){
    for (j=0; j< (CNT_CSR+CNT_PL2+CNT_CL3); j++){
      printf("%ju,", pmoncnt[i][j]);
    }
    printf("\n");
    } */

  if(str!=NULL) printf("%s,",str);
  for (j=0; j< (CNT_CSR+CNT_PL2+CNT_CL3); j++){
    var = pmoncnt[1][j] - pmoncnt[0][j];
    printf("%ju,", var);
  }
  printf("\n");
}


void store_pmoncnt(int n)
{
  long i;

  dbgprintf("%s-%d,",__FUNCTION__,__LINE__);
  dbgprintf("pmoncnt(%p),pl2_pmon_cnt_v(%p),cl3_pmon_cnt_v(%p)\n",
	    pmoncnt,pl2_pmon_cnt_v,cl3_pmon_cnt_v); 
  dbgprintf("rdcycle(%jx)\n",rdcycle());
  dbgprintf("rdinstret(%jx)\n",rdcycle());
  dbgprintf("rdtime(%jx)\n",rdtime());
  dbgprintf("rdpm3(%jx)\n",rdpm3());
  dbgprintf("rdpm4(%jx)\n",rdpm4());
  dbgprintf("rdpm5(%jx)\n",rdpm5());
  dbgprintf("rdpm6(%jx)\n",rdpm6());
  pmoncnt[n][0]=rdcycle();
  pmoncnt[n][1]=rdinstret();
  pmoncnt[n][2]=rdtime();
  pmoncnt[n][3]=rdpm3();
  pmoncnt[n][4]=rdpm4();
  pmoncnt[n][5]=rdpm5();
  pmoncnt[n][6]=rdpm6();
  
  dbgprintf("%s-%d\n",__FUNCTION__,__LINE__);
  for (i=0;i<CNT_PL2;i++){
    pmoncnt[n][i+CNT_CSR]=pl2_pmon_cnt_v[i];
  }

  dbgprintf("%s-%d\n",__FUNCTION__,__LINE__);
  for (i=0;i<CNT_CL3;i++){
    pmoncnt[n][i+CNT_CSR+CNT_PL2]=cl3_pmon_cnt_v[i];
  }

  dbgprintf("%s-%d\n",__FUNCTION__,__LINE__);
}

