
CXX=g++

ifeq ($(DEBUG),true)
CC = $(CXX) -std=c++11 -g -Werror
VERSION = debug
else
CC = $(CXX) -std=c++11 -Werror
VERSION = release
endif

SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)

DEPS = $(SRCS:.cpp=.d)

BIN := $(addprefix $(BUILD_ROOT)/,$(BIN))

LINK_OBJ_DIR = $(BUILD_ROOT)/app/link_obj
DEP_DIR = $(BUILD_ROOT)/app/dep

$(shell mkdir -p $(LINK_OBJ_DIR))
$(shell mkdir -p $(DEP_DIR))

OBJS := $(addprefix $(LINK_OBJ_DIR)/,$(OBJS))
DEPS := $(addprefix $(DEP_DIR)/,$(DEPS))

LINK_OBJ = $(wildcard $(LINK_OBJ_DIR)/*.o)
LINK_OBJ += $(OBJS)

#-------------------------------------------------------------------------------------------------------
all:$(DEPS) $(OBJS) $(BIN)

ifneq ("$(wildcard $(DEPS))","")   #如果不为空,$(wildcard)是函数【获取匹配模式文件名】，这里 用于比较是否为""
include $(DEPS)  
endif

#----------------------------------------------------------------1begin------------------
$(BIN):$(LINK_OBJ)
	@echo "------------------------build $(VERSION) mode--------------------------------!!!"

#一些变量：$@：目标，     $^：所有目标依赖
# gcc -o 是生成可执行文件
	$(CC) -o $@ $^ -lpthread   -lpaho-mqtt3c -L/usr/local/lib
	cp $@ $(BUILD_ROOT)//bin

#----------------------------------------------------------------1end-------------------


#----------------------------------------------------------------2begin-----------------
$(LINK_OBJ_DIR)/%.o:%.cpp
	$(CC) -I$(INCLUDE_PATH) -o $@ -c $(filter %.cpp,$^)
#----------------------------------------------------------------2end-------------------



#----------------------------------------------------------------3begin-----------------
$(DEP_DIR)/%.d:%.cpp
	echo -n $(LINK_OBJ_DIR)/ > $@
	$(CC) -I$(INCLUDE_PATH) -MM $^ >> $@

#clean:			






