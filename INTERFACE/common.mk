# Common project's tools.

RM = `which rm` -rf
PBC = `which protoc`
C = `which gcc`
CC = `which g++`
AR = `which ar` rcs

ALL_PROTO = $(shell ls *.proto 2>/dev/null)
BUILD_PROTO = $(filter-out $(EXCLUDED_PROTO), $(ALL_PROTO))
BUILD_SOURCES = $(patsubst %.proto,%.pb.cc,$(BUILD_PROTO))
BUILD_HEADERS = $(patsubst %.proto,%.pb.h,$(BUILD_PROTO))
BUILD_OBJECTS = $(patsubst %.proto,%.pb.cc.o,$(BUILD_PROTO))
