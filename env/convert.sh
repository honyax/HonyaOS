if [ $# != 1 ]; then echo "convert.sh input_file"; exit 1; fi
if [ ! -e $1 ]; then echo $1 not exists; exit 1; fi
INPUT_FILE=$1

echo %include \"define.s\"
echo ORG KERNEL_LOAD
echo [BITS 32]
echo

cat $INPUT_FILE | while read line
do
  if [[ ${line} =~ ^global.* ]]; then echo -n ";"; fi
  if [[ ${line} =~ ^SECTION.*rdata.* ]]; then exit 0; fi
  echo $line
done

echo SECTION .text
echo times KERNEL_SIZE - \(\$ - \$\$\) db 0

