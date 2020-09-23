if [ $# != 2 ]; then echo "convert.sh input_file template_file"; exit 1; fi
if [ ! -e $1 ]; then echo $1 not exists; exit 1; fi
if [ ! -e $2 ]; then echo $2 not exists; exit 1; fi
INPUT_FILE=$1
TEMPLATE_FILE=$2

IFS=''

# テンプレートファイルを出力
cat $TEMPLATE_FILE | while read template_line
do

  # テンプレートファイルの中の {KERNEL_SOURCECODE} の部分を入力ファイルで置き換え
  if [[ ${template_line} =~ ^\{KERNEL_SOURCECODE\} ]]; then
    echo
    echo "; REPLACE {KERNEL_SOURCECODE}"
    echo
    
    cat $INPUT_FILE | while read line
    do
      if [[ ${line} =~ ^global.* ]]; then echo -n ";"; fi
      if [[ ${line} =~ ^SECTION.*rdata.* ]]; then exit 0; fi
      line=${line//noexecute/}
      line=${line//execute/}
      echo ${line}
    done
  else
    echo ${template_line}
  fi

done


