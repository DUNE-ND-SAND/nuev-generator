#!/bin/bash

edepsim="edep-sim"
root="root"
macrodir="/wd/dune-it/enurec/prod/macros"
template="macro.mac.template"
macrolog="macro.log"

if [ "${1}" == "" ] || [ "${2}" == "" ]
then
   echo "input file or geoemtry file missing."
   return 1
fi 

input=${1}
geo=${2}


if [ ! -f ${input} ]
then
   echo "file ${input} does not exist."
   return 1
fi

if [ ! -f ${geo} ]
then
   echo "file ${geo} does not exist."
   return 1
fi

res=$(which ${edepsim} 2> /dev/null)

if [ "${res}" == "" ] 
then
	 echo "${edepsim} not exist. Please set the right environment."
   return 1
fi

res=$(which ${root} 2> /dev/null)

if [ "${res}" == "" ] 
then
	 echo "${root} not exist. Please set the right environment."
   return 1
fi

iname=$(basename ${input})
idir=$(dirname ${input})

oname=$(echo ${iname} | sed "s:rootracker:edep-sim:g")

if [ "${idir}" == "." ]
then
   odir="../edep-sim"
else
   odir=$(echo ${idir} | sed "s/rootracker/edep-sim/g")
fi

if [ ! -d ${odir} ]
then
	 echo "output directory ${odir} not exist. Please create it."
   return 1
fi

output="${odir}/${oname}"
edepsimlogname=$(echo ${oname} | sed "s:root:log:g")
edepsimlog="${macrodir}/logs/${edepsimlogname}"

macroname=$(echo ${oname} | sed "s:root:mac:")
macro="${macrodir}/${macroname}"

nev=$(echo "((TTree*)_file0->Get(\"gRooTracker\"))->GetEntries()" | root -l ${input} | tail -n 1 | awk -F' ' '{print $3}')

sed "s:__INPUT__:${input}:;s:__NEV__:${nev}:" ${macrodir}/${template} > ${macro} 

command="${edepsim} -g ${geo} -o ${output} ${macro}"

echo "${command}" >> ${macrodir}/${macrolog}
echo -n "processing ${iname} -> ${oname}: "

#pushd ${macrodir} &> /dev/null
${command}  &> ${edepsimlog}
#popd &> /dev/null

if [ -f "${output}" ]
then 
   echo "OK"
else
   echo "NO"
fi
