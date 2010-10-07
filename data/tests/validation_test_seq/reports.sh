target_exercise=$1
reports_dir=$2

nbfile=`find $target_exercise/* -prune | wc -l`

for i in `seq 1 $nbfile`
do 
    exercisename=`find $target_exercise/* -prune | sed $i{D} | grep -o "[^/]*$"`
    dispatcher=`find $target_exercise/$exercisename -name 'Dispatcher.log'`
    messages=`find $target_exercise/$exercisename -name 'Messages.log'`
    sim=`find $target_exercise/$exercisename -name 'Sim.log'`
    mkdir -p $reports_dir
    cp $dispatcher $reports_dir/${exercisename}_Dispatcher.log
    cp $messages $reports_dir/${exercisename}_Messages.log
    cp $sim $reports_dir/${exercisename}_Sim.log
done