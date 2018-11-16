CC := clang++-6.0
SRCDIR := src
BUILDDIR := build
TESTDIR := test
TARGET := gem

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
TESTS := $(shell find $(TESTDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTS:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++17 -O3 -flto
LIB := -lgtest -lgtest_main
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) -shared $^ -o lib/lib$(TARGET).so"; $(CC) -shared $(CFLAGS) $(INC) $^ -o lib/lib$(TARGET).so

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	@echo " $(CC) $(CFLAGS) -fpic $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) -fpic $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) lib/*.*"; $(RM) -r $(BUILDDIR) lib/*.*

# Tests
tests_main: $(TESTOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) tests_main.cc $(INC)  $(TESTOBJECTS) $(OBJECTS) -o bin/tests_main $(LIB)

$(BUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	@echo " Making $(@D)"
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: clean
