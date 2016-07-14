#!/bin/bash

cd sam

branch=$(git symbolic-ref --short HEAD)
coreversion=$(sed -n -e '/based/I s/^.* v\(1.*\)/\1/p' < $branch/readme.txt)

tar jcvf ../sam-$coreversion-opentracker-$branch.tar.bz2 $branch

cd ..
