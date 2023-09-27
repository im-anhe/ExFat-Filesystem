#!/bin/bash
#for req 9

#there is 1 scenario tested here

#scenario 1 starts
output=$(./extfat 2>&1)
echo $output
#default is success
test_result=0
find='Please add command line arguments in order to proceed.'
if [[ "$output" == *"$find"* ]]; then
    echo "Program *handles error correctly* when argc is less than 2."
else
    echo "Program *does not handle error correctly* when argc is less than 2."
    test_result=1
fi

exit $test_result