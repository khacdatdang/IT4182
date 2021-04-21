#! /bin/bash
../incompleted/parser ../test/example1.kpl | diff ../test/result1.txt -
../incompleted/parser ../test/example2.kpl | diff ../test/result2.txt -
../incompleted/parser ../test/example3.kpl | diff ../test/result3.txt -
../incompleted/parser ../test/example4.kpl | diff ../test/result4.txt -

