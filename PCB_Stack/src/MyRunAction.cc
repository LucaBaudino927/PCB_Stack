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
	
	//se L = 10 mm -> 100 bin mi danno bin da 100 um
	G4int id = analysisManager->CreateH2("Material Budget XY", "Material Budget XY",  200, -5, 5, 200, -5, 5);
	//analysisManager->SetH2Title(G4int id, const G4String& title);
	analysisManager->SetH2XAxisTitle(id, "X [mm]");
	analysisManager->SetH2YAxisTitle(id, "Y [mm]");
	analysisManager->SetH2ZAxisTitle(id, "X/X0");
	
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


