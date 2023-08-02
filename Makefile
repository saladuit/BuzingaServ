# **************************************************************************** #
#                                                                              #
#                                                     .--.  _                  #
#    Makefile                                        |o_o || |                 #
#                                                    |:_/ || |_ _   ___  __    #
#    By: safoh <safoh@student.codam.nl>             //   \ \ __| | | \ \/ /    #
#                                                  (|     | )|_| |_| |>  <     #
#    Created: 2022/07/07 17:49:38 by safoh        /'\_   _/`\__|\__,_/_/\_\    #
#    Updated: 2022/10/27 11:30:15 by safoh        \___)=(___/                  #
#                                                                              #
# **************************************************************************** #

include makerc/definitions.mk
include makerc/options.mk

# ************************************Rules*********************************** #

all: $(NAME)
.PHONY: all

$(NAME): SHELL :=/bin/bash

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ $(INCLUDE_FLAGS) -o $(NAME)

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

clean:
	@$(RM) $(BUILD_DIR)
.PHONY: clean

fclean: clean
	@$(RM) $(NAME)
.PHONY: fclean

re: fclean all
.PHONY: re

debug:
	@$(MAKE) DEBUG=1
.PHONY: debug

rebug: fclean debug
.PHONY: rebug

fsan:
	@$(MAKE) DEBUG=1 FSAN=1
.PHONY: fsan

resan: fclean fsan
.PHONY: resan

cov:
	@$(RM) $(COVERAGE_GCDA) $(COVERAGE_FILES)
	@$(MAKE) DEBUG=1 COV=1
	@./$(NAME)
	@lcov -q -d build -c --output-file build/coverage.info --rc lcov_branch_coverage=1
	@genhtml -q build/coverage.info -o build/coverage_report --rc genhtml_branch_coverage=1
.PHONY: cov

recov: fclean cov
.PHONY: recov

# **************************************************************************** #
