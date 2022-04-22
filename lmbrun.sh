#!/bin/sh


set -x
$PERFCMD ./lat_mem_rd -P 1 -W 4 -N 3 64M 64
$PERFCMD ./lat_mem_rd -t -P 1 -W 4 -N 3 64M 64
for CPUS in 1 
do 
 echo "cpu=$CPUS"
 echo BW_MEM32
 for WHAT in rd wr rdwr cp fwr frd fcp bzero bcopy;
 do
  echo "work=$WHAT"
  for SIZE in 1k 2k 4k 8k 16k 32k 64k 128k 256k 512k 1m 2m 4m 8m 16m 32m 64m;
  do
   $PERFCMD ./bw_mem -P $CPUS -W 4 -N 3 $SIZE $WHAT ;
 done; done; 
 echo "BW_MEM64"
 for WHAT in rd wr rdwr cp fwr frd fcp bzero bcopy;
 do
  echo "work=$WHAT"
  for SIZE in 1k 2k 4k 8k 16k 32k 64k 128k 256k 512k 1m 2m 4m 8m 16m 32m 64m;
  do
   $PERFCMD ./bw_mem64 -P $CPUS -W 4 -N 3 $SIZE $WHAT ;
 done; done;

for ver in 1 2
do 
  for SIZE in 1k 2k 4k 8k 16k 32k 64k 128k 256k 512k 1m 2m 4m 8m 16m 32m 64m 240000000;
  do
      $PERFCMD ./stream -v $ver -M $SIZE -W 4 -N 3
  done
done

for ver in 1 2
do 
  for SIZE in 1k 2k 4k 8k 16k 32k 64k 128k 256k 512k 1m 2m 4m 8m 16m 32m 64m 240000000;
  do
      $PERFCMD ./stream_org -v $ver -M $SIZE -W 4 -N 3
  done
done

done
