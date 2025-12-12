#!/bin/bash

iterations=1000

while getopts "n:" flag; do
    case "$flag" in
        n) iterations=$OPTARG ;;
    esac
done
shift $((OPTIND-1))

if [[ $# -lt 2 ]]; then
    exit 1
fi

prog=$1
prog_arg=$2

(
for i in $(seq 1 $iterations); do
    "$prog" "$prog_arg" | grep -oP "(?<=Time taken: )[0-9]+";
done
) | awk '{sum+=$1; if(min==""){min=$1}; if($1<min){min=$1}} END {print "Min time: " min " μs\nAvg time: " sum/NR " μs"}'

