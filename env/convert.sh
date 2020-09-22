if [ $# != 3 ]; then echo "convert.sh input_file header_file footer_file"; exit 1; fi
if [ ! -e $1 ]; then echo $1 not exists; exit 1; fi
if [ ! -e $2 ]; then echo $2 not exists; exit 1; fi
if [ ! -e $3 ]; then echo $3 not exists; exit 1; fi
INPUT_FILE=$1
HEADER_FILE=$2
FOOTER_FILE=$3

cat $HEADER_FILE

IFS=''
cat $INPUT_FILE | while read line
do
  if [[ ${line} =~ ^global.* ]]; then echo -n ";"; fi
  if [[ ${line} =~ ^SECTION.*rdata.* ]]; then exit 0; fi
  echo $line
done

cat $FOOTER_FILE

