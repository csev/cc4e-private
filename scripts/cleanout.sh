#! /bin/bash

# sudo su -s "/home/csev/cleanout.sh" www-data

echo Removing hung a.out processes
touch /tmp/lastzap
ps -aux --sort=-pcpu | grep ^www-data | grep -v ' 0:00' | grep './a.out' | head -10 | tee -a /tmp/lastzap | awk '{ print "kill " $2}' | bash

