import ROOT
import scipy
import numpy
import sys

if len ( sys.argv ) != 3:
    print (" USAGE : % s < input file > < output file > " %( sys.argv [0]))
    sys . exit (1)
inFileName = sys.argv [1]
outFileName = sys.argv [2]
print (" Reading from " , inFileName , " and writing to " , outFileName)

inFile = ROOT.TFile.Open( inFileName , " READ " )
tree = inFile.Get ( " HASCO " )