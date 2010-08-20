inputDir=$1
for file in $inputDir/*.lua; do
  echo $file
  sed '/ TimeSequence/{:z;N;/),/!bz;/Missing/d}' $file > tmp.txt
  cat tmp.txt > $file
  rm tmp.txt
done