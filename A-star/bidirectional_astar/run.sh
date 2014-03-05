#bin/bash

a=$(tail -1 f1 | awk '{print $1}')
b=$(tail -1 f2 | awk '{print $1}')

c=$(grep $a f2 | wc -l)
d=$(grep $b f1 | wc -l)

# echo "file1" $a
# echo "file2" $b

if [ $c -eq 1 ] 
  then
  tail -1 f1 | awk '{print $2}' > file1
  grep $a f2 | awk '{print $2}' > file2
  kill -31 $2
  kill -31 $1
elif [ $d -eq 1 ] 
  then
  tail -1 f2 | awk '{print $2}' > file2
  grep $b f1 | awk '{print $2}' > file1
  kill -31 $2
  kill -31 $1
fi
