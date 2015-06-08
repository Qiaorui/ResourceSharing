# Resource Sharing Problem
> project A

### Compile
> make

### Clean
> make clean

### Run
> main.exe [-sap | -pre] [-file  file_name | -dir directory_name | -benchmark]

* -sap: using sap algorithm
* -pre: using preflow algorithm
* -file: you can read input data from file
* -dir:  you can read input data from each file of directory
* -benchmark: run benchmark testing using default files

### Test
> make test
>> you will test with default input.in

> make test_benchmark_sap
>> you will test with benchmark with sap algorithm

> make test_benchmark_pre
>> you will test with benchmark with preflow push relabel algorithm