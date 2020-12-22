# Use common project's makefile tools.
include common.mk

# Library output name.
OUTPUT_NAME = interface
OUTPUT_LIB_NAME = lib$(OUTPUT_NAME)

# If some files should not be compiled - put them here.
EXCLUDED_PROTO =

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
	@echo === ALL VARIABLES ===
	@echo "OUTPUT_NAME    : $(OUTPUT_NAME)"
	@echo "OUTPUT_LIB_NAME: $(OUTPUT_LIB_NAME)"
	@echo "EXCLUDED_PROTO : $(EXCLUDED_PROTO)"
	@echo "ALL_PROTO      : $(ALL_PROTO)"
	@echo "BUILD_PROTO    : $(BUILD_PROTO)"
	@echo "BUILD_SOURCES  : $(BUILD_SOURCES)"
	@echo "BUILD_HEADERS  : $(BUILD_HEADERS)"
	@echo "BUILD_OBJECTS  : $(BUILD_OBJECTS)"
	@echo "RM             : $(RM)"
	@echo "PBC            : $(PBC)"
	@echo "C              : $(C)"
	@echo "CC             : $(CC)"
	@echo "AR             : $(AR)"
	@echo === ALL VARIABLES END ===

%.pb.h: %.proto
	$(PBC) --cpp_out=. $<

%.pb.cc: %.pb.h
# Empty rule. Just proxy to have both cc and h as target dependencies.
# : is bash no-op. @: does not print to stdout.
	@:

%.pb.cc.o: %.pb.cc
	$(CC) -fPIC -c -o $@ $<

%.so: $(BUILD_OBJECTS)
	$(CC) -shared -o $@ $^

%.a: $(BUILD_OBJECTS)
	$(AR) $@ $^
