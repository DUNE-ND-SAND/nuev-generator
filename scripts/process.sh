#!/bin/bash

if [ "${1}" == "" ] || [ "${2}" == "" ]
then
   echo -e "\e[5m\e[91mERROR\e[0m: input file or geoemtry file missing."
   echo "source ${BASH_SOURCE[0]} <input file> <geometry file>"
   return 1
fi 

script="$(pwd)/${BASH_SOURCE[0]}"
input=${1}
geo=${2}

sdir=$(dirname ${script})

idir=$(dirname ${input})
iname=$(basename ${input})

if [ "${idir}" == "." ]
then
   odir="../rootracker"
else
   odir=$(echo ${idir} | sed "s/ghep/rootracker/g")
fi

oname=$(echo ${iname} | sed "s/ghep/rootracker/g")

output="${odir}/${oname}"

source ${sdir}/convert.sh ${input}
source ${sdir}/propagate.sh ${output} ${geo}