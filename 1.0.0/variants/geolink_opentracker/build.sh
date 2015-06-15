# Use with MSYS prompt
export ARM_GCC_TOOLCHAIN=$APPDATA/Arduino15/packages/arduino/tools/g++_arm_none_eabi/4.8.3-2014q1/bin

# test variant build
cd $APPDATA/Arduino15/packages/opentracker/hardware/sam/1.6.4/variants/geolink_opentracker/build_gcc/
make

# build library
cd $APPDATA/Arduino15/packages/arduino/hardware/sam/1.6.4/system/libsam/build_gcc/
make
