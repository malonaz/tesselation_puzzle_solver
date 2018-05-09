#!/bin/bash

NUMBERS="3 4 5 6 7 8 9 10"
#NUMBERS20="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20"
#NUMBERS20=

mkdir "generated_puzzles"
cd "generated_puzzles"

for number in $NUMBERS
do
    a=$((number*2))
    mkdir "Puzzles_Size_$a"
    for number2 in {1..100}
    do
	cd "Puzzles_Size_$a"
	mkdir "Size_${a}_Puzzle_${number2}"
	cd "Size_${a}_Puzzle_${number2}"
	mkdir "solutions"
	../../../../../bin/pb $a
	cd ../..
    done
done
exit 0

    
#6 6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
#../../../../../bin/sp . "6 6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" 1
