#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include "math.h"
#include <TMath.h>
#include <algorithm>
#include "TTree.h"
#include "TClonesArray.h"
#include <TFile.h>
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <string>
#include "TString.h"
#include "TFitResult.h"
#include "TF1.h"
#include "TStyle.h"
#include <limits>

using namespace std;
constexpr double Pi = 3.141592653589793238462643383279502884;
constexpr double e  = 2.718281828459;
const int NofLayers = 4;
const TString layerName[NofLayers] = {"Alpide", "LowerGlue", "LowerKapton", "Copper"};
const double layerThickness[NofLayers] = {0.005, 0.0025, 0.005, 0.0005}; //cm
const double layerTeoX0[NofLayers] = {9.369, 44, 28.57, 1.436}; //cm
const int numberOfRuns = 19;
//const double referenceEnergy[numberOfRuns] = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000}; //MeV
const double referenceEnergy[numberOfRuns] = {10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000, 55000, 60000, 65000, 70000, 75000, 80000, 85000, 90000, 95000, 100000}; //MeV

/*
TFile f("B4.root");
 
TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
 c1->Divide(1,1);
 c1->cd(1);
 TTree *B4 = (TTree*)f.Get("B4");

 TF1 f1("f1","[0]*TMath::Landau(x,[1],[2])",0,530);
 f1.SetParameters(1.59,480,0.3);
 B4->Draw("Ecore>>h1(50,300,530)");
 Ecore->Fit("f1");
*/

void MaterialBudget_newMethod(){

  TString fileName = "../build/output0.root";
  TFile* file = new TFile(fileName);
  	  
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(1,1);
  c1->cd(1);
  TH2D *materialBudgetXY = (TH2D*)file->Get("Material Budget XY");
  materialBudgetXY->Draw("colz");
  
  cout << "Bin content = " << materialBudgetXY->GetBinContent(100,100) << endl;
  cout << "Bin error = "   << materialBudgetXY->GetBinError(100,100)   << endl;

  TCanvas* c2 = new TCanvas("c2", "", 20, 20, 1000, 1000);
  c2->Divide(1,1);
  c2->cd(1);
  TH2D *fNOfEventsXY = (TH2D*)file->Get("Number of Events XY");
  fNOfEventsXY->Draw("colz");

  cout << "Bin content = " << fNOfEventsXY->GetBinContent(100,100) << endl;
  cout << "Bin error = "   << fNOfEventsXY->GetBinError(100,100)   << endl;
  
  TCanvas* c3 = new TCanvas("c3", "", 20, 20, 1000, 1000);
  c3->Divide(1,1);
  c3->cd(1);
  TH2D *averageMaterialBudgetXY = (TH2D*)materialBudgetXY->Clone();
  averageMaterialBudgetXY->Divide(fNOfEventsXY);
  averageMaterialBudgetXY->Draw("colz");

  cout << "Bin content = " << averageMaterialBudgetXY->GetBinContent(100,100) << endl;
  cout << "Bin error = "   << averageMaterialBudgetXY->GetBinError(100,100)   << endl;

  /*

  //////////////////////////////VARIABLES/////////////////////////
  
  double mass; // MeV
  double z;
  
  mass = 0.511; //electron	
  //mass = 938.27; //proton
  z = 1.;
  
  //Scattering angles for each layer
  double AlpideScatteringAngle;
  double LowerGlueScatteringAngle;
  double LowerKaptonScatteringAngle;
  double CopperScatteringAngle;
  
  //Energy of particles before each layer
  double AlpideEnergy;
  double LowerGlueEnergy;
  double LowerKaptonEnergy;
  double CopperEnergy;
  
  double pBc[numberOfRuns][NofLayers];
  double pBcErr[numberOfRuns][NofLayers];
  double Theta[numberOfRuns][NofLayers];
  double ThetaErr[numberOfRuns][NofLayers];

  for(int run = 0; run < numberOfRuns; run++){
  
  	  TString fileName = "../build/elettroni_10_100GeV_step5/output"+std::to_string(run)+".root";
  	  TFile* file = new TFile(fileName);
  	  
  	  //---------------------------------------------------------------------------------------------
	  TTree *scatteringAnglesTree = (TTree*)file->Get("ScatteringAngles");
	  scatteringAnglesTree->SetBranchAddress("Alpide", &AlpideScatteringAngle);
	  scatteringAnglesTree->SetBranchAddress("LowerGlue", &LowerGlueScatteringAngle);
	  scatteringAnglesTree->SetBranchAddress("LowerKapton", &LowerKaptonScatteringAngle);
	  scatteringAnglesTree->SetBranchAddress("Copper", &CopperScatteringAngle);
	  
	  TTree *EnergyTree = (TTree*)file->Get("Energy before volume");
	  EnergyTree->SetBranchAddress("Alpide", &AlpideEnergy);
	  EnergyTree->SetBranchAddress("LowerGlue", &LowerGlueEnergy);
	  EnergyTree->SetBranchAddress("LowerKapton", &LowerKaptonEnergy);
	  EnergyTree->SetBranchAddress("Copper", &CopperEnergy);
	    	  
	  //---------------------------------------------------------------------------------------------
	  double ScatteringAngle[scatteringAnglesTree->GetEntries()][NofLayers];
	  double Energy[EnergyTree->GetEntries()][NofLayers];
	  for(int ev = 0; ev < scatteringAnglesTree->GetEntries(); ev++){
		scatteringAnglesTree->GetEntry(ev);
		ScatteringAngle[ev][0] = AlpideScatteringAngle;
		ScatteringAngle[ev][1] = LowerGlueScatteringAngle;
		ScatteringAngle[ev][2] = LowerKaptonScatteringAngle;
		ScatteringAngle[ev][3] = CopperScatteringAngle;
	  }
	  for(int ev = 0; ev < EnergyTree->GetEntries(); ev++){
		EnergyTree->GetEntry(ev);
		Energy[ev][0] = AlpideEnergy;
		Energy[ev][1] = LowerGlueEnergy;
		Energy[ev][2] = LowerKaptonEnergy;
		Energy[ev][3] = CopperEnergy;
	  }
	  
	  
	  //---------------------------------------------------------------------------------------------
	  for(int layer = 0; layer < NofLayers; layer++){
	  	TH1D* histo1[2];
	  	int nOfBins = 1000;
	  	double thetaHigh = 0.001; //rad
	  	double binWidth = thetaHigh*2./nOfBins; //rad
		histo1[0] = new TH1D(layerName[layer]+"ScatteringAngleH1", "Scattering angles in "+layerName[layer], 	nOfBins, -thetaHigh, thetaHigh);
		histo1[1] = new TH1D(layerName[layer]+"EnergyH1", 	   "Energy before "+layerName[layer],		nOfBins, referenceEnergy[run]-100,
															         referenceEnergy[run]+100);
		for(int ev = 0; ev < scatteringAnglesTree->GetEntries(); ev++) {
			if(ScatteringAngle[ev][layer] != 0.) histo1[0]->Fill(ScatteringAngle[ev][layer]);
		}
		for(int ev = 0; ev < EnergyTree->GetEntries(); ev++) {
			if(Energy[ev][layer] != 0.) histo1[1]->Fill(Energy[ev][layer]);
		}
		
		cout << endl << "----------------------------------------------------------" << endl;
		cout << 	"	Scattering angle fit for " << layerName[layer]	     << endl;
		new TCanvas();
		gPad->SetLogy();
		gStyle->SetOptFit(1);
		while(histo1[0]->GetStdDev() < 2*binWidth){
			histo1[0]->Reset("ICES");
			thetaHigh = thetaHigh/2.;
			binWidth = thetaHigh*2./nOfBins;
			histo1[0]->GetXaxis()->Set(nOfBins, -thetaHigh, thetaHigh);
			for(int ev = 0; ev < scatteringAnglesTree->GetEntries(); ev++)  histo1[0]->Fill(ScatteringAngle[ev][layer]);
		}
		//TF1* fitFunction = new TF1("fitFunction", fitFunc, 6);
		TFitResultPtr r0 = histo1[0]->Fit("gaus","S","", histo1[0]->GetMean()-2*histo1[0]->GetStdDev(), histo1[0]->GetMean()+2*histo1[0]->GetStdDev());
		histo1[0]->DrawCopy(); // scattering angles
		cout << endl << "----------------------------------------------------------" << endl;		
		
		cout << endl << "----------------------------------------------------------" << endl;
		cout << 	"	Energy fit for " << layerName[layer]	     	     << endl; 
		new TCanvas();
		gStyle->SetOptFit(1);
		TFitResultPtr r1 = histo1[1]->Fit("gaus", "S", "", referenceEnergy[run]-50, referenceEnergy[run]+50);
		histo1[1]->DrawCopy(); // energy
		cout << endl << "----------------------------------------------------------" << endl;
		
		Theta[run][layer]    = r0->Parameter(2); //histo1[0]->GetStdDev();
		ThetaErr[run][layer] = r0->ParError(2); //histo1[0]->GetStdDevError();
		pBc[run][layer]      = (r1->Parameter(1)*r1->Parameter(1)-mass*mass)/r1->Parameter(1);
		pBcErr[run][layer]   = r1->Parameter(2);
		
		cout << endl;
		cout << "==========================================================" << endl;
		cout << "    " << layerName[layer] << " Energy = " << r1->Parameter(1) << endl;
		cout << "    " << layerName[layer] << " pBc = " << pBc[run][layer] << endl;
		cout << "    " << layerName[layer] << " EnergySD = " << r1->Parameter(2) << endl;
		//cout << "    " << layerName[layer] << " thetaRMS = " << histo1[0]->GetStdDev() << ", theoric thetaRMS = " << 13.6*z*sqrt(layerThickness[layer]/layerTeoX0[layer])/pBc[run][layer]*(1+0.038*log(layerThickness[layer]/layerTeoX0[layer])) << endl;
		//cout << "    " << layerName[layer] << " thetaRMS_err = " << histo1[0]->GetStdDevError() << endl;
		cout << "    " << layerName[layer] << " thetaRMS = " << r0->Parameter(2) << ", theoric thetaRMS = " << 13.6*z*sqrt(layerThickness[layer]/layerTeoX0[layer])/pBc[run][layer]*(1+0.038*log(layerThickness[layer]/layerTeoX0[layer])) << endl;
		cout << "    " << layerName[layer] << " thetaRMS_err = " << r0->ParError(2) << endl;
		cout << "==========================================================" << endl;
				
	  }
	  file->Close();

  }
  
  //---------------------------------------------ANALISI-------------------------------------------------
  
  TGraphErrors *theta_E[NofLayers];
  double pBc_layer[numberOfRuns];
  double pBcErr_layer[numberOfRuns];
  double Theta_layer[numberOfRuns];
  double ThetaErr_layer[numberOfRuns];
    
  for(int layer = 0; layer < NofLayers; layer++){
  
  	  for(int run = 0; run < numberOfRuns; run++){
	  	pBc_layer[run] = pBc[run][layer];
	  	pBcErr_layer[run] = pBcErr[run][layer];
	  	Theta_layer[run] = Theta[run][layer];
	  	ThetaErr_layer[run] = ThetaErr[run][layer];
	  }
      
      	  double fitLow  = 9000;
      	  double fitHigh = 101000;
	  TF1 *fitFunction = new TF1("fitFunction", "13.6*[0]*(TMath::Sqrt([1]))*(1+0.038*TMath::Log([1]))/x", fitLow, fitHigh);
	  
	  fitFunction->SetParName(0, "z");
	  fitFunction->SetParName(1, "matBudget"); // X/X0
	  fitFunction->FixParameter(0, z);
	  //fitFunction->FixParameter(1, layerThickness[layer]/layerTeoX0[layer]);
	  //fitFunction->SetParLimits(1, layerThickness[layer]/layerTeoX0[layer]-0.0003, layerThickness[layer]/layerTeoX0[layer]+0.0003);

	  new TCanvas();
	  theta_E[layer] = new TGraphErrors(numberOfRuns, pBc_layer, Theta_layer, pBcErr_layer, ThetaErr_layer);
	  theta_E[layer]->SetTitle(layerName[layer] + " thetaRMS vs pBc");
	  theta_E[layer]->Draw();
	  
	  cout << endl << "----------------------------------------------------------" 		<< endl;
	  cout << 	  "	" << layerName[layer] + " Highland fit"		      		<< endl;
	  cout << 	  "	reference X/X0 = " << layerThickness[layer]/layerTeoX0[layer] 	<< endl;
	  TFitResultPtr res = theta_E[layer]->Fit("fitFunction", "S", "", fitLow, fitHigh);
	  fitFunction->Draw("SAME");
	  cout << 	  "	reference X0 = " << layerTeoX0[layer] 				<< endl;
	  cout << 	  "	experimental X0 = " << layerThickness[layer]/res->Parameter(1) 	<< endl;
	  cout << endl << "----------------------------------------------------------" << endl;
	  
  }
  
  */
  
}



