target_exercise=$1
testname=$2
reports_dir=$3

mkdir -p $reports_dir

echo "simulation errors :" `grep -r "Error" ${target_exercise}/$testname | grep -r "Sim.log" | wc -l`
echo "dispatcher errors :" `grep -r "Error" ${target_exercise}/$testname | grep -r "Dispatcher.log" | wc -l`
echo "mission impossible:" `grep -r "mission impossible" ${target_exercise}/$testname | wc -l`

echo $testname >> $reports_dir/reports.log
echo "-----" >> $reports_dir/reports.log
echo "simulation errors :" `grep -r "Error" ${target_exercise}/$testname | grep -r "Sim.log" | wc -l` >> $reports_dir/reports.log
echo "dispatcher errors :" `grep -r "Error" ${target_exercise}/$testname | grep -r "Dispatcher.log" | wc -l` >> $reports_dir/reports.log
echo "mission impossible:" `grep -r "mission impossible" ${target_exercise}/$testname | wc -l` >> $reports_dir/reports.log
echo "-----" >> $reports_dir/reports.log
