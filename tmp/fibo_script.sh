#!/bin/sh

cmpt=0;
i= 1
while $i
do
    echo "Je calcule Fibo" $cmpt
    cmpt=$(($cmpt+1))
    i= ./build/test51 $cmpt
done
