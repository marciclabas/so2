#ifndef MM_ADDRESS_H
#define MM_ADDRESS_H

#define ENTRY_DIR_PAGES       0

#define TOTAL_PAGES 1024
#define NUM_PAG_KERNEL 256
#define PAG_LOG_INIT_CODE (PAG_LOG_INIT_DATA+NUM_PAG_DATA)
#define FRAME_INIT_CODE (PH_USER_START>>12)
#define NUM_PAG_CODE 8
#define PAG_LOG_INIT_DATA (L_USER_START>>12)
#define NUM_PAG_DATA 20
#define PAGE_SIZE 0x1000

#define PAG_LOG_INIT_COPY (PAG_LOG_INIT_CODE+NUM_PAG_CODE)
#define NUM_PAG_COPY 20
#define COPY_START (PAG_LOG_INIT_COPY << 12)

#define PAG_LOG_INIT_TEMP (PAG_LOG_INIT_COPY+NUM_PAG_COPY)
#define NUM_PAG_TEMP (TOTAL_PAGES)
#define TEMP_START (PAG_LOG_INIT_TEMP << 12)

#define PAG_LOG_INIT_HEAP (PAG_LOG_INIT_TEMP+NUM_PAG_TEMP)
#define NUM_PAG_HEAP (TOTAL_PAGES)
#define HEAP_START (PAG_LOG_INIT_HEAP << 12)

/*
  ADDRESS SPACE LAYOUT

  |--------------------------|
  | KERNEL (NUM_PAG_KERNEL)  |
  |--------------------------| <-- PAG_LOG_INIT_DATA
  | DATA (NUM_PAG_DATA)      | 
  |--------------------------| <-- PAG_LOG_INIT_CODE
  | CODE (NUM_PAG_CODE)      |
  |--------------------------| <-- PAG_LOG_INIT_COPY
  | COPY (NUM_PAG_DATA)      |
  |--------------------------| <-- PAG_LOG_INIT_TEMP
  | TEMP (TOTAL_PAGES)       |
  |--------------------------| <-- PAG_LOG_INIT_HEAP
  | HEAP (TOTAL_PAGES)       |
  |--------------------------|

  - COPY: used when copying data from parent to child
  - TEMP: used when copying heap from parent to child

*/

/* Memory distribution */
/***********************/


#define KERNEL_START     0x10000
#define L_USER_START        0x100000
#define USER_ESP	L_USER_START+(NUM_PAG_DATA)*0x1000-16

#define USER_FIRST_PAGE	(L_USER_START>>12)

#define PH_PAGE(x) (x>>12)

#endif

