NULL =

CC = g++
MOC = moc
RCC = rcc

INCDIR = /usr/include
QTINCDIR = $(INCDIR)/qt5

TESTINCDIRS = `pkg-config --cflags cppunit`

TESTLIBS = `pkg-config --libs cppunit`

QTINCDIRS = \
	-I$(QTINCDIR) \
	-I$(QTINCDIR)/QtCore \
	-I$(QTINCDIR)/QtGui \
	-I$(QTINCDIR)/QtWidgets \
	$(NULL)

QTLIBS = \
	-lQt5Core \
	-lQt5Gui \
	-lQt5Widgets \
	$(NULL)

CFLAGS = -fPIC -Wall -Wpedantic -std=c++11 -g

MODELSRC = \
	src/interpreter/tree.cpp \
	src/model/agent.cpp \
	src/model/breed.cpp \
	src/model/boolean.cpp \
	src/model/changeable.cpp \
	src/model/color.cpp \
	src/model/line.cpp \
	src/model/function.cpp \
	src/model/number.cpp \
	src/model/nil.cpp \
	src/model/point.cpp \
	src/model/size.cpp \
	src/model/standard-function.cpp \
	src/model/string.cpp \
	src/model/table.cpp \
	src/model/turtle.cpp \
	src/model/type.cpp \
	src/model/user-function.cpp \
	src/model/world.cpp \
	src/model/zone.cpp \
	$(NULL)

BISONSRC = \
	src/interpreter/parser.y+ \
	$(NULL)

BISONTMP = \
	src/interpreter/stack.hh \
	src/interpreter/location.hh \
	src/interpreter/position.hh \
	src/interpreter/y.tab.c \
	src/interpreter/y.tab.h \
	$(NULL)

FLEXSRC = \
	src/interpreter/lexer.l+ \
	$(NULL)

FLEXTMP = \
	src/interpreter/lex.yy.c \
	$(NULL)

INTERPETERSRC = \
	src/model/user-function.cpp \
	src/interpreter/interpreter.cpp \
	src/interpreter/tree.cpp \
	src/interpreter/interpreter-exception.cpp \
	src/interpreter/syntax-exception.cpp \
	src/interpreter/semantic-exception.cpp \
	src/interpreter/turtle-interpreter.cpp \
	src/interpreter/world-interpreter.cpp \
	$(NULL)

TESTSRC = \
	src/tests/test-agent.cpp \
	src/tests/test-breed.cpp \
	src/tests/test-color.cpp \
	src/tests/test-function.cpp \
	src/tests/test-point.cpp \
	src/tests/test-number.cpp \
	src/tests/test-table.cpp \
	src/tests/test-turtle.cpp \
	src/tests/test-value.cpp \
	src/tests/test-world.cpp \
	src/tests/tests.cpp \
	$(NULL)

QTSRC = \
	src/qt/application.cpp \
	src/qt/runner.cpp \
	src/qt/stibbons.cpp \
	src/qt/window.cpp \
	src/qt/world-view.cpp \
	$(NULL)

MOCHEADERS = \
	src/qt/window.h \
	src/qt/world-view.h \
	$(NULL)

MOCSRC = $(MOCHEADERS:%.h=%.moc.cpp)

QRCFILES = \
	data/stibbons.qrc \
	$(NULL)

QRCSRC = $(QRCFILES:%.qrc=%.qrc.cpp)

MODELOBJECTS = $(MODELSRC:%.cpp=%.o)

BISONOBJECTS = $(BISONTMP:%.c=%.o)

FLEXOBJECTS = $(FLEXTMP:%.c=%.o)

INTERPRETEROBJECTS = $(INTERPETERSRC:%.cpp=%.o)

TESTOBJECTS = $(TESTSRC:%.cpp=%.o)

QTOBJECTS = $(QTSRC:%.cpp=%.o) $(MOCSRC:%.cpp=%.o) $(QRCSRC:%.cpp=%.o)

APP = stibbons

TEST = test

# Variables pour la documentation

DOCDIR = doc
REPDIR = $(DOCDIR)/report
RAILDIR = $(REPDIR)/rail
PDFDIR = $(DOCDIR)

RAILBIN = $(RAILDIR)/rail

REPTEX = $(REPDIR)/report.tex
REPRAI = $(DOCDIR)/report

REPDEPS = \
	$(REPDIR)/*.tex \
	$(NULL)

PDF = $(PDFDIR)/report.pdf

PDFCLN = \
	$(PDF) \
	$(PDF:%.pdf=%.aux) \
	$(PDF:%.pdf=%.log) \
	$(PDF:%.pdf=%.toc) \
	$(REPRAI).rai \
	$(NULL)

# Tout compiler

all: doc $(APP) $(TEST)

# Compilation du logiciel

$(APP): $(MODELOBJECTS) $(BISONOBJECTS) $(FLEXOBJECTS) $(INTERPRETEROBJECTS) $(QTOBJECTS)
	$(CC) $^ -o $@ $(CFLAGS) $(QTINCDIRS) $(QTLIBS)

$(TEST): $(MODELOBJECTS) $(BISONOBJECTS) $(FLEXOBJECTS) $(INTERPRETEROBJECTS) $(TESTOBJECTS)
	$(CC) $^ -o $@ $(CFLAGS) $(TESTINCDIRS) $(TESTLIBS)

$(BISONTMP): $(BISONSRC)
	bison -ydt -b $(<D)/y $<

$(FLEXTMP): $(FLEXSRC)
	flex -o $@ $<

%.moc.cpp: %.h
	$(MOC) $(QTINCDIRS) $< -o $@

%.qrc.cpp: %.qrc
	$(RCC) -name $(basename $(<F)) $< -o $@

$(MODELOBJECTS): %.o: %.cpp $(BISONTMP)
	$(CC) $< -c -o $@ $(CFLAGS)

$(BISONOBJECTS): %.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

$(FLEXOBJECTS): %.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

$(INTERPRETEROBJECTS): %.o: %.cpp $(BISONTMP)
	$(CC) $< -c -o $@ $(CFLAGS)

$(TESTOBJECTS): %.o: %.cpp
	$(CC) $< -c -o $@ $(CFLAGS) $(TESTINCDIRS) $(TESTLIBS)

$(QTOBJECTS): %.o: %.cpp
	$(CC) $< -c -o $@ $(CFLAGS) $(QTINCDIRS) $(QTLIBS)

# Compilation de la documentation

doc: $(PDF)

$(PDFDIR)/%.pdf: $(REPDIR)/%.tex $(RAILBIN)
	# Compile pour générer la TOC
	TEXINPUTS=.//:$$TEXINPUTS pdflatex -output-directory $(@D) $<
	# Compile les diagrammes de syntaxe
	if [ -a $(REPRAI).rai ] ; \
	then \
		$(RAILBIN) $(REPRAI) ; \
	fi;
	# Compile avec la TOC
	TEXINPUTS=.//:$$TEXINPUTS pdflatex -output-directory $(@D) $<

$(REPDIR)/report.tex: $(REPDEPS)
	touch $@

$(RAILBIN): $(RAILDIR)/Makefile
	make -C $(<D) -f $(<F)

clean:
	rm -Rf $(APP) $(TEST) \
	$(MODELOBJECTS) \
	$(BISONTMP) \
	$(BISONOBJECTS) \
	$(FLEXTMP) \
	$(FLEXOBJECTS) \
	$(INTERPRETEROBJECTS) \
	$(TESTOBJECTS) \
	$(MOCSRC) \
	$(QRCSRC) \
	$(QTOBJECTS) \
	$(PDFCLN)\
	$(NULL)
	make clean -C $(RAILDIR) -f Makefile

.PHONY: all doc clean

