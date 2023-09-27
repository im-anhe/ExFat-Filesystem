# for this project, there is only one makefile
#
# this makefile is expected to be run from the project 
# root.
#
# all executables are expected to be ran from the project 
# root.
#
# this makefile will clean up when asked politely
#
# No intermediate .o files or libraries are created

CPPFLAGS=-Wall -Wextra -O0 -std=gnu17 -g3 
#-fsanitize=address

# note address sanitizer "-fsanitize=address" is new. it can be
# removed from the makefile if it causes problems.

CPPINCS=-I./include

all:mmap fread unit_tests extfat crc_example

CPPFLAGS= -Wall -Wextra -O0 -std=gnu17 -g3 -fsanitize=bounds-strict -fsanitize=address
# the utility that is a focus of this project

extfat:src/extfat.c common/routines.c src/process_cmd_args.c src/read_write.c src/readDirectory.c src/deleteFile.c src/extractFile.c

	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^

# unit tests
unit_tests: my_tests

my_tests:unit_tests/munit/my_tests.c common/routines.c src/process_cmd_args.c src/readDirectory.c src/read_write.c src/extractFile.c src/deleteFile.c
	${CC} ${CPPFLAGS} unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^

system_tests: extfat
	bash tests/system_tests.bash

# requirements tests


# example code
mmap:examples/mmap.c common/routines.c
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^

fread:examples/fread.c  
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^

crc_example:examples/crc_example.c
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^ -lz

# run tests
tests: run_unit_tests system_tests

run_unit_tests: my_tests
	./my_tests

clean:
	-rm -f mmap fread my_tests extfat crc_example