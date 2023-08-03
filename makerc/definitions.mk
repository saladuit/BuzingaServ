
NAME			:=WebServ.out

#	Compiler
CC				:=c++

#	Remove command
RM				:=rm -rf

#	Compiler flags
CFLAGS			=-Wall -Wextra -Werror -Wpedantic -std=c++20

#	Directories
SRC_DIR		 	:=src
INCLUDE_DIR		:=include
BUILD_DIR		:=build

#	Preprocessing files
SRCS			:=$(wildcard $(SRC_DIR)/*.cpp)
HEADERS			:=$(wildcard $(INCLUDE_DIR)/*.hpp)

# 	Objects and Dependancies
OBJS			:=$(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS			:=$(OBJS:%.o=%.d)

INCLUDE_FLAGS	:=$(addprefix -I, $(sort $(dir $(HEADERS))))

#	Coverage
COVERAGE_GCDA		:=build/**/*.gcda
COVERAGE_GCNO		:=build/**/*.gcno
COVERAGE_FILES		:=build/coverage.info
COVERAGE			:=$(COVERAGE_FILES) $(COVERAGE_GCDA) $(COVERAGE_GCNO)
# **************************************************************************** #
