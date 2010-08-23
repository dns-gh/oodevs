unitsToCreate=`grep -o "sim:CreateUnit" out/testings/scripts/*.lua | wc -l`
missionsToLaunch=`grep -o "Mission.create" out/testings/scripts/*.lua | wc -l`
unitsCreated=`grep -o " : agent creation" $1 | wc -l`
missionsLaunched=`grep -o " : running " $1 | wc -l`
echo -n "Units created : "
echo -n $unitsCreated
echo -n "/"
echo $unitsToCreate
echo -n "Missions launched : "
echo -n $missionsLaunched
echo -n "/"
echo $missionsToLaunch