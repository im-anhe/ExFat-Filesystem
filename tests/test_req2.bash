#!/bin/bash
# for iter 1 spreadsheet req 2: The program shall accept ' -o' command to 
#specify the output file. If nothing is specified, outputfile is same as inputfile.

#3 scenarios tested

#default test result
test_result=0
#scenario 1 starts
output=$(./extfat -o 2>&1)
echo $output
find="Need arguments for the command!"
if [[ "$output" == *"$find"* ]]; then
    echo "Program *handles error correctly* when -o is not followed by any file names."
else
    echo "Program *does not handle error correctly* when -o is not followed by any file names."
    test_result=1
fi

#scenario 2 starts
output=$(./extfat -o newfilehasbeencreated.image 2>&1)
echo $output
find="Output file: newfilehasbeencreated.image"
if [[ "$output" == *"$find"* ]]; then
    echo "Program *successfully creates a new file* when -o is followed by a new file name to create."
else
    echo "Program *fails to create a new file* when -o is followed by a new file name to create."
    test_result=1
fi


#scenario 3 starts
output=$(./extfat -i test.image -c 2>&1)
echo $output
find="Output file name is test.image"
if [[ "$output" == *"$find"* ]]; then
     echo "Program *makes outputfile as inputfile* when inputfile is provided but outputile is not."
else
    echo "Program *does not make outputfile as inputfile* when inputfile is provided but outputile is not."
    test_result=1
fi

exit $test_result

