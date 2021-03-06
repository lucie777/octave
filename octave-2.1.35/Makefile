# Generated automatically from Makefile.in by configure.
#
# Makefile for octave's src directory
#
# John W. Eaton
# jwe@bevo.che.wisc.edu
# University of Wisconsin-Madison
# Department of Chemical Engineering

TARGETS = octave-bug octave-config mkoctfile libcruft liboctave \
	kpathsea dlfcn readline src scripts doc check octave.info \
	INSTALL.OCTAVE BUGS install install-strip uninstall \
	tags TAGS dist conf-dist snapshot snapshot-version

NO_DEP_TARGETS = clean mostlyclean distclean maintainer-clean

all: header-msg config-check
	$(MAKE) -f octMakefile all

$(TARGETS): FORCE
	$(MAKE) -f octMakefile $@

$(NO_DEP_TARGETS): FORCE
	$(MAKE) -f octMakefile omit_deps=true $@

binary-dist: FORCE
	@if test -f configure && test -f octMakefile.in; then \
	  ./configure --enable-shared --enable-lite-kernel ; \
	else \
	  echo ""; \
	  echo "*** You must run make binary-dist in the source directory."; \
	  echo ""; \
	  exit 1; \
	fi
	$(MAKE) -f octMakefile CFLAGS=-O CXXFLAGS=-O all
	$(MAKE) -f octMakefile binary-dist

# Maybe this message will prevent people from asking why the
# Makefiles don't work for them.  Maybe not.

header-msg: FORCE
	@echo ""
	@echo "***********************************************************"
	@echo "*"
	@echo "*  To compile Octave, you will need a recent versions of"
	@echo "*  the following software:"
	@echo "*"
	@echo "*    g++ (2.95.x or a more recent version)"
	@echo "*"
	@echo "*    flex (2.5.4 or a more recent version) -- required if"
	@echo "*    you need to recreate lex.cc from lex.l"
	@echo "*"
	@echo "*    bison (1.28 or a more recent version) -- required if"
	@echo "*    you need to recreate parse.cc from parse.y"
	@echo "*"
	@echo "*    gperf (2.7.1 or a more recent version) -- required if"
	@echo "*    you need to recreate oct-gperf.h from octave.gperf"
	@echo "*"
	@echo "*  Now would be a good time to read INSTALL.OCTAVE if"
	@echo "*  you have not done so already."
	@echo "*"
	@echo "***********************************************************"
	@echo ""

help: header-msg
	@echo ""
	@echo "For more information, please read the files INSTALL,"
	@echo "INSTALL.OCTAVE, and any other system-specific README"
	@echo "files that apply (e.g., README.Linux)."
	@echo ""
	@echo "The following targets are available:"
	@echo ""
	@echo "  all                  build everything"
	@echo "  check                run self tests"
	@echo "  install              install files"
	@echo "  install-strip        same as install but strip binaries"
	@echo "  uninstall            delete installed files"
	@echo ""
	@echo "  dist                 create a source distribution"
	@echo "  conf-dist            create a config files distribution"
	@echo "  binary-dist          create a binary distribution"
	@echo ""
	@echo "  clean                remove files created by make all"
	@echo "  mostlyclean          remove most files created by make all"
	@echo "  distclean            remove all files not in distribution"
	@echo "  maintainer-clean     clean up everything"
	@echo ""
	@echo "  octave-bug           create octave-bug script"
	@echo "  octave-config        create octave-config script"
	@echo "  mkoctfile            create mkoctfile script"
	@echo "  INSTALL.OCTAVE       create INSTALL.OCTAVE doc file"
	@echo "  BUGS                 create BUGS doc file"
	@echo "  tags                 create tags files"
	@echo "  TAGS                 create TAGS files"
	@echo ""
	@echo "  dlfcn                make all in subdirectory dlfcn"
	@echo "  doc                  make all in subdirectory doc"
	@echo "  kpathsea             make all in subdirectory kpathsea"
	@echo "  libcruft             make all in subdirectory libcruft"
	@echo "  liboctave            make all in subdirectory liboctave"
	@echo "  scripts              make all in subdirectory scripts"
	@echo "  src                  make all in subdirectory src"
	@echo ""
	@echo "  help                 print this message"
	@echo ""

config-check:
	@if test -f octMakefile; then \
	  true; \
	else \
	  echo ""; \
	  echo "*** You must run configure before running make."; \
	  echo "***"; \
	  echo "*** Please read the INSTALL and INSTALL.OCTAVE files"; \
	  echo "*** for more information about how to configure and"; \
	  echo "*** compile Octave."; \
	  echo ""; \
	  exit 1; \
	fi

FORCE:

.NOTPARALLEL:
