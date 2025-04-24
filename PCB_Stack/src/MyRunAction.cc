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
	// inizializzate in MyDetectorConstruction (e prese dal file di configurazione)
	analysisManager->CreateNtuple("MapsFoil_EnergyDepositionPerEvent", "EnergyDepositionPerEvent");
	analysisManager->CreateNtupleDColumn("AlpideEnergy");  						// column Id = 0
	if(StaticInfo::GetDetectorFlag("constructAlpidePads")) {
		for(G4int i = 0; i < NofPads; i++){
			analysisManager->CreateNtupleDColumn("AlpidePad1Energy_"+std::to_string(i));  	// column Id: from 1 to 1 + (#pad - 1)*2
			analysisManager->CreateNtupleDColumn("AlpidePad2Energy_"+std::to_string(i));  	// column Id: from 2 to 2 + (#pad - 1)*2
		}
	}
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) {
		analysisManager->CreateNtupleDColumn("LowerGlueEnergy");  				// column Id: from 3 to 3 + (#pad - 1)*2
		analysisManager->CreateNtupleDColumn("UpperGlueEnergy");  				// column Id: from 4 to 4 + (#pad - 1)*2
	}
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer")) {
		analysisManager->CreateNtupleDColumn("LowerKaptonEnergy");  				// column Id: from 5 to 5 + (#pad - 1)*2
		analysisManager->CreateNtupleDColumn("UpperKaptonEnergy");  				// column Id: from 6 to 6 + (#pad - 1)*2
	}
	if(StaticInfo::GetDetectorFlag("constructCopperLayer")) {
		analysisManager->CreateNtupleDColumn("CopperEnergy");  					// column Id: from 7 to 7 + (#pad - 1)*2
	}
	if(StaticInfo::GetDetectorFlag("constructSolderBalls")) {
		for(G4int i = 0; i < NofPads; i++){
			analysisManager->CreateNtupleDColumn("SolderBallEnergy_"+std::to_string(i));	// column Id: from 8 to 8 + (#pad - 1)*3
		}
	}
	analysisManager->FinishNtuple(0);
	
	//Energy deposition for each event
	analysisManager->CreateNtuple("PCB_EnergyDepositionPerEvent", "PCB_EnergyDepositionPerEvent");
	if(StaticInfo::GetDetectorFlag("constructPCB")) { 
		analysisManager->CreateNtupleDColumn("fPCBedep");
	}
	analysisManager->FinishNtuple(1);
	
	//Scattering Angle Distribution in each volume for a fixed particle (proton)
	analysisManager->CreateNtuple("ScatteringAngles", "ScatteringAngles");
	analysisManager->CreateNtupleDColumn("Alpide");
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) analysisManager->CreateNtupleDColumn("LowerGlue");
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) analysisManager->CreateNtupleDColumn("UpperGlue");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer"))    analysisManager->CreateNtupleDColumn("LowerKapton");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer"))    analysisManager->CreateNtupleDColumn("UpperKapton");
	if(StaticInfo::GetDetectorFlag("constructCopperLayer"))    analysisManager->CreateNtupleDColumn("Copper");
	if(StaticInfo::GetDetectorFlag("constructPCB")) 	   analysisManager->CreateNtupleDColumn("PCB");
	analysisManager->FinishNtuple(2);
	
	//Energy Distribution of particles entrying in a volume for a fixed particle (proton)
	analysisManager->CreateNtuple("Energy before volume", "Energy before volume");
	analysisManager->CreateNtupleDColumn("Alpide");
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) analysisManager->CreateNtupleDColumn("LowerGlue");
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) analysisManager->CreateNtupleDColumn("UpperGlue");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer"))    analysisManager->CreateNtupleDColumn("LowerKapton");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer"))    analysisManager->CreateNtupleDColumn("UpperKapton");
	if(StaticInfo::GetDetectorFlag("constructCopperLayer"))    analysisManager->CreateNtupleDColumn("Copper");
	if(StaticInfo::GetDetectorFlag("constructPCB")) 	   analysisManager->CreateNtupleDColumn("PCB");
	analysisManager->FinishNtuple(3);
	
	//Energy Distribution of particles entrying in a volume for a fixed particle (proton)
	analysisManager->CreateNtuple("DeltaE in volume", "DeltaE in volume");
	analysisManager->CreateNtupleDColumn("Alpide");
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) analysisManager->CreateNtupleDColumn("LowerGlue");
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) analysisManager->CreateNtupleDColumn("UpperGlue");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer"))    analysisManager->CreateNtupleDColumn("LowerKapton");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer"))    analysisManager->CreateNtupleDColumn("UpperKapton");
	if(StaticInfo::GetDetectorFlag("constructCopperLayer"))    analysisManager->CreateNtupleDColumn("Copper");
	if(StaticInfo::GetDetectorFlag("constructPCB")) 	   analysisManager->CreateNtupleDColumn("PCB");
	analysisManager->FinishNtuple(4);
	
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


