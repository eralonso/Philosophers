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

function dead_time_analyzer()
{
	echo ""
	PHILO=$(echo -e "$OUT" | awk '
	BEGIN {philo=0}
	{if ($3 == "died")
		{philo=$2}
	if ($1 < 0)
		{philo=-1}}
	END {print philo}
	')
	if [ $PHILO == -1 ]; then
		echo -e "Fatal error with the time of dead:\n\t\033[1;93mIS NEGATIVE WTF 😵!!!\033[0;m"
		return ;
	fi
	if [ $PHILO != 0 ]; then
		echo -e "$OUT" | awk -v DIE="$T_DIE" -v P="$PHILO" '
		BEGIN {lt_eat=-1; t_died=-1}
		{
			if ($2 == P && $4 == "eating")
			{
				lt_eat=$1
			}
			else if ($2 == P && $3 == "died")
			{
				t_died=$1
			}
		}
		END {
			if (lt_eat != -1 && t_died != -1 && (t_died - lt_eat) - 10 >= DIE)
			{
				print "Bad dead time with philo " P ": Die after could be die"
			}
			else
			{
				print "Dead time is OK"
			}
		}
		'
	fi
	return ;
}

LOOPS=$1
T_DELAY=$2
N_PHILO=$3
T_DIE=$4
T_EAT=$5
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
	echo -e "$OUT"
	ANY_DEAD=$(echo -e "$OUT" | awk '
	BEGIN {res=0}
	$3 == "died" {res=1} END {print res}
	')
	if [ $ANY_DEAD == 1 ]; then
		echo -e "\n"
		read -p "Dead Time Analysis [ Y / N ]:
" ANALYSIS
		while [[ $ANALYSIS != "Y" && $ANALYSIS != "N" && $ANALYSIS != "y" && $ANALYSIS != "n" ]]; do
		read -p "
Invalid param. Write a valid param [ Y / N ]:
" ANALYSIS
		done
		if [[ $ANALYSIS == "Y" || $ANALYSIS == "y" ]]; then
			dead_time_analyzer
		fi
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
