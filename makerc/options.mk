#	Flags
ifdef	DEBUG
	CFLAGS					+=-g -fstandalone-debug

endif

ifdef	FSAN
	CFLAGS					+=-fsanitize=address,undefined
endif

ifdef	COV
	CFLAGS					+=--coverage
endif

# **************************************************************************** #
