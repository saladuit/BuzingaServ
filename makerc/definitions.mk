
NAME			:=WebServ.out

#	Compiler
CC				:=c++

#	Remove command
RM				:=rm -rf

#	Compiler flags
CFLAGS			=-Wall -Wextra -Werror -Wpedantic -Wfatal-errors -std=c++17
DFLAGS			:=-MMD -MP

#	Directories
SRC_DIR		 	:=src
INCLUDE_DIR		:=include
BUILD_DIR		:=build

#	Preprocessing files
SRCS			:=$(wildcard $(SRC_DIR)/*.cpp)
HEADERS			:=$(wildcard $(INCLUDE_DIR)/*.hpp)

# 	Objects
OBJS 			:=$(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

INCLUDE_FLAGS	:=$(addprefix -I, $(sort $(dir $(HEADERS))))
DEPENDS 		:= $(patsubst %.o,%.d,$(OBJS))

#	Coverage
COVERAGE_GCDA		:=build/**/*.gcda
COVERAGE_GCNO		:=build/**/*.gcno
COVERAGE_FILES		:=build/coverage.info
COVERAGE			:=$(COVERAGE_FILES) $(COVERAGE_GCDA) $(COVERAGE_GCNO)
# **************************************************************************** #
