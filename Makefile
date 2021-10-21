# Call make for each subdirectory recursively.
build:
	for i in */; do $(MAKE) -C $$i; done

clean:
	for i in */; do $(MAKE) -C $$i clean; done

rebuild: clean build
