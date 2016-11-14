#!/bin/bash

MAX_RUNNING_TIME=10

# Show help usage
if [ "$#" -eq "0" ] || [ "$#" -gt "2" ]
then
	printf "Usage: \n\t./simulate.sh input output\n"
	exit 0
fi

# Check if there are a input file
if [ -z "$1" ]
then
	echo "[ERROR] NOT SPECIFIED INPUT FILE!"
	exit 1
fi

# Check if output file is specified
if [ -z "$2" ]
then
    echo "[ERROR] NOT SPECIFIED OUTPUT FILE!"
    exit 1
fi

# Check if compiler exists and can be executed
if [ -e "compilador" ] && [ -s "compilador" ] && [ -x "compilador" ] && [ -e "$1" ]
then
    echo "Compiling and testing program..."
else
    echo "[ERROR] COMPILER OR INPUT FILE DON'T EXIST, IS EMPTY OR CAN'T BE EXECUTABLE!"
	exit 1
fi

# Execute compiler, generate mips code pass this to spim and put the output in a file
( timeout $MAX_RUNNING_TIME ./compilador "$1" "$2.mips" > /dev/null 2>&1 && \
timeout $MAX_RUNNING_TIME spim -f "$2.mips" | tail -n +6 | head -n 10000 > "$2" ) || \
( echo "Failed to compile program" && exit 1 )

# Program simulated with success
echo "Simulation Finished!"
exit 0
