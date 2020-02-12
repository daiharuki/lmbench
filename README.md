README for lmbench 3alpha9 net release.

To run the benchmark, you should be able to say:

	cd src
	make results

If you want to see how you did compared to the other system results
included here, say

	make see

Be warned that many of these benchmarks are sensitive to other things
being run on the system, mainly from CPU cache and CPU cycle effects.
So make sure your screen saver is not running, etc.

It's a good idea to do several runs and compare the output like so

	make results
	make rerun
	make rerun
	make rerun
	cd Results && make LIST=<your OS>/*

------------

For Risc-V Fedora RFS doesn't have the rpc/rpc.h installed. Following works around. 
 - install tirpc package <br />
  $ dnf install libtirpc-devel
  - Specify flags in either the FLAGS <br />

Following necessary flags are set in the src/Makefile <br />
   CFLAGS="-I/usr/include/tirpc" <br />
   LFLAGS="-ltirpc" <br />

Additionally, following tune up flags are used conforming the CPU pipeline.

For FU54
	-O3 -mtune=rocket -mtune=sifive-5-series
For U74
	-O3 -mtune=sifive-7-series

Optionally, following optimization flags are considered:

-mcmodel=medlow or -mcmodel=medany <br />
-mexplicit-relocs or -mno-exlicit-relocs <br />

In the case -O3 switch costs the code size more than desired, following switch would help generate the load/store double where the 4B load/store can be merged to 8B load/store. <br />
-fvect-cost-model 


Ex.
	CFLAGS="-O3 -mtune=rocket"  make

