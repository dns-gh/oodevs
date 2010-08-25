if (( $2 ))
then 
    cd "../../../run/vc80"
    echo "Running testings/testings${2}..."
    ../../out/applications/simulation_app/vc80/Release/simulation_app.exe --root-dir=../../data --exercise=testings/testings$2 --session=default > ../../data/tests/mission_test/${1}/exec_$2.log
else
    nbfile=`find ../../exercises/testings/* -prune -print | wc -l`
    cd "../../../run/vc80"
    for i in `seq 1 $nbfile`
    do
        echo "Running testings/testings${i}..."
        ../../out/applications/simulation_app/vc80/Release/simulation_app.exe --root-dir=../../data --exercise=testings/testings$i --session=default > ../../data/tests/mission_test/${1}/exec_$i.log
    done
fi
