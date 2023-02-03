*Roses are red, violets are blue, [Nouha](https://github.com/ouhaa)'s  writing the documentation and [Jordan](https://github.com/JordanViknar)'s in tears.*

---

# CLY-Weather *(CLI + CY-Météo)*

![License](https://img.shields.io/github/license/CLY-Meteo/CLY-Weather?color=orange)
![Top language](https://img.shields.io/github/languages/top/CLY-Meteo/CLY-Weather?color=grey)
![Commit activity](https://img.shields.io/github/commit-activity/m/CLY-Meteo/CLY-Weather?color=red)
![Repo size](https://img.shields.io/github/repo-size/CLY-Meteo/CLY-Weather)

CLY-Weather (CLI + CY-Météo) is a Bash script + C program that translates weather data into various graphs, using gnuplot.

# Summary

- [Using CLY-Weather](#How-to-use-CLY-Weather-)
	- [Dependencies](#Dependencies)
	- [Basic syntax](#Basic-syntax-)
- [Compiling CLY-Weather](#How-to-compile-CLY-Weather-s-C-program-)

# How to use CLY-Weather ?

## Dependencies

### Compilation
- gcc
- make
### Runtime
- awk
- gnuplot

## Basic syntax
You can use CLY-Weather through this command, from the root folder of the project. 
```bash
./cly-weather.sh -f SOURCE_FILE DATA_TYPE [OPTIONS]
```

The expected source file is a CSV file, containing weather data organized like this :
```
Station ID;Date;Sea level pressure;Average wind direction 10 nm;Average wind speed 10 nm;Humidity;Station pressure;Pressure variation in 24 hours;Precipitation in the last 24 hours;Coordinates;Temperature (°C );Minimum temperature over 24 hours (°C);Maximum temperature over 24 hours (°C);Altitude;municipalities (code)
```
In order to successfully run, CLY-Weather needs at least one data type argument. If all goes well, you should be awarded with as many graphs as data types entered as arguments : one graph per argument.

## Data Type options 
### Temperature : (-t1, -t2, -t3)
	-t1 : Produces a graph of the minimum, average, and maximum temperature for each station.
	-t2 : Produces a graph of the average temperature per day/hour on every station.
	-t3 : Produces a graph of the temperatures per day/hour for each station.

### Pressure : (-p1, -p2, -p3)
	-p1 : Produces a graph of the minimum, average, and maximum pressure for each station.
	-p2 : Produces a graph of the average pressure per day/hour on every station.
	-p3 : Produces a graph of the pressures per day/hour for each station.

### Wind : (-w)
	-w : Produces a vector map of the average wind orientation and speed for each station.

### Altitude : (-h)
	-h : Produces a map of the altitude for each station.

### Humidity : (-m)
	-m : Produces a map of the maximum humidity for each station.

## Location options
*Note : Only one location can be specified at a time.*

	-F : France & Corse
	-G : Guyane
	-S : Saint-Pierre et Miquelon
	-A : Antilles
	-O : Océan indien
	-Q : Antarctique

	-g <min> <max> : Longitude filtering. Can be paired with -a.
	-a <min> <max> : Latitude filtering. Can be paired with -g.

## Date Filtering
*Note : Only one date interval can be specified at a time.*

	-d <start> <end> : Date filtering. Expects the format YYYY-MM-DD.

## Sorting algorithm
*Note : Only one sorting algorithm can be specified at a time.*

	--avl : Sorts the data using an AVL tree. (Default)
	--abr : Sorts the data using an ABR tree.
	--tab : Sorts the data using a table.

Additionally, the ```--help``` argument will displays the program's help page.

# How to **compile** CLY-Weather's C program ?

**From the root folder of the project...**

- Open a terminal, and move to the c_source folder.
- Run ```make```
- Enjoy.

*(If you want cly-weather.sh to use your compiled binary, move the binary back to the root folder of the project.)*
