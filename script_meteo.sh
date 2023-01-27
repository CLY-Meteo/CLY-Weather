#!/bin/bash

# ------------ Dev Settings ------------
declare -r Debug=true
declare -r WorkPath="/tmp/cly-meteo/"

# SHOULD BE OFF ON RELEASE
declare -r DisableCleaning=true
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
IsDateArgumentSpecified=false
UsedLocationArgument=""
UsedSortArgument=""
UsedDataArguments=()
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
			# We check that it's not already specified.
			for used_data_argument in "${UsedDataArguments[@]}"
			do
				if [[ " ${used_data_argument} " == " ${argument} " ]]; then
					echo "A data argument can only be specified once."
					exit 1
				fi
			done
			UsedDataArguments=( "${UsedDataArguments[@]}" "$argument" )
			continue
		fi
	done

	# Did the user use more than one location argument ?
	for location_argument in "${location_arguments[@]}"
	do
		if [[ " ${location_argument} " == " ${argument} " ]]; then
			if [[ ! "$UsedLocationArgument" = "" ]]; then
				echo "Only one location argument can be specified."
				exit 1
			fi
			UsedLocationArgument=$argument
			continue
		fi
	done

	# Did the user use more than one sorting argument ?
	for sort_argument in "${sort_arguments[@]}"
	do
		if [[ " ${sort_argument} " == " ${argument} " ]]; then
			if [[ ! "$UsedSortArgument" = "" ]]; then
				echo "Only one sorting method can be specified."
				exit 1
			fi
			UsedSortArgument=$argument
			continue
		fi
	done

	# Did the user use the --help argument ?
	if [[ " ${help_argument} " == " ${argument} " ]]; then
		source "$ScriptDirectory/help.sh"
		exit 0
	fi
done

# Post-argument checks
if [[ "$UsedDataArguments" = "" ]]; then
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

if [[ ! $(type -P gnuplot) ]]; then
	echo "gnuplot is not installed. Can't proceed."
	exit 4
fi

# -------------------------------------- File Checks --------------------------------------
if ! [ -e $FilePath -a -r $FilePath ]; then
	echo "The file $FilePath does not exist or is not readable."
	exit 1
fi


















# ------------------------------------------------------------------------------------------
# We get the file in the path and copy it to the work location. After creating it, if necessary.
# Depending on the distro, this could be in the RAM and take a lot of memory. Thus, IT IS IMPORTANT TO CLEAN WHEN WE'RE DONE.
mkdir -p $WorkPath
echo "Hold on..."
cp -f "$FilePath" "${WorkPath}data.csv"

# We remove the first line
sed -i '1d' "${WorkPath}data.csv"

# --------------------------------------- LOCATION FILTERING --------------------------------
# I'll have to remember to check if the coordinates are correct.
# Something was off with the file though. It didn't have the empty IDs.

if [[ ! "$UsedLocationArgument" = "" ]]; then
	echo "Filtering locations..."
	
	case $UsedLocationArgument in
		"-F")
		#France Métropolitaine + Corse
		MinLat=41 ; MaxLat=51 ; MinLong=-6 ; MaxLong=10
		;;

		"-G")
		# Guyane Française
		MinLat=-3 ; MaxLat=5 ; MinLong=-54 ; MaxLong=-51
		;;

		"-S")
		# Saint-Pierre et Miquelon
		MinLat=46 ; MaxLat=47 ; MinLong=-56 ; MaxLong=-55
		;;

		"-A")
		# Antilles
		MinLat=15 ; MaxLat=25 ; MinLong=-89 ; MaxLong=-60
		;;

		"-O")
		# Océan Indien
		MinLat=-50 ; MaxLat=40 ; MinLong=20 ; MaxLong=180
		;;

		"-Q")
		# Antarctique
		MinLat=-90 ; MaxLat=-60 ; MinLong=-180 ; MaxLong=180
		;;

		"*")
		echo "This shouldn't be happening. Dropping info that could be useful for debugging :"
		echo "Value of UsedLocationArgument : ${UsedLocationArgument}"
		exit 4 # You'd think this would be a user error, so a 1, but there's tests that should prevent this from happening. So, it's a bug.
		;;
	esac

	# Code provided by ChatGPT
	awk -i inplace -v MinLat="$MinLat" -v MaxLat="$MaxLat" -v MinLong="$MinLong" -v MaxLong="$MaxLong" -F ';' '{split($10,a,","); if((a[1]>MinLat && a[1]<MaxLat) && (a[2]>MinLong && a[2]<MaxLong)) {print $0}}' "${WorkPath}data.csv"
fi

# --------------------------------------- DATE FILTERING --------------------------------
if [[ "$IsDateArgumentSpecified" = true ]]; then
	echo "Filtering dates..."

	# ChatGPT
	awk -i inplace -v MinDate="$MinDate" -v MaxDate="$MaxDate" -F ";" '$2 >= MinDate && $2 <= MaxDate {print $0}' "${WorkPath}data.csv"
fi















# --------------------------------------- DATA FILTERING --------------------------------
# SOME INFOS GENERATED ARE NOT USABLE FOR GNUPLOT.
# NEEDS OUTPUT MODIFICATION.
for i in $UsedDataArguments; do
	case $i in
		"-h")
		# Altitude
		echo "Filtering according to altitude per station..."

		# Code provided by ChatGPT
		awk -F ";" '!seen[$1]++ {print $14 ";" $1 ";" $10}' "${WorkPath}data.csv" > "${WorkPath}altitude_filtered_data_unsorted.csv"
		;;

		"-m")
		# Humidity
		echo "Filtering according to maximum humidity per station..."

		# Code provided by ChatGPT
		awk -F ";" '{if ($1 in max_humidity) { if ($6 > max_humidity[$1]) {max_humidity[$1] = $6} } else {max_humidity[$1] = $6}} END {for (id in max_humidity) { print id ";" max_humidity[id]}}' "${WorkPath}data.csv" > "${WorkPath}humidity_filtered_data_unsorted.csv"
		;;






		"-t1")
		# Station ID; Minimum Temperature; Mean Temperature; Maximum Temperature
		echo "Filtering using t1..."

		# Code provided by ChatGPT
		awk -F ";" '{
			sum[$1]+=$11;
			count[$1]++;
			if(min[$1]>$11 || !min[$1]) min[$1]=$11;
			if(max[$1]<$11 || !max[$1]) max[$1]=$11;
		}
		END {
			for (i in sum) {
				print i ";" min[i] ";" sum[i]/count[i] ";" max[i]
			}
		}' "${WorkPath}data.csv" > "${WorkPath}t1_filtered_data_unsorted.csv"
		;;

		"-p1")
		# Station ID; Minimum Pressure; Mean Pressure; Maximum Pressure
		echo "Filtering using p1..."

		# Code provided by ChatGPT
		awk -F ";" '{
			sum[$1]+=$6;
			count[$1]++;
			if(min[$1]>$6 || !min[$1]) min[$1]=$6;
			if(max[$1]<$6 || !max[$1]) max[$1]=$6;
		}
		END {
			for (i in sum) {
				print i ";" min[i] ";" sum[i]/count[i] ";" max[i]
			}
		}' "${WorkPath}data.csv" > "${WorkPath}p1_filtered_data_unsorted.csv"
		;;



		"-t2")
		echo "Filtering using t2..."
		;;

		"-p2")
		echo "Filtering using p2..."
		;;




		"-t3")
		echo "Filtering using t3..."
		;;

		"-p3")
		echo "Filtering using p3..."
		;;

		"*")
		echo "This shouldn't be happening. Dropping info that could be useful for debugging :"
		echo "Value of i : ${i}"
		exit 4
		;;
	esac
done








# --------------------------- Final cleaning -----------------
if [[ $DisableCleaning = false ]]; then
	rm -rf /tmp/cly-meteo
fi

exit 0
