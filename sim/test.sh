#!/bin/bash

#The script will simulate a given input file under all the implemented schedulers and all possible CPU counts (until a given maximum CPU count)
#Create a shell script that does not accept arguments, but instead asks the user to enter the following information interactively:
#– Theinputfilenametobesimulated.Thescriptwillverifythatthefileexistsandisregular. If that is not the case, the script will display an error message and will ask the user to enter a file name again.

#Then, a results directory will be created and the simulator will be executed for each available scheduling algorithm and for each CPU count from 1 to the maximum CPU count provided by the used. The script will ensure that all simulation results are safely backed up (without overwriting files) in the results directory. Finally, for each file as- sociated with the various simulations, a different chart will be generated. The charts will be stored in the results directory as well.
#A possible pseudocode representation of the core processing of the script is as follows:


directorio="Resultados"

# Preguntamos por el filename

filename=""
valid=false

while [ "$valid" != "true" ]
do
	echo "(?) Introduce el nombre del archivo (Sin extension): "
	read auxFilename

	filename=("./examples/"$auxFilename".txt")
	
	#Comprobamos si existe
	if [ -f "$filename" ];
	then
	valid=true
	else
		echo "$filename not found."
	fi
done

# Creamos el directorio de los resultados
# Si ya existiera, lo reemplazamos

if [-d "$directorio" ]; then
	echo "Directorio ya creado. Accediendo..."
else
	mkdir $directorio # Crea el directorio vacio
fi



# simulator will be executed for each available scheduling algorithm.
# and for each CPU count from 1 to the maximum CPU count provided by the used. 
 
array2=()
i=0
for line in $(./schedsim -L); do
	if (( i >= 2 )); then
		cpus=1
		algorithmName=$line

		echo "=========================================================="
		echo "Executing $maxCPUs times $algorithmName algorithm"
		echo "=========================================================="

		while (( cpus <= maxCPUs )); do
			./schedsim -n $cpus -i $filename
			echo "---"
			((cpus+=1))
		done

		# Backup files. Moving to other directory and with a special name
		# Filename will be: $algorithmName"_"$cpus".log"
		for f_name in *.log; do 
			auxFilename=($directory"/"$algorithmName"__"$f_name)
			mv $f_name $auxFilename
 
			# Generate Chart generate chart
			cd "../gantt-gplot/"
			./generate_gantt_chart "../schedsim/"$auxFilename
			cd "../schedsim/"
		done

	else
		# This line is header. Not the algorithm name
		# print on screen
		#echo $line
		echo ""
	fi
	((i+=1))
done

exit 0 
 
#foreach nameSched in listOfAvailableSchedulers
#do
#    for cpus = 1 to maxCPUs
#	do
#	  ./sched-sim -n cpus -i .......
#	for i=1 to cpus
#	do
#		move CPU_$i.log to  results/nameSched-CPU-$i.log
#	done
#	generate chart
#done done

