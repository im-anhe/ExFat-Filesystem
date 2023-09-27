#!/bin/bash
# for req 4 and 4.1

#too many comments not added because echo"..." is very informative

#default return is success
test_result=0

#3 scenarios

#scenario 1 starts
./extfat -c
exit_key=$?
if [ $exit_key -eq 1 ]; then
    echo "Program *handles error correctly* when just -c is entered without -i or -o."
else
    echo "Program *does not handle error correctly* when just -c is entered without -i or -o."
    test_result=1
fi


#scenario 2 starts: takes into ccount both stderr as well as exit code
./extfat -o test1.image -c
exit_key=$?
output=$(./extfat -o test1.image -c 2>&1)
if [[ $exit_key -eq 1 && "$output" == *"Needed to specify input file at least before using -c."* ]]; then
    echo "Program *handles error correctly* when just -o and filename is entered without -i for copying."
else
    echo "Program *does not handle error correctly* when just -o and filename is entered without -i for copying."
    test_result=1
fi



#scenario 3 starts: tests the proper full form command for copying (uses prof's test_copy.bash components)
result1=($(/usr/bin/md5sum test.image))
./extfat -i test.image -o test1.image -c
exit_key=$?
if [ $exit_key -eq 1 ]; then
    echo "Program *incorrectly thinks there should be exit code 1* when command arguments are fully functional."
    test_result=1
else
    echo "Program *correctly thinks exit code should be 0* when command arguments are fully functional."
fi

result2=($(/usr/bin/md5sum test1.image))
# clean up
$(rm test1.image)
if [ "${result1[0]}" != "${result2[0]}" ]; then
    echo "md5sum1: $result1"
    echo "md5sum2: $result2"
    echo "Program *does not copy inputfile to outputfile correctly* as seen in md5sum."
    test_result=1
else
    echo "md5sum1: $result1"
    echo "md5sum2: $result2"
    echo "Program *copies inputfile to outputfile correctly* as seen in md5sum."
fi


exit $test_result