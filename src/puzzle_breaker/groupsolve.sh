#!/bin/bash

#####################PREPARATION OF CMD############################################
ARG1=$1
Num1=$1

echo $ARG1

# cmd="../../../../../bin/sp ."
# cmd="$cmd \"$ARG1 $ARG1"
#echo $cmd

cmd1="."

cmd2="$ARG1 $ARG1"
for ((i=0; i<$1; ++i))
do
    for ((j=0; j<$1; ++j))
    do
	cmd2="$cmd2 0"
    done
done
cmd2="$cmd2"

cmd3="0" #off debug
echo $cmd1 $cmd2 $cmd3
#####################END OF PREPARATION OF CMD############################################

cd "generated_puzzles"
a=$((Num1))

cd "Puzzles_Size_$a"



for number2 in {1..100}
do
    echo "solving $number2"
    cd "Size_${a}_Puzzle_${number2}"
    #    $cmd

    /usr/bin/time -f "%E,%U,%S" -a --output="../results2.csv" ../../../../../bin/sp "$cmd1" "$cmd2" "$cmd3" >> "../results2.txt"
    cd ..
done

exit 0

# to be called from actual ../../../../../bin/sp . "6 6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" 1

# to be called from actual ../../../../../bin/sp . "8 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" 1
