#!/bin/bash

# ------------ Constants ------------
declare -r argument_count=$#
declare -r arguments=$@

declare -r data_type_arguments=("-t1" "-t2" "-t3" "-p1" "-p2" "-p3" "-w" "-h" "-m") #Required, not exclusive
declare -r location_arguments=("-F" "-G" "-S" "-A" "-O" "-Q") #Exclusive between each other
declare -r date_argument="-d" #Optional
declare -r sort_arguments=("--tab" "--abr" "--avl") #Exclusive between each other
declare -r file_argument="-f" #Required
declare -r help_argument="--help" #Optional

declare -r all_arguments=("${data_type_arguments[@]}" "${location_arguments[@]}" "${sort_arguments[@]}" "${date_argument}" "${file_argument}" "${help_argument}")

# ----------- Variables -------------
IsFileArgumentSpecified=false
IsOneDataArgumentSpecified=false
IsOneLocationArgumentSpecified=false
IsOneSortArgumentSpecified=false
ShouldNextArgumentBePath=false
Path=""

for argument in $arguments
do
	if [[ "$ShouldNextArgumentBePath" = true ]]; then
		# [WARNING] THIS WILL NOT WORK IF THE PATH CONTAINS SPACES
		# Should be fixed.
		Path=$argument
		ShouldNextArgumentBePath=false

		# We move onto the next arguments
		continue
	fi

	# Did the user enter any invalid argument ?
	if [[ ! " ${all_arguments[@]} " =~ " ${argument} " ]]; then
		echo "Invalid argument : ${argument}"
		exit 1
	fi

	# Did the user use the -f argument ?
	if [[ " -f " == " ${argument} " ]]; then
		if [[ "$IsFileArgumentSpecified" = true ]]; then
			echo "Only one file argument can be specified."
			exit 1
		fi
		IsFileArgumentSpecified=true
		ShouldNextArgumentBePath=true
	fi

	# Did the user use one of the data type arguments ?
	for data_type_argument in "${data_type_arguments[@]}"
	do
		if [[ " ${data_type_argument} " == " ${argument} " ]]; then
			IsOneDataArgumentSpecified=true
		fi
	done

	# Did the user use more than one location argument ?
	for location_argument in "${location_arguments[@]}"
	do
		if [[ " ${location_argument} " == " ${argument} " ]]; then
			if [[ "$IsOneLocationArgumentSpecified" = true ]]; then
				echo "Only one location argument can be specified."
				exit 1
			fi
			IsOneLocationArgumentSpecified=true
		fi
	done

	# Did the user use more than one sorting argument ?
	for sort_argument in "${sort_arguments[@]}"
	do
		if [[ " ${sort_argument} " == " ${argument} " ]]; then
			if [[ "$IsOneSortArgumentSpecified" = true ]]; then
				echo "Only one sorting method can be specified."
				exit 1
			fi
			IsOneSortArgumentSpecified=true
		fi
	done
done

if [[ ! "$IsFileArgumentSpecified" = true ]]; then
	echo "Missing -f argument."
	exit 1
fi
if [[ ! "$IsOneDataArgumentSpecified" = true ]]; then
	echo "No valid data type specified."
	exit 1
fi