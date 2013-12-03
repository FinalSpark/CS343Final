CXX = u++					# compiler
CXXFLAGS = -g -Wall -MMD			# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = config.o q1driver.o q1printer.o q1student.o q1bank.o q1parent.o q1plant.o q1truck.o q1vending.o q1name.o q1card.o q1office.o	# object files forming executable
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXEC = soda      					# executable name

.PHONY : clean					# not file names

${EXEC} : ${OBJECTS}				# link step
	${CXX} ${CXXFLAGS} $^ -o $@		# additional object files before $^

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

# make implicitly generates rules to compile C++ files that generate .o files

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}
