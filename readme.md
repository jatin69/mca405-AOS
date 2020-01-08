# mca405-AOS

Coursework for mca405 - Advance Operating Systems

Followed book is `The Design of the UNIX Operating System - Maurice Bach`. Notes can be found [here](https://github.com/suvratapte/Maurice-Bach-Notes).

Related project is [Buffer Cache Simulation](https://github.com/jatin69/buffer-cache-simulation)

## Resources

- [C++ programming](https://boqian.weebly.com/c-programming.html)
- [C++ - Threads](https://www.youtube.com/watch?v=IdwJdJw65tU)
- [C++ 11 - Multithreading](https://www.bogotobogo.com/cplusplus/multithreaded4_cplusplus11B.php#option1)
- [C++ 11/14 - Deadlocks](https://www.bogotobogo.com/cplusplus/C11/9_C11_DeadLock.php
)
- [kernel reference](https://www.halolinux.us/kernel-reference/)

## About the Book 

- `Title` The Design of the UNIX Operating System
- `Author`	Maurice J. Bach
- This is one of the books that Linus used to design Linux. 
- It is a description of the data structures used in the System V kernel. 
- Many of the names of the important functions in the Linux source come from this book, and are named after the algorithms presented here. For instance, if you can't quite figure out what exactly getblk(), brelse(), bread(), breada(), and bwrite() are, chapter 3 explains very well.
- While most of the algorithms are similar or the same, a few differences are worth noting:
	- The Linux buffer cache is dynamically resized, so the algorithm for dealing with getting new buffers is a bit different. Therefore the above referenced explanation of getblk() is a little different than the getblk() in Linux.
	- Linux does not currently use streams, and if/when streams are implemented for Linux, they are likely to have somewhat different semantics.
	- The semantics and calling structure for device drivers is different. The concept is similar, and the chapter on device drivers is still worth reading, but for details on the device driver structures, the KHG is the proper reference.
	- The memory management algorithms are somewhat different. 
- There are other small differences as well, but a good understanding of this text will help you understand the Linux source.