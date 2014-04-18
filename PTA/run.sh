#!/bin/bash

rm -f outputs
touch outputs
for (( c=2; c<=10; c++))
do
	# echo $c
	./a.out $c
done
