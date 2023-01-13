#!/bin/bash
for i in {1..100}
do
   echo $i > /tmp/$1.sock
   sleep 0.01
done