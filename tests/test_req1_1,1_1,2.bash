#!/bin/bash
#for req 1, 1.1, 1.2

#there are 3 scenarios tested here

#scenario 1 starts
output=$(./extfat -i 2>&1)
echo $output
if [[ "$output" == *"Need arguments for the command!"* ]]; then
    echo "Program *handles error correctly* when -i is not followed by any file names."
else
    echo "Program *does not handle error correctly* when -i is not followed by any file names."
    test_result=1
fi

#scenario 2 starts
output=$(./extfat -i -o 2>&1)
echo $output
if [[ "$output" == *"-i must be followed by test.image file type"* ]]; then
    echo "Program *handles error correctly* when -i is followed by invalid files/words like -o."
else
    echo "Program *does not handle error correctly* when -i is followed by an invalid word like -o."
    test_result=1
fi

#scenario 3 starts
output=$(./extfat -i test.image 2>&1)
echo $output
if [[ "$output" == *"Input file: test.image"* ]]; then
    echo "Program *handles correctly* when -i is followed by an existant normal file name like test.image."
else
    echo "Program *does not handle correctly* even when -i is followed by an existant normal file name like test.image."
    test_result=1
fi

exit $test_result
