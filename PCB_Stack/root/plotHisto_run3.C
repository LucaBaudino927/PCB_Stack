// ROOT macro file for plotting histograms and analyze data
//
// Can be run from ROOT session:
// root[0] .x plotHisto_run3.C

{
	gROOT->Reset();
	gROOT->SetStyle("Plain");

	// Open file filled by Geant4 simulation
	TFile f("/home/luca/geant4_workdir/FlexBond/Geant4_FlexBond-main/output/output.root");
	
	// Create a canvas and divide it into 2x3 pads
	TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1500, 1000);
	c1->Divide(3,2);//col,row
	c1->cd(1);
	TH1F* h1_HitTime = (TH1F*)f.Get("Layer1, HitTime");
	h1_HitTime->Draw();
	c1->cd(2);
	TH1F* h1_Hit_Z = (TH1F*)f.Get("Layer1, Hit_Z");
	h1_Hit_Z->Draw();
	c1->cd(3);
	TH1F* h1_HitEdep = (TH1F*)f.Get("Layer1, HitEdep");
	h1_HitEdep->Draw();
	c1->cd(4);
	TH1F* h2_HitTime = (TH1F*)f.Get("Layer2, HitTime");
	h2_HitTime->Draw();
	c1->cd(5);
	TH1F* h2_Hit_Z = (TH1F*)f.Get("Layer2, Hit_Z");
	h2_Hit_Z->Draw();
	c1->cd(6);
	TH1F* h2_HitEdep = (TH1F*)f.Get("Layer2, HitEdep");
	h2_HitEdep->Draw();

	// ##### Energy deposition for each event
	// ##### Scoring
	TCanvas* c2 = new TCanvas("c2", "", 20, 20, 1000, 1000);
	TNtuple* ntupleScoring = (TNtuple*)f.Get("Scoring");
	ntupleScoring->Draw("fEdep");
	  

}
