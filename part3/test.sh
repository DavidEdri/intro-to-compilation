output=./output.txt
testsFolder=./tests

for f in tests/*.code
do
    tmp=${f%%.*}

    if ! ./dist/output "test" < $tmp.code; then
        echo "t1.code failed"
    else
        DIFF=$(diff  -w -q $output $tmp.txt)
        if [ "$DIFF" != "" ]
        then
            echo "$tmp.code failed"
        fi
    fi 
done

