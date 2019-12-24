#!/bin/bash

dir="/wd/dune-it/enurec/prod/data/ghep"

if [ "${1}" != "" ]
then
   dir="${1}"
fi

files="$(ls -lrt ${dir} | tail -n +2 | awk -F' ' '{print $9}')"

dscript="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

script="${dscript}/convert.sh"

for f in ${files}
do 
   source ${script} ${dir}/${f}
done
