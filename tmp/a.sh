#!/bin/sh

cmpt=0;
i= 1
while $i
do
    cmpt=$(($cmpt+1))
    i= ./a.out test21  $cmpt
done
