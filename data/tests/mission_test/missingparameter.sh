inputDir=$1
for file in $inputDir/*.lua; do
  echo $file
  sed '/ TimeSequence/{:z;N;/end),/!bz;/Missing/d}' $file > tmp.txt
  cat tmp.txt > $file
  rm tmp.txt
  nbmission=`grep Mission.create $file | wc -l`
  if (( `expr $nbmission = 0` ))
  then
    echo "-->" $file ": no mission to launch"
    rm $file
  fi
done
echo `grep -r "Mission.create(" $inputDir | wc -l` "missions to be tested."
