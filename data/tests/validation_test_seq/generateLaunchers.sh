exe=$1
rootdir=$2
working_dir=$3
scripts_dir=$4
exercisename=$5
sword_doc=$6

mkdir -p $scripts_dir

for file in `find $sword_doc -name '*.ord'`
do 
    testname=`echo $file | grep -o "[^/]*$" | cut -d "." -f 1`
    touch $scripts_dir/$testname.sh
    chmod +x $scripts_dir/$testname.sh
    echo 'cd '$working_dir'
'$exe' --root-dir='$rootdir' --exercise="'$exercisename'/'$testname'" --session=default' > $scripts_dir/$testname.sh
done