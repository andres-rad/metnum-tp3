input_csv=$1
output_csv=$2
magnitud_discretizacion=$3
varianza_ruido=$4
if [[ -e $1 ]] ; then
    make clean -C src; make -C src
    echo "Corriendo tomografía..."
    ./src/main -i $input_csv -o $output_csv -d $magnitud_discretizacion -v $varianza_ruido
    python csv_visualizer.py $2
else
    echo "Archivo de input inexistente."
fi
