# Use common project's makefile tools.
include ../../COMMON/common.mk

CCFLAGS = -g
LDFLAGS =
INCLUDE_PATH = ../../INTERFACE
INCLUDES = $(addprefix -I, $(INCLUDE_PATH))

# Library output name.
OUTPUT_NAME      = relaysvr
OUTPUT_APP_NAME  = $(OUTPUT_NAME).bin
OUTPUT_TEST_NAME = $(OUTPUT_APP_NAME).test

ALL_TEST_SRC      = $(shell ls *_test.cc 2>/dev/null)
BUILD_TEST_OBJ    = $(patsubst %.cc,%.cc.o, $(ALL_TEST_SRC))
EXCLUDED_CC_SRC   = $(ALL_TEST_SRC)
ALL_CC_SRC        = $(shell ls *.cc 2>/dev/null)
BUILD_CC_SRC      = $(filter-out $(EXCLUDED_CC_SRC), $(ALL_CC_SRC))
BUILD_OBJ         = $(patsubst %.cc,%.cc.o, $(BUILD_CC_SRC))
BUILD_OBJ_NO_MAIN = $(filter-out main.cc.o, $(BUILD_OBJ))
# List of external dependencies like shared/static libraries.
APP_DEPENDENCIES  = ../../INTERFACE/libinterface.a ../../DEPENDENCIES/protobuf/src/.libs/libprotobuf.a /usr/local/lib/libcryptopp.a
TEST_DEPENDENCIES = /usr/local/lib/libgtest.a /usr/local/lib/libgtest_main.a

# Do not remove intermediate files. 
.PRECIOUS: %.cc.o %.a %.so

# Main target. Build interface library.
main: $(OUTPUT_APP_NAME)
	@echo "$@: Done."

# Remove all build artifacts and ProtoBuf generated sources and headers.
clean:
	@$(RM) *.cc.o
	@$(RM) $(OUTPUT_APP_NAME)

# Install built library in target location.
install:
	@echo "$@: Not implemented yet."

# Build and run unit tests.
test: $(OUTPUT_TEST_NAME)
# Run tests.
	@./$<
	@echo "$@: Done."

$(OUTPUT_TEST_NAME): $(BUILD_OBJ_NO_MAIN) $(BUILD_TEST_OBJ) $(TEST_DEPENDENCIES)
	$(CC) -o $@ $^ -lpthread

# Helper target to print all Makefile's variables.
debug_print:
	@echo "All variables:"
	@echo "    OUTPUT_NAME      : $(OUTPUT_NAME)"
	@echo "    OUTPUT_APP_NAME  : $(OUTPUT_APP_NAME)"
	@echo "    OUTPUT_TEST_NAME : $(OUTPUT_TEST_NAME)"
	@echo "    ALL_TEST_SRC     : $(ALL_TEST_SRC)"
	@echo "    BUILD_TEST_OBJ   : $(BUILD_TEST_OBJ)"
	@echo "    EXCLUDED_CC_SRC  : $(EXCLUDED_CC_SRC)"
	@echo "    ALL_CC_SRC       : $(ALL_CC_SRC)"
	@echo "    BUILD_CC_SRC     : $(BUILD_CC_SRC)"
	@echo "    BUILD_OBJ        : $(BUILD_OBJ)"
	@echo "    BUILD_OBJ_NO_MAIN: $(BUILD_OBJ_NO_MAIN)"
	@echo "    APP_DEPENDENCIES : $(APP_DEPENDENCIES)"
	@echo "    TEST_DEPENDENCIES: $(TEST_DEPENDENCIES)"
	@echo "    RM               : $(RM)"
	@echo "    PBC              : $(PBC)"
	@echo "    C                : $(C)"
	@echo "    CC               : $(CC)"
	@echo "    AR               : $(AR)"

# Make external static library.
%.a:
	$(MAKE) -C $(dir $@)

# Make external shared library.
%.so:
	$(MAKE) -C $(dir $@)

%.cc.o: %.cc
	$(CC) -c -o $@ $< $(CCFLAGS) $(INCLUDES)

%.bin: $(BUILD_OBJ) $(APP_DEPENDENCIES)
	$(CC) -o $@ $^ $(LDFLAGS)
