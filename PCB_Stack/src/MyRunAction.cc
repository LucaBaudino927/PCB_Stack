#include "MyRunAction.hh"

// ######## Costructor
MyRunAction::MyRunAction() {

	//G4cout<<"---MyRunAction---"<<G4endl;
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	//analysisManager->SetDefaultFileType("root");
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetNtupleMerging(true);
	//analysisManager->SetNtupleDirectoryName("output");
	//analysisManager->SetHistoDirectoryName("output");
	//analysisManager->SetFileName("output");

	DefineCommands();
            
}


// ######## Begin of the run
void MyRunAction::BeginOfRunAction(const G4Run* run){
	
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	
	//----------------------------------------------------------------------------------------------------------------
	// Ad ogni Run reinizializzo l'analisysManager in modo
	// che, se reinizializzo la geometria e modifico il 
	// numero di detector, posso modificare anche il numero di ntuple
	analysisManager->Clear();
	
	//----------------------------------------------------------------------------------------------------------------
	// Apro i file di output assegnando un nome in base al RunID.
	// NB: Il main analysis file viene creato nel master e i file contenenti i dati vengono creati per ogni thread.
	//     In generale il RunID del Master Thread è diverso dal RunID dei vari thread, quindi devo salvare il RunID
	//     del master per fare in modo che i vari thread creino i loro file con lo stesso nome altrimenti non 
	//     funziona il merging delle ntuple.
	//     Es: Master thread con RunID = 0 crea output0.root, thread con RunID = X deve creare un output0_tX.root per funzionare correttamente
	std::stringstream strRunID;
	if(IsMaster()) {
		StaticInfo::SetRunIdOnMasterThread(run->GetRunID());
		StaticInfo::SetScanWidth(fScanWidth);
		StaticInfo::SetBeamYPosition(Y_BeamPosition);
		StaticInfo::SetBeamXPosition(X_BeamPosition);
	}
	//G4cout<<"---MyRunAction---run->GetRunID(): "<<run->GetRunID()<<G4endl;
	//G4cout<<"---MyRunAction---StaticInfo::GetRunIdOnMasterThread(): "<<StaticInfo::GetRunIdOnMasterThread()<<G4endl;
    strRunID << StaticInfo::GetRunIdOnMasterThread();
	analysisManager->SetFileName("output"+strRunID.str()+".root");
	analysisManager->SetNtupleFileName("output"+strRunID.str()+".root");
	
	// Reset histograms from previous run
	analysisManager->Reset();
	
	analysisManager->OpenFile("output"+strRunID.str()+".root");
	
	//----------------------------------------------------------------------------------------------------------------
	// Creo le ntuple in base alle flag di costruzione dei detector

	// X/X0
	analysisManager->CreateNtuple("Material Budget", "Material Budget");
	analysisManager->CreateNtupleDColumn("MaterialBudget");
	analysisManager->FinishNtuple(0);
	
	// inizializzate in MyDetectorConstruction (e prese dal file di configurazione)
	analysisManager->CreateNtuple("EnergyDepositionPerEvent", "EnergyDepositionPerEvent");
	analysisManager->CreateNtupleDColumn("TotalEnergyDeposition");
	analysisManager->FinishNtuple(1);

	//Scattering Angle Distribution in each volume for a fixed particle (proton)
	analysisManager->CreateNtuple("ScatteringAngles", "ScatteringAngles");
	analysisManager->CreateNtupleDColumn("TotalScatteringAngle");
	analysisManager->FinishNtuple(2);

	//Energy Distribution of particles entrying in a volume for a fixed particle (proton)
	analysisManager->CreateNtuple("Energy before volume", "Energy before volume");
	analysisManager->CreateNtupleDColumn("PrimaryBeamEnergy");			
	analysisManager->FinishNtuple(3);
	
	if(IsMaster()){
		//se L = 10 mm -> 100 bin mi danno bin da 100 um
		//standard L = 10 mm divisi in 200 bin da -5 a +5 -> bin da 50x50 um
		G4int id = analysisManager->CreateH2("Material Budget XY", "Material Budget XY",  fScanWidth/0.05, 
																						-fScanWidth/2. + X_BeamPosition, 
																						fScanWidth/2. + X_BeamPosition, 
																						fScanWidth/0.05, 
																						-fScanWidth/2. + Y_BeamPosition,
																						fScanWidth/2. + Y_BeamPosition);
		//analysisManager->SetH2Title(G4int id, const G4String& title);
		analysisManager->SetH2XAxisTitle(id, "X [mm]");
		analysisManager->SetH2YAxisTitle(id, "Y [mm]");
		analysisManager->SetH2ZAxisTitle(id, "X/X0");
	}else{
		//se L = 10 mm -> 100 bin mi danno bin da 100 um
		//standard L = 10 mm divisi in 200 bin da -5 a +5 -> bin da 50x50 um
		G4int id = analysisManager->CreateH2("Material Budget XY", "Material Budget XY",  StaticInfo::GetScanWidth()/0.05, 
																						-StaticInfo::GetScanWidth()/2. + StaticInfo::GetBeamXPosition(), 
																						StaticInfo::GetScanWidth()/2. + StaticInfo::GetBeamXPosition(), 
																						StaticInfo::GetScanWidth()/0.05, 
																						-StaticInfo::GetScanWidth()/2. + StaticInfo::GetBeamYPosition(), 
																						StaticInfo::GetScanWidth()/2. + StaticInfo::GetBeamYPosition());
		//analysisManager->SetH2Title(G4int id, const G4String& title);
		analysisManager->SetH2XAxisTitle(id, "X [mm]");
		analysisManager->SetH2YAxisTitle(id, "Y [mm]");
		analysisManager->SetH2ZAxisTitle(id, "X/X0");
	}
	
	if(IsMaster()){
		//se L = 10 mm -> 100 bin mi danno bin da 100 um
		//standard L = 10 mm divisi in 200 bin da -5 a +5 -> bin da 50x50 um
		G4int id = analysisManager->CreateH2("Number of Events XY", "Number of Events XY",  fScanWidth/0.05, 
																				  -fScanWidth/2. + X_BeamPosition, 
																				  fScanWidth/2. + X_BeamPosition, 
																				  fScanWidth/0.05, 
																				  -fScanWidth/2. + Y_BeamPosition,
																				  fScanWidth/2. + Y_BeamPosition);
		//analysisManager->SetH2Title(G4int id, const G4String& title);
		analysisManager->SetH2XAxisTitle(id, "X [mm]");
		analysisManager->SetH2YAxisTitle(id, "Y [mm]");
		analysisManager->SetH2ZAxisTitle(id, "NofEvents");
	}else{	
		//se L = 10 mm -> 100 bin mi danno bin da 100 um
		//standard L = 10 mm divisi in 200 bin da -5 a +5 -> bin da 50x50 um
		G4int id = analysisManager->CreateH2("Number of Events XY", "Number of Events XY",  StaticInfo::GetScanWidth()/0.05, 
																				  -StaticInfo::GetScanWidth()/2. + StaticInfo::GetBeamXPosition(), 
																				  StaticInfo::GetScanWidth()/2. + StaticInfo::GetBeamXPosition(), 
																				  StaticInfo::GetScanWidth()/0.05, 
																				  -StaticInfo::GetScanWidth()/2. + StaticInfo::GetBeamYPosition(),
																				  StaticInfo::GetScanWidth()/2. + StaticInfo::GetBeamYPosition());
		//analysisManager->SetH2Title(G4int id, const G4String& title);
		analysisManager->SetH2XAxisTitle(id, "X [mm]");
		analysisManager->SetH2YAxisTitle(id, "Y [mm]");
		analysisManager->SetH2ZAxisTitle(id, "NofEvents");
	}
	
	// Set ntuple output file
	//analysisManager->SetNtupleFileName(0, "output"+strRunID.str()+".root");
	//analysisManager->SetNtupleFileName(1, "output"+strRunID.str()+".root");
	//analysisManager->SetNtupleFileName(2, "output"+strRunID.str()+".root");
	

}

// ######## End of the run
void MyRunAction::EndOfRunAction(const G4Run*){

        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        analysisManager->Write();
        analysisManager->CloseFile(false);

}

void MyRunAction::DefineCommands()
{
	// Define /myRun command directory using generic messenger class
	fMessenger = new G4GenericMessenger(this, "/myRun/", "Custom run commands");
	
	// beamYPosition command
	auto& beamYPositionCmd = fMessenger->DeclarePropertyWithUnit("beamYPosition", "mm", Y_BeamPosition, "Y coordinate of the beam in mm");
	beamYPositionCmd.SetParameterName("Ybp", true);
	//beamYPositionCmd.SetRange("Ybp>=0.");
	beamYPositionCmd.SetDefaultValue("0");
	
	// beamXPosition command
	auto& beamXPositionCmd = fMessenger->DeclarePropertyWithUnit("beamXPosition", "mm", X_BeamPosition, "X coordinate of the beam in mm");
	beamXPositionCmd.SetParameterName("Xbp", true);
	//beamXPositionCmd.SetRange("Xbp>=0.");
	beamXPositionCmd.SetDefaultValue("0");
	
	// scanWidth command
	auto& scanWidthCmd = fMessenger->DeclarePropertyWithUnit("scanWidth", "mm", fScanWidth, "Width of the possible beam positions.");
	scanWidthCmd.SetParameterName("scanW", true);
	scanWidthCmd.SetRange("scanW>=0.");
	scanWidthCmd.SetDefaultValue("10");
  
}



