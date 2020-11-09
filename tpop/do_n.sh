#!/bin/bash

N=3
if [ $# -gt 0 ]
then
    N=$1
fi

i=1
TIMEFORMAT='%R'
min="10000"
max="0"
sum="0"
avg="0"
while [ "$i" -le "$N" ]; do
    t="$(time ( ./markov < Genesis.txt > dump.txt) 2>&1 > /dev/null)"
    sum="$(echo $sum + $t | bc -l)"
    echo "$i: $t"
    i=$(($i + 1))
    if (( $(echo "$t < $min" | bc -l) )); then
        min=$t
    fi
    if (( $(echo "$t > $max" | bc -l) )); then
        max=$t
    fi
done

rm dump.txt

echo  "MINIMUM: $min"
echo  "MAXIMUM: $max"

avg="$(echo "$sum / $N" | bc -l)"
echo -n "AVERAGE: "
printf "%.*f\n" "3" "$avg"
