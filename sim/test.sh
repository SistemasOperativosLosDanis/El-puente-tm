#!/bin/bash

directorio=Resultados

# Preguntamos por el filename

filename=""
valid=false

while [ "$valid" != "true" ]
do
	echo "Introduce el nombre del archivo (Sin extension): "
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
if [ -e "$directorio" ]; then
	echo "Directorio ya creado. Accediendo..."
else
	mkdir $directorio # Crea el directorio vacio
fi



# Encendemos el simulador
# Generate Chart
	cd "../gantt/"
	./generate_gantt_chart "../sim/"$filename
	cd "../sim/"

exit 0 
 


