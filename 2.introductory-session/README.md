# 2.6 Stuff to do

## 2.6.1. Initial steps

1. Generate ZeOS image: `make`
2. Examine make output
  
3. Run image in bochs: `make emuldbg`, press `c` + `Enter`
   
4. What instruction is executing? 0x0000fffffff0?
5. View `objdump -d` output. Where is the instruction located? (what file and line)