#!/bin/sh 

if [ $# == 0  ];then 
    echo "build sarting ..."
    protoc -I=./ --cpp_out=./ test_field.proto
    g++ -c main.cpp
    g++ -c test_field.pb.cc
    g++ -lprotobuf -o a.out main.o test_field.pb.o
    echo "build finished"
elif [ $1 == "clean" ];then
    echo "clean sarting ..."
	rm -rf a.out
	rm -rf *.o
	rm -rf test_proto.pb.cc
	rm -rf test_proto.pb.h
    echo "clean finished"
else
    echo "unknown option"
fi
