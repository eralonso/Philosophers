#!/bin/bash

if [[ $# < 6 || $# > 7 ]]; then
	echo ""
	echo -e "\033[1;93mInvalid number of arguments\033[0m"
	echo ""
	echo -e "\033[1;92mUsage: \033[1;94m./script.sh N_LOOPS T_DELAY N_PHILO TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP <N_TIMES_EAT>\033[0m"
	echo ""
	exit 1
fi

mkfs="make --warn-undefined-variables --no-print-directory"

$mkfs > /dev/null



LOOPS=$1
T_DELAY=$2
N_PHILO=$3
T_DIE=$4
T_EAT=$6
T_SLP=$6
if [ $# == 7 ]; then
	TT_EAT=$7
else
	TT_EAT=""
fi
i=0

while [ $i -lt $LOOPS ]; do
	echo -e "\n\t\033[1;94mLOOP $((i + 1))\033[0m\n"
	OUT=$(./philo $N_PHILO $T_DIE $T_EAT $T_SLP $TT_EAT)
	echo -e "$OUT\n"
	read -p "Dead Time Analysis [ Y / N ]:
" ANALYSIS
	while [[ $ANALYSIS != "Y" && $ANALYSIS != "N" && $ANALYSIS != "y" && $ANALYSIS != "n" ]]; do
		read -p "
Invalid param. Write a valid param [ Y / N ]:
" ANALYSIS
	done
	if [[ $ANALYSIS == "Y" || $ANALYSIS == "y" ]]; then
		echo -e "$OUT" | awk -v DIE="$T_DIE" '($3 == "died" && $1 > DIE + 10) || $1 < DIE {print "Invalid dead time for philo " $2}'
	fi
	if [ $((i + 1)) -lt $LOOPS ]; then
		sleep $T_DELAY
	fi
	((i++))
done

echo ""

#other Script
# while [ $i -lt $MAX ]; do
	
	# ARG=$(./philo $N_PHILO $T_DIE $T_EAT $T_SLP $TT_EAT)
	# echo $ARG
	# len=${#ARG}
	# j=0
	# k=1
	
	# while [ $((j+2)) -lt $len ]; do
	
	# 	N1=${ARG:$j:$k}
	# 	N2=${ARG:$j+2:$k+2}
	# 	# echo "$N1"
	# 	# echo "$N2"
	# 	echo "$j"
	# 	echo "$k"

	# 	if [ $N1 -gt $N2 ]; then
	# 		echo "mal"
	# 	fi

	# 	((j += 2))
	# 	((k += 2))

	# done
	
# 	((i++))

# done
