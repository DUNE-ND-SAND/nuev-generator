GENIE = /wd/sw/GENIE/
ROOT = /wd/sw/ROOT/root-6.16.00/root-6.16.00.binary/
LOG4CPP = /wd/sw/LOG4CPP/log4cpp.binary/

GENIELIBS = -L$(GENIE)R-3_00_04/source/lib 
GENIEINDIR = $(GENIE)R-3_00_04/binary/include/GENIE
ROOTLIBS =-L $(ROOT)/lib/
ROOTINDIR =$(ROOT)/include
LOG4CPPINDIR=$(LOG4CPP)include/
LOG4CPPLIBS =-L$(LOG4CPP)lib/ -llog4cpp 
GNULIBS =-L/usr/lib64 -lxml2 -lgsl -lgslcblas

all: generate

generate: src/generate.cpp 
	g++ -std=c++11 src/generate.cpp -o bin/$@ -g -Wl,--no-as-needed -Wl,--no-undefined -I $(ROOTINDIR) -I $(GENIEINDIR) -I $(LOG4CPPINDIR)\
          $(ROOTLIBS) -lGui -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree \
		              -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread \
		              -lMultiProc -lROOTDataFrame -lGeom -lEGPythia6 -lEG -pthread -lm -ldl -rdynamic \
		  $(GENIELIBS) -lGFwMsg -lGFwReg -lGFwAlg -lGFwInt -lGFwGHEP -lGFwNum -lGFwUtl -lGFwParDat -lGFwEG \
		               -lGFwNtp -lGPhXSIg -lGPhPDF -lGPhNuclSt -lGPhCmn -lGPhDcy -lGPhHadTransp -lGPhHadnz -lGPhDeEx \
		               -lGPhAMNGXS -lGPhAMNGEG -lGPhChmXS -lGPhCohXS -lGPhCohEG -lGPhDISXS -lGPhDISEG -lGPhDfrcXS -lGPhDfrcEG \
		               -lGPhGlwResEG -lGPhIBDXS -lGPhIBDEG -lGPhMNucXS -lGPhMNucEG -lGPhMEL -lGPhNuElXS -lGPhNuElEG \
		               -lGPhResEG -lGPhStrXS -lGPhStrEG -lGTlGeo -lGTlFlx -lGPhQELXS -lGPhQELEG -lGPhResXS -lGPhGlwResXS \
		  $(GNULIBS) $(LOG4CPPLIBS)
