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
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <string>
#include "TString.h"
#include <boost/math/special_functions/lambert_w.hpp>
#include "TFitResult.h"

using boost::math::lambert_w0;
using boost::math::lambert_wm1;
using namespace std;
constexpr double Pi = 3.141592653589793238462643383279502884;
constexpr double e  = 2.718281828459;
const int NofLayers = 5;

void MaterialBudget(std::string fileName){

  TString* fileNameTString = new TString(fileName);
  TFile *file1= new TFile(*fileNameTString);

  //////////////////////////////VARIABLES/////////////////////////
  
  double mass; // MeV
  
  bool proton = false;
  bool electron = false;
  if(!(fileName.find("e-") == std::string::npos)){
  	electron = true;
  	mass = 0.511;
  } else if(!(fileName.find("proton") == std::string::npos)){
  	proton = true;
  	mass = 938.27;
  } else {
  	throw domain_error("No particle specified in the file name");
  }

  //Scattering angles for each layer
  double AlpideScatteringAngle;
  double LowerGlueScatteringAngle;
  double LowerKaptonScatteringAngle;
  double CopperScatteringAngle;
  double PCBScatteringAngle;
  
  //Energy of particles before each layer
  double AlpideEnergy;
  double LowerGlueEnergy;
  double LowerKaptonEnergy;
  double CopperEnergy;
  double PCBEnergy;
  
  ///////////////////////TTREES////////////////////////////////////

  TTree *scatteringAnglesTree = (TTree*)file1->Get("ScatteringAngles");
  scatteringAnglesTree->SetBranchAddress("Alpide", &AlpideScatteringAngle);
  scatteringAnglesTree->SetBranchAddress("LowerGlue", &LowerGlueScatteringAngle);
  scatteringAnglesTree->SetBranchAddress("LowerKapton", &LowerKaptonScatteringAngle);
  scatteringAnglesTree->SetBranchAddress("Copper", &CopperScatteringAngle);
  scatteringAnglesTree->SetBranchAddress("PCB", &PCBScatteringAngle);
  
  TTree *EnergyTree = (TTree*)file1->Get("Energy before volume");
  EnergyTree->SetBranchAddress("Alpide", &AlpideEnergy);
  EnergyTree->SetBranchAddress("LowerGlue", &LowerGlueEnergy);
  EnergyTree->SetBranchAddress("LowerKapton", &LowerKaptonEnergy);
  EnergyTree->SetBranchAddress("Copper", &CopperEnergy);
  EnergyTree->SetBranchAddress("PCB", &PCBEnergy);
  
  double ScatteringAngle[scatteringAnglesTree->GetEntries()][NofLayers];
  double Energy[EnergyTree->GetEntries()][NofLayers];
  
  for(int ev = 0; ev < scatteringAnglesTree->GetEntries(); ev++){
	scatteringAnglesTree->GetEntry(ev);
	ScatteringAngle[ev][0] = AlpideScatteringAngle;
	ScatteringAngle[ev][1] = LowerGlueScatteringAngle;
	ScatteringAngle[ev][2] = LowerKaptonScatteringAngle;
	ScatteringAngle[ev][3] = CopperScatteringAngle;
	ScatteringAngle[ev][4] = PCBScatteringAngle;
  }
  
  for(int ev = 0; ev < EnergyTree->GetEntries(); ev++){
	EnergyTree->GetEntry(ev);
	Energy[ev][0] = AlpideEnergy;
	Energy[ev][1] = LowerGlueEnergy;
	Energy[ev][2] = LowerKaptonEnergy;
	Energy[ev][3] = CopperEnergy;
	Energy[ev][4] = PCBEnergy;
  }
  
  ///////////////////////////HISTOS///////////////////////////////
  
  /*
  double averageScatteringAngle = 0;
  double ScatteringAngleMax = 0;
  double ScatteringAngleMin = 0;
  double ScatteringAngleMedian = 0;
  for(int i = 0; i < scatteringAnglesTree->GetEntries(); i++){
  	averageScatteringAngle += ScatteringAngle[i];
  	//cout<<"ScatteringAngle[i] = "<<ScatteringAngle[i]<<endl;
  	//ScatteringAngleMedian = median(ScatteringAngle, scatteringAnglesTree->GetEntries());
  	if(ScatteringAngle[i] > ScatteringAngleMax) ScatteringAngleMax = ScatteringAngle[i];
  	if(ScatteringAngle[i] < ScatteringAngleMin) ScatteringAngleMin = ScatteringAngle[i];
  }
  averageScatteringAngle = averageScatteringAngle/scatteringAnglesTree->GetEntries();
  //cout<<"averageScatteringAngle = "<<averageScatteringAngle<<endl;
  
  double averageEnergy = 0;
  double EnergyMax = 0;
  double EnergyMin = 0;
  double EnergyMedian = 0;
  for(int i = 0; i < EnergyTree->GetEntries(); i++){
  	averageEnergy += Energy[i];
  	EnergyMedian = median(Energy, EnergyTree->GetEntries());
  	if(Energy[i] > EnergyMax) EnergyMax = Energy[i];
  	if(Energy[i] < EnergyMin) EnergyMin = Energy[i];
  }
  averageEnergy = averageEnergy/EnergyTree->GetEntries();
  cout<<"averageEnergy = "<<averageEnergy<<endl;
  */
  
  TString layerName[NofLayers] = {"Alpide", "LowerGlue", "LowerKapton", "Copper", "PCB"}; 
  TH1D* histo1[2][NofLayers];
  //--------------------------elettroni 10MeV-----------------------------------------------
  if(electron && !(fileName.find("10MeV") == std::string::npos)) {
	histo1[0][0] = new TH1D(layerName[0]+"ScatteringAngleH1", "Scattering angles in "+layerName[0], 100, -0.2, 0.2);
	histo1[1][0] = new TH1D(layerName[0]+"EnergyH1", 	  "Energy before "+layerName[0],	100, 9.8, 10.01);
	histo1[0][1] = new TH1D(layerName[1]+"ScatteringAngleH1", "Scattering angles in "+layerName[1], 100, -0.2, 0.2);
	histo1[1][1] = new TH1D(layerName[1]+"EnergyH1", 	  "Energy before "+layerName[1],	100, 9.8, 10.01);
	histo1[0][2] = new TH1D(layerName[2]+"ScatteringAngleH1", "Scattering angles in "+layerName[2], 100, -0.2, 0.2);
	histo1[1][2] = new TH1D(layerName[2]+"EnergyH1", 	  "Energy before "+layerName[2],	100, 9.8, 10.01);
	histo1[0][3] = new TH1D(layerName[3]+"ScatteringAngleH1", "Scattering angles in "+layerName[3], 100, -0.2, 0.2);
	histo1[1][3] = new TH1D(layerName[3]+"EnergyH1", 	  "Energy before "+layerName[3],	100, 9.8, 10.01);
	histo1[0][4] = new TH1D(layerName[4]+"ScatteringAngleH1", "Scattering angles in "+layerName[4], 100, -0.2, 0.2);
	histo1[1][4] = new TH1D(layerName[4]+"EnergyH1", 	  "Energy before "+layerName[4],	100, 9.8, 10.01);
  }
  //----------------------------------------------------------------------------------------
  //--------------------------protoni 10 GeV------------------------------------------------
  else if(proton && !(fileName.find("10GeV") == std::string::npos)) {
	  histo1[0][0] = new TH1D(layerName[0]+"ScatteringAngleH1", "Scattering angles in "+layerName[0], 100, -50E-6, 100E-6);
	  histo1[1][0] = new TH1D(layerName[0]+"EnergyH1", 	    "Energy before "+layerName[0], 	  100, 10043.8, 10043.91);
	  histo1[0][1] = new TH1D(layerName[1]+"ScatteringAngleH1", "Scattering angles in "+layerName[1], 100, -50E-6, 100E-6);
	  histo1[1][1] = new TH1D(layerName[1]+"EnergyH1", 	    "Energy before "+layerName[1], 	  100, 10043.8, 10043.91);
	  histo1[0][2] = new TH1D(layerName[2]+"ScatteringAngleH1", "Scattering angles in "+layerName[2], 100, -50E-6, 100E-6);
	  histo1[1][2] = new TH1D(layerName[2]+"EnergyH1", 	    "Energy before "+layerName[2], 	  100, 10043.8, 10043.91);
	  histo1[0][3] = new TH1D(layerName[3]+"ScatteringAngleH1", "Scattering angles in "+layerName[3], 100, -50E-6, 100E-6);
	  histo1[1][3] = new TH1D(layerName[3]+"EnergyH1", 	    "Energy before "+layerName[3], 	  100, 10043.8, 10043.91);
	  histo1[0][4] = new TH1D(layerName[4]+"ScatteringAngleH1", "Scattering angles in "+layerName[4], 100, -50E-6, 100E-6);
	  histo1[1][4] = new TH1D(layerName[4]+"EnergyH1", 	    "Energy before "+layerName[4], 	  100, 10043.8, 10043.91);
  }
  //----------------------------------------------------------------------------------------
  //--------------------------protoni 200 MeV-----------------------------------------------
  else if(proton && !(fileName.find("200MeV") == std::string::npos)) {
	  histo1[0][0] = new TH1D(layerName[0]+"ScatteringAngleH1", "Scattering angles in "+layerName[0], 100, -0.02, 0.02);
	  histo1[1][0] = new TH1D(layerName[0]+"EnergyH1", 	    "Energy before "+layerName[0],        100, 958.7, 959.);
	  histo1[0][1] = new TH1D(layerName[1]+"ScatteringAngleH1", "Scattering angles in "+layerName[1], 100, -0.02, 0.02);
	  histo1[1][1] = new TH1D(layerName[1]+"EnergyH1", 	    "Energy before "+layerName[1],        100, 958.85, 959.1);
	  histo1[0][2] = new TH1D(layerName[2]+"ScatteringAngleH1", "Scattering angles in "+layerName[2], 100, -0.02, 0.02);
	  histo1[1][2] = new TH1D(layerName[2]+"EnergyH1", 	    "Energy before "+layerName[2],        100, 959., 959.3);
	  histo1[0][3] = new TH1D(layerName[3]+"ScatteringAngleH1", "Scattering angles in "+layerName[3], 100, -0.02, 0.02);
	  histo1[1][3] = new TH1D(layerName[3]+"EnergyH1", 	    "Energy before "+layerName[3],        100, 957.8, 958.6);
	  histo1[0][4] = new TH1D(layerName[4]+"ScatteringAngleH1", "Scattering angles in "+layerName[4], 100, -0.02, 0.02);
	  histo1[1][4] = new TH1D(layerName[4]+"EnergyH1", 	    "Energy before "+layerName[4],        100, 957.8, 958.7);
  }
  //----------------------------------------------------------------------------------------
  else {
  	throw domain_error("No particle specified in the file name");
  }
  
  for(int ev = 0; ev < scatteringAnglesTree->GetEntries(); ev++){
  	for(int i = 0; i < NofLayers; i++){
  		histo1[0][i]->Fill(ScatteringAngle[ev][i]);
  	}
  }
  
  for(int ev = 0; ev < EnergyTree->GetEntries(); ev++){
	for(int i = 0; i < NofLayers; i++){
		histo1[1][i]->Fill(Energy[ev][i]);
	}
  }
  
  /*
  double MomBin1[2] = {Npbar[0],Npbar[6]};
  double MomErr1[2] = {sigma_pbar[0],sigma_pbar[6]};
  double Theta[2] = {histo2[0]->GetYaxis()->GetBinCenter(1),histo2[0]->GetYaxis()->GetBinCenter(2)};
  double ThetaErr[2] = {(histo2[0]->GetYaxis()->GetBinWidth(1))/2,(histo2[0]->GetYaxis()->GetBinWidth(2))/2};
  TGraphErrors *graph_theta[1];
  graph_theta[0] = new TGraphErrors(2, Theta, MomBin1, ThetaErr, MomErr1);
  */
  TFitResultPtr r0[NofLayers];
  TFitResultPtr r1[NofLayers];
  for(int i = 0; i < NofLayers; i++){
	new TCanvas();
	r0[i] = histo1[0][i]->Fit("gaus", "S");
	histo1[0][i]->DrawCopy("hist"); // scattering angles

	new TCanvas();
	r1[i] = histo1[1][i]->Fit("gaus", "S");
	histo1[1][i]->DrawCopy("hist"); // energy
  }
  
  double matBudget = 0;
  int z = 1; //proton/electron
  double thetaRMS;
  double thetaRMS_err;
  double energy;
  double energySD;
  double k;
  
  cout<<endl<<"==============================Silicio (Alpide)=============================="<<endl;
  //double thetaRMS = r0[0]->Parameter(2);
  thetaRMS = histo1[0][0]->GetStdDev();
  //double thetaRMS_err = r0[0]->ParError(2);
  thetaRMS_err = histo1[0][0]->GetStdDevError();
  
  energy = r1[0]->Parameter(1);
  energySD = r1[0]->Parameter(2);
  
  double alpideThickness = 0.005; //cm
  k = 13.6 * energy / (energy*energy - mass*mass) * z; //energy in MeV
  
  cout<<endl<<"------------------------------"<<endl;
  cout<<"Contributo 1 a thetaRMS = "<<13.6*z*sqrt(alpideThickness/9.369)*energy/(energy*energy - mass*mass)<<endl;
  cout<<"Contributo 2 a thetaRMS = "<<13.6*z*sqrt(alpideThickness/9.369)*energy/(energy*energy - mass*mass)*0.038*log(alpideThickness/9.369)<<endl;
  cout<<"Theorical RMS angle = "<<13.6*z*sqrt(alpideThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(alpideThickness/9.369))<<" rad"<<endl;
  cout<<"------------------------------"<<endl;
  //thetaRMS = 13.6*z*sqrt(alpideThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(alpideThickness/9.369));
  
  /*
  matBudget = pow(thetaRMS/(2*0.038*k*lambert_w0(pow(e,(0.5/0.038))*thetaRMS/(0.038*k))), 2);
  cout<<endl<<"---Usando pBc = (E^2-m^2)/E---"<<endl;
  cout<<"-------Highland formula-------"<<endl;
  cout<<"thetaRMS = "<<thetaRMS<<" rad"<<endl;
  cout<<"energy = "<<energy<<" MeV"<<endl;
  cout<<"k = "<<k<<endl;
  cout<<"Lambert("<<pow(e,(0.5/0.038))*thetaRMS/(0.038*k)<<") = "<<lambert_w0(pow(e,(0.5/0.038))*thetaRMS/(0.038*k))<<endl;
  cout<<"X/X0 = "<<matBudget*100<<" %"<<endl;
  cout<<"X0 = "<<alpideThickness/matBudget<<" cm"<<endl;
  cout<<"------------------------------"<<endl;
  */
  
  matBudget = pow(thetaRMS/(2*0.038*k*lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))), 2);
  cout<<endl<<"---Usando pBc = (E^2-m^2)/E---"<<endl;
  cout<<"-------Highland formula-------"<<endl;
  cout<<"thetaRMS = "<<thetaRMS<<" rad"<<endl;
  cout<<"energy = "<<energy<<" MeV"<<endl;
  cout<<"k = "<<k<<endl;
  cout<<"Lambert("<<0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k)<<") = "<<lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))<<endl;
  cout<<"X/X0 = "<<matBudget*100<<" %"<<endl;
  cout<<"X0 = "<<alpideThickness/matBudget<<" cm"<<endl;
  cout<<"------------------------------"<<endl<<endl;
  cout<<"============================================================================"<<endl;
  
  cout<<endl<<"==============================Epoxy Glue===================================="<<endl;
  //double thetaRMS = r0[1]->Parameter(2);
  thetaRMS = histo1[0][1]->GetStdDev();
  //double thetaRMS_err = r0[1]->ParError(2);
  thetaRMS_err = histo1[0][1]->GetStdDevError();
  
  energy = r1[1]->Parameter(1);
  energySD = r1[1]->Parameter(2);
  
  double glueThickness = 0.0025; //cm
  k = 13.6 * energy / (energy*energy - mass*mass) * z; //energy in MeV
  
  cout<<endl<<"------------------------------"<<endl;
  cout<<"Contributo 1 a thetaRMS = "<<13.6*z*sqrt(glueThickness/9.369)*energy/(energy*energy - mass*mass)<<endl;
  cout<<"Contributo 2 a thetaRMS = "<<13.6*z*sqrt(glueThickness/9.369)*energy/(energy*energy - mass*mass)*0.038*log(glueThickness/9.369)<<endl;
  cout<<"Theorical RMS angle = "<<13.6*z*sqrt(glueThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(glueThickness/9.369))<<" rad"<<endl;
  cout<<"------------------------------"<<endl;
  //thetaRMS = 13.6*z*sqrt(glueThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(glueThickness/9.369));
  
  matBudget = pow(thetaRMS/(2*0.038*k*lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))), 2);
  cout<<endl<<"---Usando pBc = (E^2-m^2)/E---"<<endl;
  cout<<"-------Highland formula-------"<<endl;
  cout<<"thetaRMS = "<<thetaRMS<<" rad"<<endl;
  cout<<"energy = "<<energy<<" MeV"<<endl;
  cout<<"k = "<<k<<endl;
  cout<<"Lambert("<<0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k)<<") = "<<lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))<<endl;
  cout<<"X/X0 = "<<matBudget*100<<" %"<<endl;
  cout<<"X0 = "<<glueThickness/matBudget<<" cm"<<endl;
  cout<<"------------------------------"<<endl<<endl;
  cout<<"============================================================================"<<endl;
  
  cout<<endl<<"==============================Kapton========================================="<<endl;
  //double thetaRMS = r0[2]->Parameter(2);
  thetaRMS = histo1[0][2]->GetStdDev();
  //double thetaRMS_err = r0[2]->ParError(2);
  thetaRMS_err = histo1[0][2]->GetStdDevError();
  
  energy = r1[2]->Parameter(1);
  energySD = r1[2]->Parameter(2);
  
  double kaptonThickness = 0.005; //cm
  k = 13.6 * energy / (energy*energy - mass*mass) * z; //energy in MeV
  
  cout<<endl<<"------------------------------"<<endl;
  cout<<"Contributo 1 a thetaRMS = "<<13.6*z*sqrt(kaptonThickness/9.369)*energy/(energy*energy - mass*mass)<<endl;
  cout<<"Contributo 2 a thetaRMS = "<<13.6*z*sqrt(kaptonThickness/9.369)*energy/(energy*energy - mass*mass)*0.038*log(kaptonThickness/9.369)<<endl;
  cout<<"Theorical RMS angle = "<<13.6*z*sqrt(kaptonThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(kaptonThickness/9.369))<<" rad"<<endl;
  cout<<"------------------------------"<<endl;
  //thetaRMS = 13.6*z*sqrt(kaptonThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(kaptonThickness/9.369));
  
  matBudget = pow(thetaRMS/(2*0.038*k*lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))), 2);
  cout<<endl<<"---Usando pBc = (E^2-m^2)/E---"<<endl;
  cout<<"-------Highland formula-------"<<endl;
  cout<<"thetaRMS = "<<thetaRMS<<" rad"<<endl;
  cout<<"energy = "<<energy<<" MeV"<<endl;
  cout<<"k = "<<k<<endl;
  cout<<"Lambert("<<0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k)<<") = "<<lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))<<endl;
  cout<<"X/X0 = "<<matBudget*100<<" %"<<endl;
  cout<<"X0 = "<<kaptonThickness/matBudget<<" cm"<<endl;
  cout<<"------------------------------"<<endl<<endl;
  cout<<"============================================================================"<<endl;
  
  cout<<endl<<"==============================Copper========================================="<<endl;
  //double thetaRMS = r0[3]->Parameter(2);
  thetaRMS = histo1[0][3]->GetStdDev();
  //double thetaRMS_err = r0[3]->ParError(2);
  thetaRMS_err = histo1[0][3]->GetStdDevError();
  
  energy = r1[3]->Parameter(1);
  energySD = r1[3]->Parameter(2);
  
  double copperThickness = 0.0005; //cm
  k = 13.6 * energy / (energy*energy - mass*mass) * z; //energy in MeV
  
  cout<<endl<<"------------------------------"<<endl;
  cout<<"Contributo 1 a thetaRMS = "<<13.6*z*sqrt(copperThickness/9.369)*energy/(energy*energy - mass*mass)<<endl;
  cout<<"Contributo 2 a thetaRMS = "<<13.6*z*sqrt(copperThickness/9.369)*energy/(energy*energy - mass*mass)*0.038*log(copperThickness/9.369)<<endl;
  cout<<"Theorical RMS angle = "<<13.6*z*sqrt(copperThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(copperThickness/9.369))<<" rad"<<endl;
  cout<<"------------------------------"<<endl;
  //thetaRMS = 13.6*z*sqrt(copperThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(copperThickness/9.369));
  
  matBudget = pow(thetaRMS/(2*0.038*k*lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))), 2);
  cout<<endl<<"---Usando pBc = (E^2-m^2)/E---"<<endl;
  cout<<"-------Highland formula-------"<<endl;
  cout<<"thetaRMS = "<<thetaRMS<<" rad"<<endl;
  cout<<"energy = "<<energy<<" MeV"<<endl;
  cout<<"k = "<<k<<endl;
  cout<<"Lambert("<<0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k)<<") = "<<lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))<<endl;
  cout<<"X/X0 = "<<matBudget*100<<" %"<<endl;
  cout<<"X0 = "<<copperThickness/matBudget<<" cm"<<endl;
  cout<<"------------------------------"<<endl<<endl;
  cout<<"============================================================================"<<endl;
  
  cout<<endl<<"==============================PCB============================================"<<endl;
  //double thetaRMS = r0[4]->Parameter(2);
  thetaRMS = histo1[0][4]->GetStdDev();
  //double thetaRMS_err = r0[4]->ParError(2);
  thetaRMS_err = histo1[0][4]->GetStdDevError();
  
  energy = r1[4]->Parameter(1);
  energySD = r1[4]->Parameter(2);
  
  double pcbThickness = 3*0.005; //cm
  k = 13.6 * energy / (energy*energy - mass*mass) * z; //energy in MeV
  
  cout<<endl<<"------------------------------"<<endl;
  cout<<"Contributo 1 a thetaRMS = "<<13.6*z*sqrt(pcbThickness/9.369)*energy/(energy*energy - mass*mass)<<endl;
  cout<<"Contributo 2 a thetaRMS = "<<13.6*z*sqrt(pcbThickness/9.369)*energy/(energy*energy - mass*mass)*0.038*log(pcbThickness/9.369)<<endl;
  cout<<"Theorical RMS angle = "<<13.6*z*sqrt(pcbThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(pcbThickness/9.369))<<" rad"<<endl;
  cout<<"------------------------------"<<endl;
  //thetaRMS = 13.6*z*sqrt(pcbThickness/9.369)*energy/(energy*energy - mass*mass)*(1+0.038*log(pcbThickness/9.369));
  
  matBudget = pow(thetaRMS/(2*0.038*k*lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))), 2);
  cout<<endl<<"---Usando pBc = (E^2-m^2)/E---"<<endl;
  cout<<"-------Highland formula-------"<<endl;
  cout<<"thetaRMS = "<<thetaRMS<<" rad"<<endl;
  cout<<"energy = "<<energy<<" MeV"<<endl;
  cout<<"k = "<<k<<endl;
  cout<<"Lambert("<<0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k)<<") = "<<lambert_w0(0.5*pow(e, 1.0/0.038)*sqrt(pow(e, -1.0/0.038)*0.038*0.038*k*k/(thetaRMS*thetaRMS))*(thetaRMS*thetaRMS)/(0.038*0.038*k*k))<<endl;
  cout<<"X/X0 = "<<matBudget*100<<" %"<<endl;
  cout<<"X0 = "<<pcbThickness/matBudget<<" cm"<<endl;
  cout<<"------------------------------"<<endl<<endl;
  cout<<"============================================================================"<<endl;
  
  TODO Per lunedì: calcolare l'errore su X/X0 con formula di propagazione errori
  
  file1->Close();
  
}



