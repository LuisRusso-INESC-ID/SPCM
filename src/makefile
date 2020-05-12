# BSD 2-Clause License

# Copyright (c) 2020, Luís M. S. Russo
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:

# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.

# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
# This is a multipurpose makefile
#

# IMPORTANT: splint main.c
# To pack the project use the pack-F2 dir, with symlinks and the archiver, for both procedures there are nautilus scripts.
# Profiling: gprof project
# kprof is graphical gprof: kprof
# Anotated code : gprof -l -A -x project
# Better anotated code : make coverage (then just less the .c.cov)
# WARNING just use kcachegrind it is good:
#                    valgrind --tool=callgrind -v ./project && kcachegrind callgrind.out.4330
# Memory alignment stuff
# pahole project
# valgrind --tool=cachegrind -v ./project

######### IMPORTANT DO NOT COMMENT THESE FLAGS

# The compiler to use
CC        = gcc
# The name of the executable to produce
EXEC      = project
# Ansi flags, make the compiler more informative
ANSI      = -ansi -pedantic -Wall -Wextra -std=c99
# Puts cacheline size in a macro
CLS       = -DCLS=$(getconf LEVEL1_DCACHE_LINESIZE)
# Use this if you want to use SSE2 instructions
#SSE2      = -msse2
# Debug flags for gdb/GUD
#DEBUG     = -O0 -gdwarf-2 -g3 -ggdb
#DEBUG     = -O0 -gdwarf-2
# Profile flags for gprof
#PROFILE   = -pg -fprofile-arcs -ftest-coverage
# Optimization flags for production
OPTIMIZE  = -O9 -D NDEBUG
# Static for static StandAlones
#STATIC = -static
# PThreads are usefull
#THREADS = -pthread

#########  COMMENT THESE

# Uncomment the flags that you need in a particular compile
# Always use ANSI
CFLAGS   = $(ANSI)
CFLAGS  += $(DEBUG)
CFLAGS  += -D_DEFAULT_SOURCE
#CFLAGS  += $(PROFILE)
CFLAGS  += $(OPTIMIZE)
#CFLAGS  += $(CLS) $(SSE2)
#CFLAGS  += $(STATIC)
#CFLAGS  += $(THREADS)

##  Base Makefile
##  Diferent classes of files
CS    = $(wildcard *.c)
HS    = $(wildcard *.h)
IS    = $(addsuffix .i, $(basename $(CS)))
SS    = $(addsuffix .s, $(basename $(IS)))
OS    = $(addsuffix .o, $(basename $(SS)))
HOLES = $(addsuffix .hls, $(basename $(SS)))
COVS  = $(addsuffix .c.gcov, $(basename $(CS)))
STATS = $(addsuffix .gcda, $(basename $(CS))) $(addsuffix .gcno, $(basename $(CS)))

# Phony targets
.PHONY: all coverage clean tags depend dox check

# Default Compile
all: $(SS) $(EXEC) $(HOLES)

coverage: $(COVS)

## Linking rule
$(EXEC): $(OS)
	@echo Linking: $@
	$(CC) $(CFLAGS) $(OS) -o $@ -lm -lbsd

## Run Preprocessor
%.i: %.c
	@echo PreProcess: $<
	$(CC) $(CFLAGS) -E -o $@ $<

## Generate Assembly
%.s: %.i
	@echo Assemble: $<
	$(CC) $(CFLAGS) -S -o $@ $<

## Build Object
%.o: %.s
	@echo Build Object from: $<
	$(CC) $(CFLAGS) -c -o $@ $<

## Find Holes
%.hls: %.o
	@echo Holes in: $<
	touch $@
	# pahole -I -H 1 $< | sed 's/.* \(.\+:[0-9]\+\) .*/\1:1:/'

## Pack using directory links
%.tgz: $(CS) $(HS) makefile
	@echo Pack Using directory
	tar -chvzf $@ $(basename $@ .tgz)

## Coverage statistics
%.c.gcov: %.gcda %.gcno
	@echo Anotated coverage source $<
	gcov -a -b -f $<
	dos2unix $@

## Make dependencies
depend: depend.mak

## Make dependencies forcing creation of all files
depend.mak: $(CS) $(HS)
	@echo Making dependencies ...
	$(CC) -MM $(CS) | sed 's/\.o/\.i/g' > depend.mak

-include depend.mak

## Clean up
clean:
	@echo Cleaning Up
	rm -f $(EXEC) $(STATS) $(COVS) $(IS) $(SS) $(OS) gmon.out depend.mak *~ callgrind.out.* cachegrind.out.*

## TAGS
tags: TAGS
TAGS: $(CS) $(HS)
	@echo Making TAGS
	etags $(CS) $(HS)

## doc, make documentation with doxymacs
dox: $(CS) $(HS) Doxyfile
	doxygen

## run splint for checking code
check: $(CS) $(HS)
	splint $<
