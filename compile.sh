rm invariantMassFitCal.exe
g++ -I. `root-config --libs --cflags` -lRooFitCore -lRooFit -lRooStats -lTMVA -lXMLIO -lTreePlayer -o invariantMassFitCal.exe invariantMassFitCal.C 
