nbfile=`find ../../exercises/testings -name '*Dispatcher.log' | wc -l`

function getfile
{
    find ../../exercises/testings -name '*Dispatcher.log' | echo `awk -v l=$i 'NR==l{ print }'`
}

rm -f ./reports/${1}_Dispatchers.log
touch ./reports/${1}_Dispatchers.log

for i in `seq 1 $nbfile`
do
    file=`getfile $i`
    echo  >> ./reports/${1}_Dispatchers.log
    echo "--------------------------------------------------------------------------------" >> ./reports/${1}_Dispatchers.log
    echo $file >> ./reports/${1}_Dispatchers.log
    echo "--------------------------------------------------------------------------------" >> ./reports/${1}_Dispatchers.log
    cat $file >> ./reports/${1}_Dispatchers.log
    echo  >> ./reports/${1}_Dispatchers.log
    echo "Resume:" >> ./reports/${1}_Dispatchers.log
    echo "------" >> ./reports/${1}_Dispatchers.log
    echo -n `grep Error $file | wc -l` >> ./reports/${1}_Dispatchers.log
    echo " errors" >> ./reports/${1}_Dispatchers.log
    echo -n `grep "agent creation" $file | wc -l` >> ./reports/${1}_Dispatchers.log
    echo " agent(s) created" >> ./reports/${1}_Dispatchers.log
    echo -n `grep " : running" $file | wc -l` >> ./reports/${1}_Dispatchers.log
    echo " mission(s) launched" >> ./reports/${1}_Dispatchers.log
done


