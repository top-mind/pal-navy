# General makefile for generic unix & linux platforms

TARGET = sdlpal
TEST_TARGET = ./sdlpal-tests

INTER=.unix

HOST =
TEST_CCFLAGS =

GENERATED = -DPAL_HAS_GIT_REVISION $(shell ../scripts/gengitrev)

DEPFLAGS = -MT $@ -MMD -MP -MF $*$(INTER).Td

GTEST_DIR = ../3rd/googletest/googletest

SOURCES = .. ../adplug
CFILES = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.cpp)) unix.cpp
TOBJFILES = $(CFILES:.c=$(INTER).o) $(CPPFILES:.cpp=$(INTER).o)
DEPFILES = $(OBJFILES:.o=.d)
TEST_CPPFILES = $(wildcard ../tests/*.cpp)
TEST_OBJFILES = $(TEST_CPPFILES:.cpp=.o)
SDL_CONFIG = sdl2-config


CC = $(HOST)gcc
CXX = $(HOST)g++
override CCFLAGS += `$(SDL_CONFIG) --cflags` -D_GNU_SOURCE -g -Wall -O2 -fno-strict-aliasing $(EXTRA_CCFLAGS) $(TEST_CCFLAGS) $(GENERATED)
CXXFLAGS = $(CCFLAGS) -std=c++11
CFLAGS = $(CCFLAGS) -std=gnu99
LDFLAGS = `$(SDL_CONFIG) --libs` -lstdc++ -lm
TEST_CXXFLAGS += -isystem $(GTEST_DIR)/include -I $(GTEST_DIR) -g -Wall -Wextra -pthread

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -framework OpenGL
	OBJFILES = $(filter-out ../mini_glloader.unix.o, $(TOBJFILES))
else
	LDFLAGS += -lGL -pthread
	OBJFILES = $(TOBJFILES)
endif

POSTCOMPILE = @mv -f $*$(INTER).Td $*$(INTER).d && touch $@

.PHONY : all clean check

all: $(TARGET)

$(TARGET): $(OBJFILES)
	@echo [LD] $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

gtest-all.o : $(GTEST_DIR)/src/gtest-all.cc %.d
	@echo [CC] $<
	@$(CXX) $(DEPFLAGS) $(TEST_CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

%$(INTER).o: %.c %$(INTER).d
	@echo [CC] $<
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
	$(POSTCOMPILE)

%$(INTER).o: %.cpp %$(INTER).d
	@echo [CC] $<
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(TEST_TARGET): $(OBJFILES) $(TEST_OBJFILES) gtest-all.o
	@echo [LD] $@
	@$(CXX) $^ -o $@ $(LDFLAGS) -lpthread

clean:
	-@rm -f $(TARGET) $(TEST_TARGET) $(OBJFILES) $(TEST_OBJFILES) $(DEPFILES)

%.d: ;
.PRECIOUS: %.d

-include $(DEPFILES)

%$(INTER).o: EXTRA_CCFLAGS := -I. -I.. -DPAL_HAS_PLATFORM_SPECIFIC_UTILS
check: TEST_CCFLAGS = -I. -I.. -DUNIT_TEST=1 -isystem $(GTEST_DIR)/include
check: $(TEST_TARGET)
	@echo [EXEC] $(TEST_TARGET)
	@chmod +x $(TEST_TARGET)
	@exec $(TEST_TARGET)
