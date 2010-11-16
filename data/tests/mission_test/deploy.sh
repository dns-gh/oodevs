nbfile=`find -path './out/scripts/*' -prune -name '*.lua' | wc -l`
function getfile
{
    find -path './out/scripts/*' -prune -name '*.lua' | echo `awk -v l=$i 'NR==l{ print }'`
}

unitpersim=$1

exercicesdirectory='out/exercises/'
mkdir $exercicesdirectory

currentdir="1"

mkdir "${exercicesdirectory}testings$currentdir"
cp -r out/testings/* "${exercicesdirectory}testings$currentdir"
echo "creating exercise testings$currentdir ..."

for i in `seq 1 $nbfile`
do 
    if (( `expr $i % $unitpersim = 0` ))
    then
        currentdir=`expr $currentdir + 1`
        mkdir "${exercicesdirectory}testings$currentdir"
        echo "creating exercise testings$currentdir ..."
        cp -r out/testings/* "${exercicesdirectory}testings$currentdir"
        cp `getfile` "${exercicesdirectory}testings$currentdir/scripts"
    else
        cp `getfile` "${exercicesdirectory}testings$currentdir/scripts"
    fi
done

