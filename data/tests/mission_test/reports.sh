nbfile=`find ../../exercises/testings -name '*Dispatcher.log' | wc -l`

rm -f ./reports/${1}_Dispatchers.log
touch ./reports/${1}_Dispatchers.log

totalErrors=0
totalUnits=0
totalMissions=0

for i in `seq 1 $nbfile`
do
    file=`find ../../exercises/testings/testings${i}/ -name '*Dispatcher.log'`

    currentErrors=`grep Error $file | wc -l`
    currentUnits=`grep -r "agent creation" $file | wc -l`
    currentMissions=`grep -r " : running" $file | wc -l`

    totalErrors=`expr $totalErrors + $currentErrors`
    totalUnits=`expr $totalUnits + $currentUnits`
    totalMissions=`expr $totalMissions + $currentMissions`

    echo  >> ./reports/${1}_Dispatchers.log
    echo "-------------------------------------------------------------------" >> ./reports/${1}_Dispatchers.log
    echo $file >> ./reports/${1}_Dispatchers.log
    echo "-------------------------------------------------------------------" >> ./reports/${1}_Dispatchers.log
    cat $file >> ./reports/${1}_Dispatchers.log
    echo  >> ./reports/${1}_Dispatchers.log
    echo "Resume:" >> ./reports/${1}_Dispatchers.log
    echo "------" >> ./reports/${1}_Dispatchers.log

    echo $currentErrors "errors" >> ./reports/${1}_Dispatchers.log

    echo -n $currentUnits "/" >> ./reports/${1}_Dispatchers.log 
    echo `grep -r "sim:CreateUnit(" ../../exercises/testings/testings${i}/scripts | wc -l` "agent(s) created" >> ./reports/${1}_Dispatchers.log 

    echo -n $currentMissions "/" >> ./reports/${1}_Dispatchers.log
    echo `grep -r "Mission.create(" ../../exercises/testings/testings${i}/scripts | wc -l` "mission(s) launched" >> ./reports/${1}_Dispatchers.log 

    echo "-------------------------------------------------------------------" >> ./reports/${1}_execution_report.log
    echo $file >> ./reports/${1}_execution_report.log

    echo $currentErrors "errors" >> ./reports/${1}_execution_report.log

    echo -n $currentUnits "/" >> ./reports/${1}_execution_report.log
    echo `grep -r "sim:CreateUnit(" ../../exercises/testings/testings${i}/scripts | wc -l` "agent(s) created" >> ./reports/${1}_execution_report.log

    echo -n $currentMissions "/" >> ./reports/${1}_execution_report.log
    echo `grep -r "Mission.create(" ../../exercises/testings/testings${i}/scripts | wc -l` "mission(s) launched" >> ./reports/${1}_execution_report.log
done

echo "-------------------------------------------------------------------" >> ./reports/${1}_execution_report.log
echo "Total:" >> ./reports/${1}_execution_report.log
echo "------" >> ./reports/${1}_execution_report.log
echo $totalErrors "errors" >> ./reports/${1}_execution_report.log
echo $totalUnits "agent(s) created" >> ./reports/${1}_execution_report.log
echo $totalMissions "mission(s) launched" >> ./reports/${1}_execution_report.log
cat ./reports/${1}_execution_report.log

