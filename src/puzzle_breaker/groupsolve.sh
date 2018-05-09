#!/bin/bash

#####################PREPARATION OF CMD############################################
ARG1=$1
Num1=$1

echo $ARG1

cmd="../../../../../bin/sp ."
cmd="$cmd \"$ARG1 $ARG1"
echo $cmd

for ((i=0; i<Num1; ++i))
do
    for ((j=0; j<Num1; ++j))
    do
	cmd="$cmd 0 "
    done
done				

cmd="$cmd \" 0" #off debug
echo $cmd
#####################END OF PREPARATION OF CMD############################################

cd "generated_puzzles"
cd "Puzzles_Size_$Num1"


for number2 in {1..100}
do
    cd "Size_${a}_Puzzle_${Num1}"
    $cmd
    cd ..
done


exit 0

# to be called from actual ../../../../../bin/sp . "6 6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" 1
