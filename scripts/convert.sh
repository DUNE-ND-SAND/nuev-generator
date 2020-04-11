#!/bin/bash

command="gntpc"

res=$(which ${command} 2> /dev/null)

if [ "${res}" == "" ] 
then
	 echo -e "\e[5m\e[91mERROR\e[0m: ${command} does not exist. Please set the right environment."
   return 1
fi

input=${1}

if [ "${input}" == "" ]
then
   echo -e "\e[5m\e[91mERROR\e[0m: input file missing."
   echo "source ${BASH_SOURCE[0]} <input file>"
   return 1
elif [ ! -f "${input}" ]
then
   echo -e "\e[5m\e[91mERROR\e[0m: file ${input} does not exist."
   return 1
fi

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

if [ -f "${output}" ]
then
   echo -e "\e[5m\e[91mERROR\e[0m: output file ${output} exists. Remove it to re-process."
   return 1
fi    

${command} -i ${input} -o ${output} -f t2k_rootracker -c &> /dev/null

if [ "${?}" == "0" ]
then
   echo "file: ${output}: OK"
else
   echo "file: ${output}: NO"
fi
