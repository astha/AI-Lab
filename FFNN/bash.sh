#bin/bash

g++ main.cpp -std=c++0x

for i in {0..9}
do
	# echo '22/7' | bc -l
	# echo $i
	echo 0.0$i  $(./a.out 0.5 0.0$i  c)
done

for i in {10..99}
do
	# echo '22/7' | bc -l
	# echo $i
	echo 0.$i $(./a.out 0.5  0.$i  c)
done