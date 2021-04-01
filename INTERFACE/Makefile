# Use common project's makefile tools.
include ../COMMON/common.mk

CCFLAGS = -g

# Library output name.
OUTPUT_NAME = e2eem-iface
OUTPUT_LIB_NAME = lib$(OUTPUT_NAME)

# If some files should not be compiled - put them here.
EXCLUDED_PROTO = FileMessage.proto
ALL_PROTO      = $(shell ls *.proto 2>/dev/null)
BUILD_PROTO    = $(filter-out $(EXCLUDED_PROTO), $(ALL_PROTO))
BUILD_SOURCES  = $(patsubst %.proto,%.pb.cc,$(BUILD_PROTO))
BUILD_HEADERS  = $(patsubst %.proto,%.pb.h,$(BUILD_PROTO))
BUILD_OBJECTS  = $(patsubst %.proto,%.pb.cc.o,$(BUILD_PROTO))

# Do not remove intermediate files. 
.PRECIOUS: %.pb.h %.pb.cc %.pb.cc.o

# Main target. Build interface library.
main: $(OUTPUT_LIB_NAME).so $(OUTPUT_LIB_NAME).a

# Remove all build artifacts and ProtoBuf generated sources and headers.
clean:
	@$(RM) *.pb.*
	@$(RM) *.so
	@$(RM) *.a

# Install built library in target location.
install:
	@echo "$@: Not implemented yet."

# Helper target to print all Makefile's variables.
debug_print:
	@echo "All variables:"
	@echo "    OUTPUT_NAME    : $(OUTPUT_NAME)"
	@echo "    OUTPUT_LIB_NAME: $(OUTPUT_LIB_NAME)"
	@echo "    EXCLUDED_PROTO : $(EXCLUDED_PROTO)"
	@echo "    ALL_PROTO      : $(ALL_PROTO)"
	@echo "    BUILD_PROTO    : $(BUILD_PROTO)"
	@echo "    BUILD_SOURCES  : $(BUILD_SOURCES)"
	@echo "    BUILD_HEADERS  : $(BUILD_HEADERS)"
	@echo "    BUILD_OBJECTS  : $(BUILD_OBJECTS)"
	@echo "    RM             : $(RM)"
	@echo "    PBC            : $(PBC)"
	@echo "    C              : $(C)"
	@echo "    CC             : $(CC)"
	@echo "    AR             : $(AR)"

# Display makefile help.
help:
	@echo "You can invoke this Makefile in the following ways:"
	@echo "    make             - will build default target, i.e. shared and static libraries."
	@echo "    make clean       - will remove all build artifacts / all make-generated files."
	@echo "    make install     - will install binaries on the system in the E2E_INTERFACE_INSTALL_PATH path."
	@echo "    make debug_print - will print all Makefile's variables."
	@echo "    make help        - will display Makefile's help."

%.pb.h: %.proto
	$(PBC) --cpp_out=. $<

%.pb.cc: %.pb.h
# Empty rule. Just proxy to have both cc and h as target dependencies.
# : is bash no-op. @: does not print to stdout.
	@:

%.pb.cc.o: %.pb.cc
	$(CC) -fPIC -c -o $@ $< $(CCFLAGS)

# Special rule that has complex dependency.
AllMessages.pb.cc.o: AllMessages.pb.cc ForwardTargetMessage.pb.cc TargetMessage.pb.cc
	$(CC) -fPIC -c -o $@ $< $(CCFLAGS)

%.so: $(BUILD_OBJECTS)
	$(CC) -shared -o $@ $^

%.a: $(BUILD_OBJECTS)
	$(AR) $@ $^
