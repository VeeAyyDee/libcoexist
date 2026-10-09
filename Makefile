# SPDX-License-Identifier: MPL-2.0
CC ?= cc
AR ?= ar
CPPFLAGS = -DCOEX_HOST_TEST -Iinclude -Itests/sdk-shim -Ispec/public-sdk
CFLAGS = -std=gnu11 -O2 -g -Wall -Wextra -Werror -Wconversion -Wshadow -Wstrict-prototypes -Wmissing-prototypes -ffunction-sections -fdata-sections
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:src/%.c=build/%.o)
HEADERS = $(wildcard include/*.h spec/public-sdk/*.h tests/sdk-shim/*.h tests/sdk-shim/*/*.h)
.PHONY: all test clean layout manifest
all: build/libcoexist.a
build:
	mkdir -p build
build/%.o: src/%.c $(HEADERS) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
build/libcoexist.a: $(OBJECTS)
	$(AR) rcs $@ $^
layout: | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -m32 -ffreestanding -c tests/layout.c -o build/layout32.o
build/test: tests/test.c $(filter-out src/hardware.c src/scheduler.c src/external.c,$(SOURCES)) $(HEADERS) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -O1 -fsanitize=undefined,address -fno-omit-frame-pointer $(filter %.c,$^) -o $@
test: build/test build/test_full build/test_weak layout manifest
	ASAN_OPTIONS=detect_leaks=0 ./build/test
	ASAN_OPTIONS=detect_leaks=0 ./build/test_full
	ASAN_OPTIONS=detect_leaks=0 ./build/test_weak
clean:
	rm -rf build
build/test_full: tests/test_full.c $(SOURCES) $(HEADERS) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -O1 -fsanitize=undefined,address -fno-omit-frame-pointer $(filter %.c,$^) -o $@
build/test_weak: tests/test_full.c tests/weak_override.c $(SOURCES) $(HEADERS) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -DCOEX_TEST_WEAK_OVERRIDE -O1 -fsanitize=undefined,address -fno-omit-frame-pointer $(filter %.c,$^) -o $@
manifest: build/libcoexist.a
	python3 tools/audit_exports.py build/libcoexist.a
