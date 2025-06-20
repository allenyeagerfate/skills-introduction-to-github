
export BUILD_ROOT = $(shell pwd)

#定义头文件的路径变量
export INCLUDE_PATH = $(BUILD_ROOT)/include

#定义我们要编译的目录
BUILD_DIR = $(BUILD_ROOT)/util \
			$(BUILD_ROOT)/network \
			$(BUILD_ROOT)/modbus \
			$(BUILD_ROOT)/mqtt \
			$(BUILD_ROOT)/modules \
			$(BUILD_ROOT)/service \
			$(BUILD_ROOT)/app

export DEBUG = true

