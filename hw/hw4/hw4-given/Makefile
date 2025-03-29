## [M1: point 1]
#  Define the module name, which is a variable MODULE is holding.
MODULE   = hw4

## [M2: point 1]
#  Append the module object file to obj-m, which is required by the kernel build system.
#  This tells the build system to compile hw4.c into a kernel module.
obj-m += $(MODULE).o

## [M3: point 1]
#  Define the kernel build directory. Where source code is present for the kernel.
KERNELDIR ?= /lib/modules/$(shell uname -r)/build

## [M4: point 1]
#  Store the current working directory (PWD), by executing shell command pwd.
PWD := $(shell pwd)

## [M5: point 1]
#  Define the default build target.
all: $(MODULE)

## [M6: point 1]
#  Rule for compiling .c files into .o object files. (Uses regular expressions)
#  First print the compilation command.
#  $@ is the target name from the left side of the rule
#  $< is the first prerequisite (c source), from the right side of the rule
%.o: %.c
        @echo "  CC      $<"
        @$(CC) -c $< -o $@

## [M7: point 1]
#  The actual rule to build the kernel module.
#  Invokes make in the kernel build dir, specifying the current directory (M=$(PWD))
#  where our module source code is located.
#  modules is the target in the kernel build system for building loadable modules.
$(MODULE):
        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules

## [M8: point 1]
#  Clean up generated files. This invokes the kernel build system's clean target
#  to remove compiled objects and temporary files from the module directory.
clean:
        $(MAKE) -C $(KERNELDIR) M=$(PWD) clean
