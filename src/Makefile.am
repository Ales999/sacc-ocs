SUBDIRS		= auth 
bindir = ${prefix}/sacc/bin
bin_PROGRAMS = sacc racc
sacc_SOURCES = main.cpp krbconv.cpp namepair.cpp mylog.cpp
sacc_LDADD   =  -lm -lmysqlclient
racc_SOURCES = cisco.cpp
racc_LDADD   =  -lm -lmysqlclient

#EXTRA_DIST = main.cpp

install-data-local:
	$(mkinstalldirs) ${prefix}/sacc/bin/

uninstall-local:
	$(RM) -fr ${prefix}/sacc/bin

# set the include path found by configure
INCLUDES= $(all_includes)
 
# the library search path.
sacc_LDFLAGS = $(all_libraries)
#sacc_CFLAGS = -pg
sacc_CXXFLAGS = -Wall -W -Weffc++
racc_LDFLAGS = $(all_libraries)
racc_CXXFLAGS = -Wall -W
