target_exercise=$1
reports_dir=$2

mkdir -p $reports_dir

for line in `find $target_exercise -name '*Dispatcher.log'`
do 
    cp $line $reports_dir/`echo $line | cut -d "/" -f 5`_dispatcher.log
done

for line in `find $target_exercise -name '*Sim.log'`
do 
    cp $line $reports_dir/`echo $line | cut -d "/" -f 5`_sim.log
done

for line in `find $target_exercise -name '*Messages.log'`
do 
    cp $line $reports_dir/`echo $line | cut -d "/" -f 5`_messages.log
done

echo "Errors:" `grep -r "Error" $reports_dir | wc -l`
echo "mission impossible:" `grep -r "mission impossible" $reports_dir | wc -l`
