#!/bin/sh

cmpt=0;
i= 1

while $i
do
    echo "Fibo Pthread" $cmpt
    i= ./a.out pthtest51 $cmpt
    echo "Fibo MyPthread" $cmpt
    ./a.out test51 $cmpt
    cmpt=$(($cmpt+1))
done
