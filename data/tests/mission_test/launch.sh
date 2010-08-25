if (( $2 ))
then 
    cd "../../../run/vc80"
    echo "Running testings/testings$2 ..."
    echo "Log file : ${1}_$2.log"
    ../../out/applications/simulation_app/vc80/Release/simulation_app.exe --root-dir=../../data --exercise=testings/testings$2 --session=default > ../../data/tests/mission_test/${1}_$2.log
else
    echo "all tests !"
    nbfile=`find ../../exercises/testings/* -prune -print | wc -l`
    cd "../../../run/vc80"
    for i in `seq 1 $nbfile`
    do
        echo "Running testings/testings$i ..."
        echo "Log file : ${1}_$i.log"
        ../../out/applications/simulation_app/vc80/Release/simulation_app.exe --root-dir=../../data --exercise=testings/testings$i --session=default > ../../data/tests/mission_test/${1}_$i.log
    done
fi

