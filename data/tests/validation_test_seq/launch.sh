scripts_dir=$1
reports_dir=$2

for file in `find $scripts_dir -name '*.sh'`
do 
    echo "executing" $file"..."
    testname=`echo $file | cut -d '/' -f 2 | cut -d '.' -f 1`
    mkdir -p ${reports_dir}
    $file 1> ${reports_dir}/${testname}_exec.log
done