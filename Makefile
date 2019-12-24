GENIE = /mnt/c/Linux/Dune/GENIE/lamp/
ROOT = /mnt/c/Linux/root6/root-bin/

GENIELIBS = -L$(GENIE)Generator/lib/ 
GENIEINDIR = $(GENIE)/GENIE-install/include/GENIE
ROOTLIBS =-L $(ROOT)/lib/
ROOTINDIR =$(ROOT)/include
GNULIBS =-L/usr/lib/x86_64-linux-gnu/ -llog4cpp -lxml2 -lgsl -lgslcblas

all: generate

generate: src/generate.cpp 
	g++ -std=c++11 src/generate.cpp -o bin/$@ -g -Wl,--no-as-needed -Wl,--no-undefined -I $(ROOTINDIR) -I $(GENIEINDIR) \
          $(ROOTLIBS) -lGui -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree \
		              -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread \
		              -lMultiProc -lROOTDataFrame -lGeom -lEGPythia6 -lEG -pthread -lm -ldl -rdynamic \
		  $(GENIELIBS) -lGFwMsg -lGFwReg -lGFwAlg -lGFwInt -lGFwGHEP -lGFwNum -lGFwUtl -lGFwParDat -lGFwEG \
		               -lGFwNtp -lGPhXSIg -lGPhPDF -lGPhNuclSt -lGPhCmn -lGPhDcy -lGPhHadTransp -lGPhHadnz -lGPhDeEx \
		               -lGPhAMNGXS -lGPhAMNGEG -lGPhChmXS -lGPhCohXS -lGPhCohEG -lGPhDISXS -lGPhDISEG -lGPhDfrcXS -lGPhDfrcEG \
		               -lGPhGlwResEG -lGPhIBDXS -lGPhIBDEG -lGPhMNucXS -lGPhMNucEG -lGPhMEL -lGPhNuElXS -lGPhNuElEG \
		               -lGPhResEG -lGPhStrXS -lGPhStrEG -lGTlGeo -lGTlFlx -lGPhQELXS -lGPhQELEG -lGPhResXS -lGPhGlwResXS \
		  $(GNULIBS)
