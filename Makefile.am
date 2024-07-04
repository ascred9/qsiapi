bin_PROGRAMS = qsiapp 

qsiapp_CPPFLAGS = $(all_includes) -I$(top_srcdir)/lib -I/usr/local/include
qsiapp_CXXFLAGS = -g -Wall `root-config --cflags`

qsiapp_LDFLAGS = $(all_libraries) -L$(top_srcdir)/lib -L/usr/local/lib `root-config --glibs`
qsiapp_LDADD = $(top_builddir)/lib/libqsiapi.la 

qsiapp_SOURCES = MyMainFrame.h MyMainFrame.cxx app.cpp
