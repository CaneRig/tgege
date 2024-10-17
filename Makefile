CC=g++
CFLAGS= -I/usr/local/include -I./include/ -std=c++2a -fno-inline-small-functions -g
LIBS= -lTgBot -lboost_system -lssl -lcrypto -lpthread


THR_PARTY_PATH=thrdparty
INCLUDE_PATH=include
SOURCE_PATH=scr
OBJ_PATH=obj
BUILD_PATH=build


APP_INC=$(INCLUDE_PATH)/Apps
APP_SCR=$(SOURCE_PATH)/Apps 
TEST_SCR_PATH=$(SOURCE_PATH)/Test
TEST_PATH=$(BUILD_PATH)/test
TEST_SOURCES = $(shell ls $(TEST_SCR_PATH))

CPL=$(CC) -c
CPR=$(CFLAGS) $(LIBS)

all: clean main.o tests

main.o: $(SOURCE_PATH)/main.cpp
	$(CC) $(SOURCE_PATH)/main.cpp $(CPR) -o $(BUILD_PATH)/build.o

tests:
	@mkdir build/test || echo "Nothing created"
	@for file in $(TEST_SOURCES) ; do \
		$(CC) $(TEST_SCR_PATH)/$$file $(CPR) -o $(TEST_PATH)/$$file.o ; \
	done


clean:
	@echo Cleaning...
	@rm -f $(shell find ./include -name "*.gch") || echo "Nothing have cleaned"
	@rm -f "*.o" || echo "Nothing have cleaned"
	@rm -r "build/test" || echo "Nothing have cleaned"
	@mkdir obj || echo "Nothing created"
	@mkdir build || echo "Nothing created"
	@mkdir build/test || echo "Nothing created"