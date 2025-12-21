#!/bin/bash
sudo cat /sys/firmware/acpi/tables/DSDT > dsdt.dat
iasl -d dsdt.dat
head -50 dsdt.dsl
