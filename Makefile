# Copyright (C) 2017, deadcafe.beef@gmail.com
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#   3. Neither the name of the project nor the names of its contributors
#      may be used to endorse or promote products derived from this software
#      without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

CC ?= gcc
AR ?= ar
RANLIB ?= ranlib
RM ?= rm
MKDIR ?= mkdir

CPPFLAGS := -std=gnu99 -D_GNU_SOURCE
LDFLAGS :=

CFLAGS := \
         -g -O3 -fPIC \
         -Werror -Wall -Wextra \
         -Wformat=2 -Wcast-qual -Wcast-align -Wwrite-strings \
         -Wconversion -Wfloat-equal -Wpointer-arith

SRCS = $(wildcard *.c)

OBJ_DIR := objs
TARGET := $(OBJ_DIR)/cpuid

OBJS = $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

DEPENDS =  $(OBJ_DIR)/.depends

.PHONY: all clean depend

.SUFFIXES: .c .o

all: depend $(TARGET)

$(TARGET): $(OBJS) $(LIB)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

depend:	$(OBJ_DIR) Makefile
	@$(CC) -MM $(CPPFLAGS) $(SRCS) > $(DEPENDS)

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

clean:
	@$(RM) -rf $(OBJ_DIR) $(TARGET)

-include $(DEPENDS)
