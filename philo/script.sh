#!/bin/bash

argc=$#
argv=($@)

if [[ ${argv[0]} == "-h" || ${argv[0]} == "--help" ]]; then
	echo -e "\033[1;92mUsage: \033[1;94m./script.sh <options: [--help/-h...]> N_LOOPS T_DELAY N_PHILO TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP <N_TIMES_EAT>\n\033[0m"
	echo -e "\033[1;93mOptions:"
	echo -e "-cd0: No Check Death"
	echo -e "-cd1: Check Death - level 1"
	echo -e "-cd2: Check Death - level 2"
	echo -e "-cda: Check Death - all levels"
	exit 1;
fi

level=-0

if [ ${argv[0]} == "-cd0" ]; then
	level=-1
elif [ ${argv[0]} == "-cd1" ]; then
	level=1
elif [ ${argv[0]} == "-cd2" ]; then
	level=2
elif [ ${argv[0]} == "-cda" ]; then
	level=3
elif [ ${argv[0]:0:3} == "-cd" ]; then
	echo -e "\033[1;91mIncorrect Check Death level\n\n\033[1;93mFor info use ./script.sh [-h/--help]\033[0m"
	exit 1
fi

if [[ ${argv[0]} == "-cd0" || ${argv[0]} == "-cd1" || ${argv[0]} == "-cd2" || ${argv[0]} == "-cda" ]]; then
	((argc--))
	unset argv[0]
	argv=(${argv[@]})
fi

if [[ $argc < 6 || $argc > 7 ]]; then
	echo ""
	echo -e "\033[1;93mInvalid number of arguments\033[0m"
	echo ""
	echo -e "\033[1;92mUsage: \033[1;94m./script.sh <options: [--help/-h...]> N_LOOPS T_DELAY N_PHILO TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP <N_TIMES_EAT>\033[0m"
	echo ""
	exit 1
fi

mkfs="make --warn-undefined-variables --no-print-directory"

$mkfs &> /dev/null

if [ $? -ne 0 ]; then
	echo "Error trying to execute makefile"
	exit 1
fi

function 	all_philos_time_analyzer(){
	OK=1
	j=1
	while [ $j -le $N_PHILO ]; do
		k=2
		ITERATIONS=$(echo -e "$OUT" | grep "$j is eating" | wc -l)
		if [ $ITERATIONS -eq 1 ]; then
			F_EAT=$(echo -e "$OUT" | grep "$j is eating" | awk '{print $1}')
			if [[ $F_EAT -ge $T_DIE && $DEAD_TIME -gt $(((F_EAT - $INIT_EAT_TIME) + 10)) ]]; then
				echo -e "\t\033[1;91mKO\033[1;37m: Bad dead time with philo \033[1;94m$j\033[1;37m: Die after could be die -> Ultimate eat: $F_EAT; Time died: $DEAD_TIME: Time could be die: $((F_EAT + $T_DIE))"
				OK=0
			fi
		else
			while [ $k -le $ITERATIONS ]; do
				F_EAT=$(echo -e "$OUT" | grep "$j is eating" | head -$k | tail -2 | head -1 | awk '{print $1}')
				S_EAT=$(echo -e "$OUT" | grep "$j is eating" | head -$k | tail -1 | awk '{print $1}')
				if [[ $((S_EAT - $F_EAT)) -ge $T_DIE && $DEAD_TIME -gt $((S_EAT + 10)) ]]; then
					echo -e "\t\033[1;91mKO\033[1;37m: Bad dead time with philo \033[1;94m$j\033[1;37m: Die after could be die -> Penultimate eat: $F_EAT; Ultimate eat: $S_EAT: Time could be die: $((F_EAT + $T_DIE))"
					OK=0
				fi
				((k++))
			done
		fi
		((j++))
	done
	if [[ $OK == 1 ]]; then
		echo -e "\t\033[1;92mOK\033[0m"
	fi
	return ;
}


function dead_time_analyzer()
{
	if [[ $level -eq 0 || $level -eq 1 || $level -eq 3 ]]; then
		echo -e "\n\033[1mTest 1 - \033[1;93mCheck the two last meals (of the philo dead) and the dead time:\033[0m"
		PHILOS_DIED=$(echo -e "$OUT" | grep "died" | wc -l)
		if [ $PHILOS_DIED -gt 1 ]; then
			echo "\t\033[1;91mKO: More than 1 philo died -> ($PHILOS_DIED)\033[0m"
			return ;
		fi
		POST_DEAD=$(echo -e "$OUT" | tail -1 | awk 'BEGIN {res=0} $3 != "died" {res=1} END {print res}')
		if [ $POST_DEAD -eq 1 ]; then
			echo -e "\t\033[1;91mKO\033[1;37m: There are messages after the death of a philo!!!\033[0m"
			return ;
		fi
		WEIRD_TIME=$(echo -e "$OUT" | awk 'BEGIN {weird=0} $1 < 0 {weird=-1} END {print weird}')
		if [ $WEIRD_TIME == -1 ]; then
			echo -e "\t\033[1;91mKO\033[1;37m: Fatal error with the time:\n\t\033[1;93mIS NEGATIVE WTF 😵!!!\033[0m"
			return ;
		fi
		INIT_TIME=$(echo -e "$OUT" | head -1 | awk '{print $1}')
		INIT_EAT_TIME=$(echo -e "$OUT" | grep "eating" | head -1 | awk '{print $1}')
		PHILO=$(echo -e "$OUT" | grep "died" | awk '{print $2}')
		TIMES_EAT=$(echo -e "$OUT" | grep " $PHILO is eating" | tail -2 | wc -l)
		DEAD_TIME=$(echo -e "$OUT" | grep " $PHILO died" | awk '{print $1}')
		LT_EAT=$(echo -e "$OUT" | grep " $PHILO is eating" | tail -1 | awk '{print $1}')
		if [ $TIMES_EAT -gt 1 ]; then
			PN_EAT=$(echo -e "$OUT" | grep " $PHILO is eating" | tail -2 | head -1 | awk '{print $1}')
			if [[ $((DEAD_TIME - $LT_EAT)) -gt $((T_DIE + 10)) ]]; then
				echo -e "\t\033[1;91mKO\033[1;37m: Bad dead time with philo \033[1;94m$PHILO\033[1;37m: Die after could be die -> Penultimate eat: $PN_EAT; Ultimate eat: $LT_EAT: Time could be die: $((LT_EAT + $T_DIE))"
			elif [[ $((DEAD_TIME - $PN_EAT)) -gt $((T_DIE + 10)) ]]; then
				echo -e "\t\033[1;91mKO\033[1;37m: Bad dead time with philo \033[1;94m$PHILO\033[1;37m: Die after could be die -> Penultimate eat: $PN_EAT; Ultimate eat: $LT_EAT: Time could be die: $((PN_EAT + $T_DIE))"
			else
				echo -e "\t\033[1;92mOK\033[0m"
			fi
		elif [ $TIMES_EAT -lt 1 ]; then
			if [[ $DEAD_TIME -gt $((T_DIE + 10)) ]]; then
				echo -e "\t\033[1;91mKO\033[1;37m: Bad dead time with philo \033[1;94m$PHILO\033[1;37m: Die after could be die -> Time could be die: $T_DIE"
			else
				echo -e "\t\033[1;92mOK\033[0m"
			fi
		else
			if [[ $((DEAD_TIME - $LT_EAT)) -gt $((T_DIE + 10 + $INIT_TIME)) ]]; then
				echo -e "\t\033[1;91mKO\033[1;37m: Bad dead time with philo \033[1;94m$PHILO\033[1;37m: Die after could be die -> Ultimate eat: $LT_EAT: Time could be die: $((LT_EAT + $T_DIE))"
			elif [[ $DEAD_TIME -gt $((T_DIE + 10)) ]]; then
				echo -e "\t\033[1;91mKO\033[1;37m: Bad dead time with philo \033[1;94m$PHILO\033[1;37m: Die after could be die -> Time could be die: $T_DIE"
			else
				echo -e "\t\033[1;92mOK\033[0m"
			fi
		fi
	fi
	if [[ $level -eq 0 || $level -gt 1 ]]; then
		echo -e "\n\033[1mTest 2 - \033[1;93mCheck all meals of all philos - (Maybe the result is equal or similarity):\033[0m"
		all_philos_time_analyzer
	fi
	return ;
}


LOOPS=${argv[0]}
T_DELAY=${argv[1]}
N_PHILO=${argv[2]}
T_DIE=${argv[3]}
T_EAT=${argv[4]}
T_SLP=${argv[5]}
if [ $argc == 7 ]; then
	TT_EAT=${argv[6]}
else
	TT_EAT=""
fi
i=0

while [ $i -lt $LOOPS ]; do
	echo -e "\n\t\033[1;94mLOOP $((i + 1))\033[1;37m\n"
	OUT=$(./philo $N_PHILO $T_DIE $T_EAT $T_SLP $TT_EAT)
	echo -e "$OUT"
	ANY_DEAD=$(echo -e "$OUT" | awk '
	BEGIN {res=0}
	$3 == "died" {res=1} END {print res}
	')
	if [ $ANY_DEAD == 1 ]; then
		echo -e "\n"
		if [ $level -eq 0 ]; then
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
		else
			dead_time_analyzer
		fi
	fi
	if [ $((i + 1)) -lt $LOOPS ]; then
		sleep $T_DELAY
	fi
	((i++))
done

echo ""