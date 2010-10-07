scripts_dir=$1

nbfile=`find $scripts_dir -name '*.sh' | wc -l`

for i in `seq 1 $nbfile`
do 
    filename=`find $scripts_dir -name '*.sh' | sed $i{D}`
    echo "executing" $filename"..."
    $filename
done