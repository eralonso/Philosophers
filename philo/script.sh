#!/bin/bash

if [ $# -gt 3 ]; then
	echo "Error to many arguments"
	exit
fi

mkfs="make --warn-undefined-variables --no-print-directory"

$mkfs > /dev/null

MAX=100
N_PHILO=2

if [ $# == 1 ]; then
	N_PHILO=$1
elif [ $# == 2 ]; then
	N_PHILO=$1
	MAX=$2
fi

i=0

while [ $i -lt $MAX ]; do
	
	ARG=$(./philo $N_PHILO)
	len=${#ARG}
	j=0
	k=1
	
	while [ $((j+2)) -lt $len ]; do
	
		N1=${ARG:$j:$k}
		N2=${ARG:$j+2:$k+2}
		# echo "$N1"
		# echo "$N2"
		echo "$j"
		echo "$k"

		if [ $N1 -gt $N2 ]; then
			echo "mal"
		fi

		((j += 2))
		((k += 2))

	done
	
	((i++))

done
