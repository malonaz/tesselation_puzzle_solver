#!/bin/bash

#NUMBERS20="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20"
#NUMBERS20=

cd "generated_puzzles"
for a in {38..50..2}
do
  cd "Puzzles_Size_$a"
  rm "results2.csv"
  rm "results2.txt"
  for number2 in {1..100}
  do
      cd "Size_${a}_Puzzle_${number2}"
      rm -rf solutions/*

      cd ..
  done
  cd ..
done
exit 0


#6 6 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
