#!/bin/sh

EXPECTED_ARGS=5

if [ $# -ne $EXPECTED_ARGS ]
then
  echo "Usage: `runme.sh working_dir input.fasta V.fasta D.fasta J.fasta` "
  echo "If you do not have any of V D J fasta files, just write path to non-existing file, utility will handle it"
  exit
fi

WORKDIR=$1
INPUT=$2
VGENE=$3
DGENE=$4
JGENE=$5

echo "Working directory: $WORKDIR"
echo "Input fasta file: $INPUT"
echo "V genes file: $VGENE"
echo "D genes file: $DGENE"
echo "J genes file: $JGENE"

echo "Creating config for V genes"
cat ./regions/src/config/config.template > ./regions/bin/config/config.ini
echo "reference_file=$VGENE" >> ./regions/bin/config/config.ini
echo "input_file=$INPUT" >> ./regions/bin/config/config.ini
echo "output_align=${WORKDIR}/v_align" >> ./regions/bin/config/config.ini
echo "output_regions=${WORKDIR}/v_regions" >> ./regions/bin/config/config.ini
echo "Done"

echo "Run ./regions for V gene"
cd ./regions/bin
./regions
cd ../../
echo "Done"


echo "Creating config for D genes"
cat ./regions/src/config/config.template > ./regions/bin/config/config.ini
echo "reference_file=$DGENE" >> ./regions/bin/config/config.ini
echo "input_file=$INPUT" >> ./regions/bin/config/config.ini
echo "output_align=${WORKDIR}/d_align" >> ./regions/bin/config/config.ini
echo "output_regions=${WORKDIR}/d_regions" >> ./regions/bin/config/config.ini
echo "Done"

echo "Run ./regions for D gene"
cd ./regions/bin
./regions
cd ../../
echo "Done"

echo "Creating config for J genes"
cat ./regions/src/config/config.template > ./regions/bin/config/config.ini
echo "reference_file=$JGENE" >> ./regions/bin/config/config.ini
echo "input_file=$INPUT" >> ./regions/bin/config/config.ini
echo "output_align=${WORKDIR}/j_align" >> ./regions/bin/config/config.ini
echo "output_regions=${WORKDIR}/j_regions" >> ./regions/bin/config/config.ini
echo "Done"

echo "Run ./regions for J gene"
cd ./regions/bin
./regions
cd ../../
echo "Done"

echo "Prepare to run ./regions_assembler for collected data"
echo "[data]" > ./regions_assembler/bin/config/config.ini 
echo "output_file=${WORKDIR}/result.txt" >> ./regions_assembler/bin/config/config.ini 
echo "v_data_file=${WORKDIR}/v_regions" >> ./regions_assembler/bin/config/config.ini 
echo "d_data_file=${WORKDIR}/d_regions" >> ./regions_assembler/bin/config/config.ini 
echo "j_data_file=${WORKDIR}/j_regions" >> ./regions_assembler/bin/config/config.ini 
cd ./regions_assembler/bin

echo "Start ./regions_assembler"
./regions_assembler
echo "Done. You're results are saved to ${WORKDIR}/result.txt"


