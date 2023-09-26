#	Flags
ifdef	DEBUG
	CFLAGS					+=-g
endif

ifdef	FSAN
	CFLAGS					+=-fsanitize=address,undefined
endif

ifdef	COV
	CFLAGS					+=--coverage
endif

# **************************************************************************** #
