MKDIR_P    = mkdir -p
RM         = rm
INSTALL    = install

CC         = cc
CSTANDARD  = -std=gnu99
CWARNINGS  = -Wall -Wextra -Wshadow -pedantic -Wno-unused-result -Wvla
COPTIMIZE  = -O2
CFLAGS     = $(CSTANDARD) $(CWARNINGS) $(COPTIMIZE) $(CDEBUG)

LDLIBS     = -lm

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

all: napizza

napizza: napizza.c Makefile
	$(CC) $(CFLAGS) $< $(LDLIBS) -o $@

install: all
	$(MKDIR_P) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 napizza $(DESTDIR)$(BINDIR)

uninstall:
	$(RM) -f $(DESTDIR)$(BINDIR)/napizza

clean:
	$(RM) -f napizza

.PHONY: all install uninstall clean
