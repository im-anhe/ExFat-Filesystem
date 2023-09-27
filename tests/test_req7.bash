#!/bin/bash
#iter 1 req spreadsheet 7: Print error message if invalid arguments that are not
# specified are entered on command line. Examples would include '-g' or '-z'.
#2 total scenarios

#scenario 1 starts
./extfat -g test.image
#exit_code will be used to determine pass or fail
exit_key=$?
#test_result differs from exit_code because exit_key has to be 1 to pass the test_result
test_result=0

#if program handled error when random commands like -g or -z, then passed. Otherwise, failed.
if [ $exit_key -eq 1 ]; then
    echo "Program *handles error correctly* when -g is entered."
else
    echo "Program *does not handle error correctly* when -g is entered."
    test_result=1
fi


#scenario 2 starts
./extfat -z test.image
#exit_code will be used to determine pass or fail
exit_key=$?
#test_result differs from exit_code because exit_key has to be 1 to pass the test_result

#if program handled error when random commands like -g or -z, then passed. Otherwise, failed.
if [ $exit_key -eq 1 ]; then
    echo "Program *handles error correctly* when -z is entered."
else
    echo "Program *does not handle error correctly* when -z is entered."
    test_result=1
fi


exit $test_result

