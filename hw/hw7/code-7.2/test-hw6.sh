#!/bin/bash

set -x  # Enable command tracing

sudo insmod hw6.ko 
cat /proc/perftop
sudo rmmod hw6.ko
#sudo dmesg | tail -n 25

set +x  # Disable command tracing
