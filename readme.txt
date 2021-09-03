files created by james villeneuve but are really simple so prob have been done before. 
This is the most efficient conversion from decimal to string hex i can find and create. it is a simple lookup table. it does take 512 bytes but for deeply nested loops where more than one char is read it is incredibly fast on 64bit arm neon. specifically tested on rock3328 (pine rock64)


ARM RISC processors normally are incredibly efficient at memory access and calls
but slower at doing math concurrently in nested loops, so although parallelism can be achieved in math problems, a deep nested loop will be limited because writes need to happen to same registers for math, and this limits tasking.

a more efficient method is to use memory mapping at least in my testing
as this involves a look up table and more than likely direct calls to memory.
in nested loops mem table will be cached so speed is quite fast.

the speed is from the storage of variables as global so unloading and loading of values in stack at time of function call does not slow it down, and the values stay in cache as used so access is immediate.
