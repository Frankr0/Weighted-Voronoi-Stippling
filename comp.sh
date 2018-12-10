#!/bin/bash

basepath=$(cd `dirname $0`; pwd)

if [ "$1" == "-c" ]
then
	echo "Clean....."	
	rm -rf  $basepath/build/*
fi

cd $basepath/build
echo "Build....."
cmake ..
make

