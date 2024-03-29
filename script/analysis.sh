#!/bin/bash

perf_events='cpu-clock,cpu-cycles,instructions,LLC-misses'

# On intel architectures turn off turboboost and hyperthreading
#echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo
echo 0 > /sys/devices/system/cpu/cpufreq/boost
chcpu -d 7


# Turning off RT throttling
echo -1 >/proc/sys/kernel/sched_rt_runtime_us


# Read the tests
j=0
while read i
do
	tacle_prog[$j]=$i
	j=$(( $j + 1 ))
done <script/tacleNames.txt

j=0

for f in 800mhz 2100mhz
do

	if [ ! -d "results_$f" ]; then
		mkdir results_$f
	fi

	if [ ! -d "results_$f/wcet" ]; then
		mkdir results_$f/wcet
	fi

	if [ ! -d "results_$f/cpucycles" ]; then
		mkdir results_$f/cpucycles
	fi

	if [ ! -d "results_$f/instructions" ]; then
		mkdir results_$f/instructions
	fi

	if [ ! -d "results_$f/l3miss" ]; then
		mkdir results_$f/l3miss
	fi

	# Set the frequency for testing
	cpupower --cpu all frequency-set -d $f -u $f
	for t in {0..22}
	do
		prog=${tacle_prog[$t]}
		# Test if the result files exist
		if test ! -f results_$f/wcet/$prog.csv; then
			touch results_$f/wcet/$prog.csv
		fi

		if test ! -f results_$f/cpucycles/$prog.csv; then
			touch results_$f/cpucycles/$prog.csv
		fi

		if test ! -f results_$f/instructions/$prog.csv; then
			touch results_$f/instructions/$prog.csv
		fi

		if test ! -f results_$f/l3miss/$prog.csv; then
			touch results_$f/l3miss/$prog.csv
		fi

		for i in {1..500}
		do
			perf stat -C 3 -e $perf_events ./prof/wrapper bench/sequential/$prog/a.out 2>&1 >/dev/null | grep -e 'clock' -e 'cpu-cycles' -e 'instructions' -e 'LLC-misses' | awk '{print $1}' |\
			tee >(awk 'NR % 4 == 0' >>results_$f/l3miss/$prog.csv) >(awk 'NR % 4 == 1' >>results_$f/wcet/$prog.csv) >(awk 'NR % 4 == 2' >> results_$f/cpucycles/$prog.csv)\
			>(awk 'NR % 4 == 3' >> results_$f/instructions/$prog.csv)
		done
	done
done

