#!/bin/bash

# ------------ Dev Settings ------------
declare -r Debug=true
declare -r WorkPath="/tmp/cly-meteo/"

# SHOULD BE OFF ON RELEASE
declare -r DisableCleaning=true
declare -r UseOldLocationFilter=true
declare -r AlwaysRebuild=true

# ------------ Constants ------------
declare -r argument_count=$#
declare -r arguments=$@
declare -r ScriptDirectory=$(dirname "$0")

declare -r data_type_arguments=("-t1" "-t2" "-t3" "-p1" "-p2" "-p3" "-w" "-h" "-m") #Required, not exclusive
declare -r location_arguments=("-F" "-G" "-S" "-A" "-O" "-Q") #Exclusive between each other
declare -r date_argument="-d" #Optional
declare -r sort_arguments=("--tab" "--abr" "--avl") #Exclusive between each other
declare -r file_argument="-f" #Required
declare -r help_argument="--help" #Optional

declare -r all_arguments=("${data_type_arguments[@]}" "${location_arguments[@]}" "${sort_arguments[@]}" "${date_argument}" "${file_argument}" "${help_argument}")

# ----------- Variables -------------
IsFileArgumentSpecified=false
AssumeNextCanBePath=false
IsOneDataArgumentSpecified=false
IsDateArgumentSpecified=false
IsOneDataArgumentSpecified=false
LocationArgument=""
SortArgument=""
ShouldNextArgumentBeFilePath=false
ShouldNextArgumentBeDate=0
FilePath=""
MinDate=""
MaxDate=""

# -------------------------------------------------------------------------------------------
# This part of the code detects if the arguments entered by the user are valid.
for argument in $arguments
do
	if [[ "$ShouldNextArgumentBeFilePath" = true ]]; then
		FilePath=$argument
		ShouldNextArgumentBeFilePath=false

		AssumeNextCanBePath=true
		# We move onto the next arguments
		continue
	fi

	if [[ "$ShouldNextArgumentBeDate" = 1 ]]; then
		# My brain overheated there...
		if ! grep -qE '^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$' <<< "$argument"; then
			echo "Invalid date format : ${argument}"
			exit 1
		fi

		MinDate=$argument
		ShouldNextArgumentBeDate=2

		# We move onto the next arguments
		continue
	elif [[ "$ShouldNextArgumentBeDate" = 2 ]]; then
		if ! grep -qE '^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$' <<< "$argument"; then
			echo "Invalid date format : ${argument}"
			exit 1
		fi
		
		MaxDate=$argument
		ShouldNextArgumentBeDate=0

		# We move onto the next arguments
		continue
	fi

	# Did the user enter any invalid argument ?
	if [[ ! " ${all_arguments[@]} " =~ " ${argument} " ]]; then
		#Allows for paths with spaces to work
		if [[ "$AssumeNextCanBePath" == true ]]; then
			FilePath="$FilePath $argument"
			continue
		fi
		echo "Invalid argument : ${argument}"
		exit 1
	fi
	AssumeNextCanBePath=false

	# Did the user use the -f argument ?
	if [[ " ${file_argument} " == " ${argument} " ]]; then
		if [[ "$IsFileArgumentSpecified" = true ]]; then
			echo "Only one file argument can be specified."
			exit 1
		fi
		IsFileArgumentSpecified=true
		ShouldNextArgumentBeFilePath=true
	fi

	# Did the user use the -d argument ?
	if [[ " ${date_argument} " == " ${argument} " ]]; then
		if [[ "$IsDateArgumentSpecified" = true ]]; then
			echo "Only one date argument can be specified."
			exit 1
		fi
		IsDateArgumentSpecified=true
		ShouldNextArgumentBeDate=1
	fi

	# Did the user use one of the data type arguments ?
	for data_type_argument in "${data_type_arguments[@]}"
	do
		if [[ " ${data_type_argument} " == " ${argument} " ]]; then
			IsOneDataArgumentSpecified=true
			continue
		fi
	done

	# Did the user use more than one location argument ?
	for location_argument in "${location_arguments[@]}"
	do
		if [[ " ${location_argument} " == " ${argument} " ]]; then
			if [[ ! "$IsOneLocationArgumentSpecified" = "" ]]; then
				echo "Only one location argument can be specified."
				exit 1
			fi
			LocationArgument=$argument
			continue
		fi
	done

	# Did the user use more than one sorting argument ?
	for sort_argument in "${sort_arguments[@]}"
	do
		if [[ " ${sort_argument} " == " ${argument} " ]]; then
			if [[ ! "$SortArgument" = "" ]]; then
				echo "Only one sorting method can be specified."
				exit 1
			fi
			SortArgument=$argument
			continue
		fi
	done
done

# Post-argument checks
if [[ ! "$IsOneDataArgumentSpecified" = true ]]; then
	echo "No valid data type specified."
	exit 1
fi

# File checks
if [[ ! "$IsFileArgumentSpecified" = true ]]; then
	echo "Missing -f argument."
	exit 1
fi
if [[ "$FilePath" = "" ]]; then
	echo "-f was called, but no file was given."
	exit 1
fi

# Date checks
if [[ "$IsDateArgumentSpecified" = true ]]; then
	if [[ "$MinDate" = "" || "$MaxDate" = "" ]]; then
		echo "Error : -d was called, but the dates are missing/incomplete."
		exit 1
	fi
fi

# ------------------------ Check for dependencies ------------------------
if [[ "$AlwaysRebuild" = true ]]; then
	rm -f "$ScriptDirectory/cly-meteo-sorting"
fi

# We need to see if the executable cly-meteo-sorting exists at the root.
if [[ ! -f "$ScriptDirectory/cly-meteo-sorting" ]]; then
	echo "WARNING : cly-meteo-sorting is missing !"
	# We check if gcc and make are installed.
	if [[ ! $(type -P gcc) ]]; then
		echo "gcc is not installed. Can't proceed."
		exit 4
	fi
	if [[ ! $(type -P make) ]]; then
		echo "make is not installed. Can't proceed."
		exit 4
	fi

	backup_pwd=$(pwd)
	cd "$ScriptDirectory/c_source"

	echo "Compiling cly-meteo-sorting with make..."
	echo "------------ Make output ------------"
	make
	if [[ ! $? = 0 ]]; then
		echo "------------ End of make output ------------"
		echo "cly-meteo couldn't compile the executable. Exiting..."
		exit 4
	fi
	echo "------------ End of make output ------------"

	mv cly-meteo-sorting ../cly-meteo-sorting
	cd "$backup_pwd"
fi

# [IMPLEMENT CHECKS THAT THE FILE SPECIFIED IS USABLE (exists, can be opened, is formatted properly, etc...)]

# ------------------------------------------------------------------------------------------
# We get the file in the path and copy it to the work location. After creating it, if necessary.
# Depending on the distro, this could be in the RAM and take a lot of memory. Thus, IT IS IMPORTANT TO CLEAN WHEN WE'RE DONE.
mkdir -p $WorkPath
echo "Hold on..."
cp -f "$FilePath" "${WorkPath}data.csv"

# We cut the first line in the csv, we don't need it.


# --------------------------------------- LOCATION FILTERING --------------------------------
# [WARNING] EXCLUDES LOCATIONS THAT DON'T HAVE A POSTAL CODE.
if [[ ! "$LocationArgument" = "" ]]; then
	if [[ $UseOldLocationFilter = false ]]; then
		echo "Placeholder"
		exit 1
	else
		case $LocationArgument in
			"-F")
			#France Métropolitaine + Corse
			declare -r LocationList="07"
			;;

			"-G")
			# Guyane Française
			declare -r LocationList="81"
			;;

			"-S")
			# Saint-Pierre et Miquelon
			declare -r LocationList="71"
			;;

			"-A")
			# Antilles
			declare -r LocationList="78"
			;;

			"-O")
			# Océan Indien
			declare -r LocationList="67|61"
			;;

			"-Q")
			# Antarctique
			declare -r LocationList="89"
			;;

			"*")
			echo "This shouldn't be happening. Dropping info that could be useful for debugging :"
			echo "Value of LocationArgument : ${LocationArgument}"
			exit 1
			;;
		esac

		# We overwrite the temporary file.
		echo "Filtering locations..."
		grep -E "^(${LocationList})" "${WorkPath}data.csv" > "${WorkPath}filtered_data.csv"

		rm "${WorkPath}data.csv"
		mv "${WorkPath}filtered_data.csv" "${WorkPath}data.csv"
	fi
fi

# --------------------------------------- DATE FILTERING --------------------------------
# [WARNING] CURRENTLY BROKEN
if [[ "$IsDateArgumentSpecified" = true ]]; then
	echo "Filtering dates..."
	# This doesn't work ?
	awk -F ';' "\$2 >= $MinDate && \$2 <= $MaxDate" "${WorkPath}data.csv" > "${WorkPath}filtered_data.csv"

	# Again, we overwrite it.
	rm "${WorkPath}data.csv"
	mv "${WorkPath}filtered_data.csv" "${WorkPath}data.csv"
fi

# --------------------------- Final cleaning -----------------
if [[ $DisableCleaning = false ]]; then
	rm -rf /tmp/cly-meteo
fi

exit 0
