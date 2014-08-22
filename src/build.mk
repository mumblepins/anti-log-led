# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_LED_PATH = libraries/anti-log-led
TARGET_LED_SRC_PATH = $(TARGET_LED_PATH)/src

# Add include paths.
INCLUDE_DIRS += $(TARGET_LED_PATH)/inc

# C source files included in this build.
CSRC +=

# C++ source files included in this build.
CPPSRC += $(TARGET_LED_SRC_PATH)/anti-log-led.cpp

# ASM source files included in this build.
ASRC +=