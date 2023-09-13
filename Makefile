include makerc/definitions.mk
include makerc/options.mk
-include $(DEPS)

# ************************************Rules*********************************** #

all: $(NAME)
.PHONY: all

$(NAME): SHELL :=/bin/bash

$(NAME): $(BUILD_DIR) $(OBJS)
	$(CC) $(CFLAGS) $^ $(INCLUDE_FLAGS) -o $(NAME)

$(BUILD_DIR):
	@mkdir -p $@

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -MMD -c $< -o $@

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

run: all
	@./$(NAME)

recov: fclean cov
.PHONY: recov

test:
	./tests/test.sh

# **************************************************************************** #
