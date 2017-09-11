#!/bin/bash

SUCC=0

for i in `seq 1000`; do 
	echo $i
	ret=$(./test_crypto | grep "Success" | wc -l) 
	SUCC=$((SUCC+ret))
done

echo "SUCC = $SUCC"
