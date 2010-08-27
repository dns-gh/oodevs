totalErrors=0
totalUnits=0
totalMissions=0
totalUnitsToBeCreated=0
totalMissionsToBeLaunched=0

nbfile=`find ../../exercises/testings -name '*Dispatcher.log' | wc -l`

for i in `seq 1 $nbfile`
do
    file=`find ../../exercises/testings/testings${i}/ -name '*Dispatcher.log'`

    currentErrors=`grep Error $file | wc -l`
    currentUnits=`grep -r "agent creation" $file | wc -l`
    currentMissions=`grep -r " : running" $file | wc -l`
    currentUnitsToBeCreated=`grep -r "sim:CreateUnit(" ../../exercises/testings/testings${i}/scripts | wc -l`
    currentMissionsToBeLaunched=`grep -r "Mission.create(" ../../exercises/testings/testings${i}/scripts | wc -l`

    totalErrors=`expr $totalErrors + $currentErrors`
    totalUnits=`expr $totalUnits + $currentUnits`
    totalMissions=`expr $totalMissions + $currentMissions`
    totalUnitsToBeCreated=`expr $totalUnitsToBeCreated + $currentUnitsToBeCreated`
    totalMissionsToBeLaunched=`expr $totalMissionsToBeLaunched + $currentMissionsToBeLaunched`

    if (( `expr $file != ""` ))
    then
        cp $file ${1}/Dispatcher_${i}.log
        cp ../../exercises/testings/testings$i/sessions/default/Messages.log ${1}/Messages_${i}.log
        cp ../../exercises/testings/testings$i/sessions/default/Sim.log ${1}/Sim_${i}.log
        cp ../../exercises/testings/testings$i/sessions/default/actions.ord ${1}/actions_${i}.ord
        
        echo "-------------------------------------------------------------------" >> ${1}/execution_report.log
        echo $file >> ${1}/execution_report.log
        echo $currentErrors "errors" >> ${1}/execution_report.log
        echo $currentUnits "/" $currentUnitsToBeCreated "agent(s) created" >> ${1}/execution_report.log
        echo $currentMissions "/" $currentMissionsToBeLaunched "mission(s) launched" >> ${1}/execution_report.log
        echo `grep "mission impossible" ../../exercises/testings/testings${i}/sessions/default/Messages.log | wc -l` "mission impossible" >> ${1}/execution_report.log
    fi
done

echo "-------------------------------------------------------------------" >> ${1}/execution_report.log
echo "Total:" >> ${1}/execution_report.log
echo "------" >> ${1}/execution_report.log
echo $totalErrors "errors" >> ${1}/execution_report.log
echo $totalUnits "/" $totalUnitsToBeCreated "agent(s) created" >> ${1}/execution_report.log
echo $totalMissions "/" $totalMissionsToBeLaunched "mission(s) launched" >> ${1}/execution_report.log
echo `grep "mission impossible" ${1}/Messages_*.log | wc -l` "mission impossible" >> ${1}/execution_report.log
cat ${1}/execution_report.log

