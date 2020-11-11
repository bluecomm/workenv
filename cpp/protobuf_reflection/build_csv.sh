protoc foo.proto --cpp_out=.
g++ -c foo.pb.cc -lpthread
g++ -c serialize2csv.cpp -lprotobuf -lboost -lpthread
g++ -o a.out foo.pb.o serialize2csv.o -lprotobuf -lpthread
