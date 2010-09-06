totalDispatcherErrors=0
totalSimulationErrors=0
totalUnits=0
totalMissions=0
totalUnitsToBeCreated=0
totalMissionsToBeLaunched=0

nbfile=`find ../../exercises/testings -maxdepth 1 | wc -l`

for i in `seq 1 $nbfile`
do
    dispatcherLog=../../exercises/testings/testings${i}/sessions/default/Dispatcher.log
    simLog=../../exercises/testings/testings${i}/sessions/default/Sim.log
    messageLog=../../exercises/testings/testings${i}/sessions/default/Messages.log

    if [ -e $dispatcherLog ]
    then
        currentDispatcherErrors=`grep Error $dispatcherLog | wc -l`
        if (( `expr $currentDispatcherErrors != 0` ))
        then
            echo $dispatcherLog >> ${1}/errors_report.log
            echo "\n" `grep Error $dispatcherLog` >> ${1}/errors_report.log
        fi
        currentSimulationErrors=`grep Error $simLog | wc -l`
        if (( `expr $currentSimulationErrors != 0` ))
        then
            echo $simLog >> ${1}/errors_report.log
            echo `grep Error $simLog` >> ${1}/errors_report.log
        fi
        currentUnits=`grep -r "agent creation" $dispatcherLog | wc -l`
        currentMissions=`grep -r " : running" $dispatcherLog | wc -l`
        currentUnitsToBeCreated=`grep -r "sim:CreateUnit(" ../../exercises/testings/testings${i}/scripts | wc -l`
        currentMissionsToBeLaunched=`grep -r "Mission.create(" ../../exercises/testings/testings${i}/scripts | wc -l`

        totalDispatcherErrors=`expr $totalDispatcherErrors + $currentDispatcherErrors`
        totalSimulationErrors=`expr $totalSimulationErrors + $currentSimulationErrors`
        totalUnits=`expr $totalUnits + $currentUnits`
        totalMissions=`expr $totalMissions + $currentMissions`
        totalUnitsToBeCreated=`expr $totalUnitsToBeCreated + $currentUnitsToBeCreated`
        totalMissionsToBeLaunched=`expr $totalMissionsToBeLaunched + $currentMissionsToBeLaunched`

        cp $dispatcherLog ${1}/Dispatcher_${i}.log
        cp ../../exercises/testings/testings$i/sessions/default/Messages.log ${1}/Messages_${i}.log
        cp $simLog ${1}/Sim_${i}.log
        cp ../../exercises/testings/testings$i/sessions/default/actions.ord ${1}/actions_${i}.ord
        
        echo "-------------------------------------------------------------------" >> ${1}/execution_report.log
        echo $dispatcherLog >> ${1}/execution_report.log
        echo $currentDispatcherErrors "dispatcher errors" >> ${1}/execution_report.log
        echo $currentSimulationErrors "simulation errors" >> ${1}/execution_report.log
        echo $currentUnits "/" $currentUnitsToBeCreated "agent(s) created" >> ${1}/execution_report.log
        echo $currentMissions "/" $currentMissionsToBeLaunched "mission(s) launched" >> ${1}/execution_report.log
        missionImpossible=`grep "mission impossible" $messageLog | wc -l`
        echo $missionImpossible "mission impossible" >> ${1}/execution_report.log
        if (( `expr $missionImpossible != 0` ))
        then
            echo $messageLog >> ${1}/errors_report.log
            echo `grep "mission impossible" $messageLog` >> ${1}/errors_report.log
        fi
    fi
done

echo "-------------------------------------------------------------------" >> ${1}/execution_report.log
echo "Total:" >> ${1}/execution_report.log
echo "------" >> ${1}/execution_report.log
echo $totalDispatcherErrors "dispatcher errors" >> ${1}/execution_report.log
echo $totalSimulationErrors "simulation errors" >> ${1}/execution_report.log
echo $totalUnits "/" $totalUnitsToBeCreated "agent(s) created" >> ${1}/execution_report.log
echo $totalMissions "/" $totalMissionsToBeLaunched "mission(s) launched" >> ${1}/execution_report.log
echo `grep "mission impossible" ${1}/Messages_*.log | wc -l` "mission impossible" >> ${1}/execution_report.log
cat ${1}/execution_report.log

if (( `expr $totalDispatcherErrors = 0` && `expr $totalSimulationErrors = 0` && `expr $totalUnits = $totalUnitsToBeCreated`  && `expr $totalMissions = $totalMissionsToBeLaunched` ))
then
    exit 0
else
    exit 1
fi
