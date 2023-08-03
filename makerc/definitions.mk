# **************************************************************************** #
#                                                                              #
#                                                     .--.  _                  #
#    definitions.mk                                  |o_o || |                 #
#                                                    |:_/ || |_ _   ___  __    #
#    By: safoh <safoh@student.codam.nl>             //   \ \ __| | | \ \/ /    #
#                                                  (|     | )|_| |_| |>  <     #
#    Created: 2023/05/24 15:08:00 by safoh        /'\_   _/`\__|\__,_/_/\_\    #
#    Updated: 2023/05/24 15:08:00 by safoh        \___)=(___/                  #
#                                                                              #
# **************************************************************************** #

NAME			:=WebServ.out

#	Compiler
CC				:=c++

#	Remove command
RM				:=rm -rf

#	Compiler flags
CFLAGS			=-Wall -Wextra -Werror -Wpedantic -std=c++17

#	Directories
SRC_DIR		 	:=src
INCLUDE_DIR		:=include
BUILD_DIR		:=build

#	Preprocessing files
SRCS			:=$(wildcard $(SRC_DIR)/*.cpp)
HEADERS			:=$(wildcard $(INCLUDE_DIR)/*.hpp)

# 	Objects
OBJS			:=$(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

INCLUDE_FLAGS	:=$(addprefix -I, $(sort $(dir $(HEADERS))))

#	Coverage
COVERAGE_GCDA		:=build/**/*.gcda
COVERAGE_GCNO		:=build/**/*.gcno
COVERAGE_FILES		:=build/coverage.info
COVERAGE			:=$(COVERAGE_FILES) $(COVERAGE_GCDA) $(COVERAGE_GCNO)
# **************************************************************************** #
