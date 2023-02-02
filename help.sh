list="\
CLY-Meteo (CLI + CY-Meteo) is a Bash script + C program that sorts weather data into various graphs, using gnuplot.
Usage: ./cly-meteo.sh -f SOURCE_FILE DATA_TYPE [OPTIONS]

--- Data type options ---
Note : You can use multiple types at the same time.

Temperature : -t1, -t2, -t3
	-t1 : Produces a graph of the minimum, average, and maximum temperature for each station.
	-t2 : Produces a graph of the average temperature per day/hour on every station.
	-t3 : Produces a graph of the temperatures per day/hour for each station.

Pressure : -p1, -p2, -p3
	-p1 : Produces a graph of the minimum, average, and maximum pressure for each station.
	-p2 : Produces a graph of the average pressure per day/hour on every station.
	-p3 : Produces a graph of the pressures per day/hour for each station.

Wind :
	-w : Produces a vector map of the average wind orientation and speed for each station.

Altitude :
	-h : Produces a map of the altitude for each station.

Humidity :
	-m : Produces a map of the maximum humidity for each station.

-- Location options --
Note : Only one location can be specified at a time.
	-F : France & Corse
	-G : Guyane
	-S : Saint-Pierre et Miquelon
	-A : Antilles
	-O : Océan indien
	-Q : Antarctique

	-g <min> <max> : Longitude filtering. Can be paired with -a.
	-a <min> <max> : Latitude filtering. Can be paired with -g.

-- Date --
Note : Only one date interval can be specified at a time.
	-d <start> <end> : Date filtering. Expects the format YYYY-MM-DD.

-- Sorting algorithm --
Note : Only one sorting can be specified at a time.
	--avl : Sorts the data using an AVL tree. (Default)
	--abr : Sorts the data using an ABR tree.
	--tab : Sorts the data using a table.

--help : Displays this help page."

echo "$list"
