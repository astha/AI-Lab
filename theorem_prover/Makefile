
#############################################################################################
#
#                                cfglp : A CFG Language Processor
#                                --------------------------------
#
#           About:
#
#           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
#           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
#           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
#           Bombay.
#
#           Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
#           Khedker. This  implemenation  has been made  available purely
#           for academic purposes without any warranty of any kind.
#
#           Documentation (functionality, manual, and design) and related
#           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp
#
#
###############################################################################################

AST_OBJ = ast.o
OBJ_CORE = main1.o 
OBJ_GEN = scanner.o parser.o

HEADERS_CORE = ast.hh 
HEADERS_GEN = parser.h parser.ih parserbase.h scanner.h scanner.ih scannerbase.h

OBJ =  $(OBJ_GEN)  $(OBJ_CORE) $(AST_OBJ)
HEADERS = $(HEADERS_CORE) $(HEADERS_GEN)

CPP = g++
BISONCPP = bisonc++
FLEXCPP = flexc++
TGT = theorem_prover

CFLAGS = -g 

$(TGT): $(OBJ) 
	$(CPP) --std=c++0x $(OBJ) -o $(TGT)

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x $(CFLAGS) -c $<

scanner.cc scannerbase.h: scanner.ll scanner.ih scanner.h
	$(FLEXCPP) scanner.ll

parser.cc parserbase.h: parser.yy parser.ih parser.h
	$(BISONCPP) --verbose parser.yy

clean :
	rm -f *.o cfglp64 cfgpp64 scannerbase.h parserbase.h scanner.cc parser.cc parser.yy.output