Files created by James villeneuve but are really simple so prob have been done before. 
This is the most efficient conversion from decimal to string hex i can find and create. it is a simple lookup table. it does take at least 512 bytes-514bytes if string terminator used and some char buffers but for deeply nested loops where more than one char is read it is incredibly fast on 64bit arm neon. specifically tested on rock3328 (pine rock64) at about 2 bytes converted per clock cycle, and possibly 16 bytes or more per clock using O3 optimized mode.It might not work as well in a complex system environment if software is loading up cache constantly.

ARM RISC processors normally are incredibly efficient at memory access and calls,
but slower at doing math concurrently in nested loops, so although parallelism can be achieved in math problems, a deep nested loop will be limited because writes need to happen to same registers for math, and this limits tasking.

A more efficient method is to use memory mapping at least in my testing
as this involves a look up table and more than likely direct calls to memory.
In nested loops memory table will be cached so speed is quite fast.

The speed is from the storage of variables as global so unloading and loading of values in stack at time of function call does not slow it down, and the values stay in cache as used so access is immediate.


I will be updating soon with a read of large hex numbers to hex values in a string soon. There is not a set timeline for that, but additional example files will be shown and code split into libraries and examples modified to use the libraries.
