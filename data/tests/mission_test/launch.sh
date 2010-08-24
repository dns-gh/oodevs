nbfile=`find ../../exercises/testings/* -prune -print | wc -l`
cd "../../../run/vc80"
for i in `seq 1 $nbfile`
do
    echo "Running testings/testings$i ..."
    echo "Log file : ${1}_$i.log"
    ../../out/applications/simulation_app/vc80/Release/simulation_app.exe --root-dir=../../data --exercise=testings/testings$i --session=default > ../../data/tests/mission_test/${1}_$i.log
done

