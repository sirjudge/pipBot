#!/bin/bash

# for each argument passed in
# will be the name of each file you want to build
for var in $@
do
	# print what file you're making
	echo making file $var
	# actually build that file	
	make $var
	# using scp, copy the file from this computer to the home directory of pip
	# this replaces the sftp pip + put dance
	scp $var pip:/home/pip
done
