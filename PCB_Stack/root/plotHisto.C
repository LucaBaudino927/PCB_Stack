// ROOT macro file for plotting histograms and analyze data
//
// Can be run from ROOT session:
// root[0] .x plotHisto.C

#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleReader.hxx>

{
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // Open file filled by Geant4 simulation
  TFile f("/home/luca/geant4_workdir/FlexBond/Geant4_FlexBond-main/output/output.root");

  bool useHistos = false;
  bool useNTuple = true;


  if(useNTuple){
	  // ##### MC data trouth
	  // ##### Photons
	  // Create a canvas and divide it into 2x3 pads
	  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
	  c1->Divide(2,3);//col,row  
	  TNtuple* ntuplePhotons = (TNtuple*)f.Get("Photons");
	  c1->cd(1);
	  ntuplePhotons->Draw("fEvent");
	  c1->cd(2);
	  ntuplePhotons->Draw("fX");
	  c1->cd(3);
	  ntuplePhotons->Draw("fY");
	  c1->cd(4);
	  ntuplePhotons->Draw("fZ");
	  c1->cd(5);
	  ntuplePhotons->Draw("fWlen");
	  c1->cd(6);
	  ntuplePhotons->Draw("fT");
	  
	  // ##### Experimental-like simulation
	  // ##### Hits
	  // Create a canvas and divide it into 2x2 pads
	  TCanvas* c2 = new TCanvas("c2", "", 20, 20, 1000, 1000);
	  c2->Divide(2,2);//col,row  
	  TNtuple* ntupleHits = (TNtuple*)f.Get("Hits");
	  c2->cd(1);
	  ntupleHits->Draw("fEvent");
	  c2->cd(2);
	  ntupleHits->Draw("fX");
	  c2->cd(3);
	  ntupleHits->Draw("fY");
	  c2->cd(4);
	  ntupleHits->Draw("fZ");
	  
	  // ##### Energy deposition for each event
	  // ##### Scoring
	  TCanvas* c3 = new TCanvas("c3", "", 20, 20, 1000, 1000);
	  TNtuple* ntupleScoring = (TNtuple*)f.Get("Scoring");
	  ntupleScoring->Draw("fEdep");
	  
  }else if(useHistos){
  
  
  }

}
