#include "MyEventAction.hh"

// ######## Constructor
MyEventAction::MyEventAction(){

	// set printing per each event
	//G4RunManager::GetRunManager()->SetPrintProgress(1);
	fHitCollID.push_back(-1);
	fPCBHitCollID.push_back(-1);
	
	// hits collections names
	detHCName = { { "Alpide/MySensitiveDetectorColl" } };
	if(StaticInfo::GetDetectorFlag("constructAlpidePads")) {
		for(G4int i = 0; i < NofPads; i++){
			detHCName.push_back("logicPad1_"+std::to_string(i)+"/MySensitiveDetectorColl");
			detHCName.push_back("logicPad2_"+std::to_string(i)+"/MySensitiveDetectorColl");
		}
	}
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) detHCName.push_back("LowerGlue/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) detHCName.push_back("UpperGlue/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer")) detHCName.push_back("LowerKapton/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer")) detHCName.push_back("UpperKapton/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructCopperLayer")) detHCName.push_back("CopperLayer/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructSolderBalls")) {
		for(G4int i = 0; i < NofPads; i++){
			detHCName.push_back("logicSolderBall_"+std::to_string(i)+"/MySensitiveDetectorColl");
		}
	}
	if(StaticInfo::GetDetectorFlag("constructPCB")){ 
		PCBHCName.push_back("PCB/PCBSensitiveDetectorColl");
	}
	
}

// ######## Start of Event -> call MySensitiveDetector::Initialize() and then MyEventAction::BeginOfEventAction()
void MyEventAction::BeginOfEventAction(const G4Event*){ 
	
	
	fPCBedep = 0.;
	/*
	auto analysisManager = G4AnalysisManager::Instance();
	if(fHitCollID[0] < 0){
		auto sdManager = G4SDManager::GetSDMpointer();	
		// hits collections ID
		G4cout << "------MapsFoilDetectorList::GetLogicalDetectorList().size(): " << MapsFoilDetectorList::GetLogicalDetectorList().size() << G4endl;
		for(int i = 0; i < MapsFoilDetectorList::GetLogicalDetectorList().size(); ++i){			  	 //nomeSD/nomeColl
			fHitCollID[i] = sdManager->GetCollectionID(MapsFoilDetectorList::GetLogicalDetectorList()[i]->GetName()+"/MySensitiveDetectorColl");
			G4cout << "Indexing collection ID: " << fHitCollID[i] << G4endl;
		}
		G4int count = 0;
		for(int i = 0; i < NofMySensitiveDetector; ++i){
			for(G4int j = 0; j < NofObservables; ++j){
				fMySensitiveDetectorHistoID[i][j] = analysisManager->GetH1Id(fRunAction->GetHistoNameByIndex(count));
				G4cout<< "---fRunAction->GetHistoNameByIndex("<<count<<") = "<<fRunAction->GetHistoNameByIndex(count)<<G4endl;
				count++;
			}
		}
	}*/
	
		
	detHCName.clear();
	PCBHCName.clear();
	
	detHCName = { { "Alpide/MySensitiveDetectorColl" } };
	if(StaticInfo::GetDetectorFlag("constructAlpidePads")) {
		for(G4int i = 0; i < NofPads; i++){
			detHCName.push_back("logicPad1_"+std::to_string(i)+"/MySensitiveDetectorColl");
			detHCName.push_back("logicPad2_"+std::to_string(i)+"/MySensitiveDetectorColl");
		}
	}
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) detHCName.push_back("LowerGlue/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) detHCName.push_back("UpperGlue/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer")) detHCName.push_back("LowerKapton/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructKaptonLayer")) detHCName.push_back("UpperKapton/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructCopperLayer")) detHCName.push_back("CopperLayer/MySensitiveDetectorColl");
	if(StaticInfo::GetDetectorFlag("constructSolderBalls")) {
		for(G4int i = 0; i < NofPads; i++){
			detHCName.push_back("logicSolderBall_"+std::to_string(i)+"/MySensitiveDetectorColl");
		}
	}
	if(StaticInfo::GetDetectorFlag("constructPCB")){ 
		PCBHCName.push_back("PCB/PCBSensitiveDetectorColl");
	}
	
	fHitCollID.clear();
	fHitCollID.push_back(-1);
	fPCBHitCollID.clear();
	fPCBHitCollID.push_back(-1);
	
	
	if (fHitCollID[0] < 0) {
		auto sdManager = G4SDManager::GetSDMpointer();
		auto analysisManager = G4AnalysisManager::Instance();
		
		//inizializzo fHitCollID
		fHitCollID.clear();
		
		//riempio fHitCollID
		for (G4int iDet = 0; iDet < detHCName.size(); ++iDet) {
			// hit collections IDs
			fHitCollID.push_back(sdManager->GetCollectionID(detHCName[iDet]));
		}
  	}
  	
  	
  	if (fPCBHitCollID[0] < 0) {
		auto sdManager = G4SDManager::GetSDMpointer();
		auto analysisManager = G4AnalysisManager::Instance();
		
		//inizializzo fPCBHitCollID
		fPCBHitCollID.clear();

		for (G4int iDet = 0; iDet < PCBHCName.size(); ++iDet) {
			// hit collections IDs
			fPCBHitCollID.push_back(sdManager->GetCollectionID(PCBHCName[iDet]));
		}
    
  	}
	
}


void MyEventAction::EndOfEventAction(const G4Event* anEvent){
	
	/*G4SDManager* SDmanager = G4SDManager::GetSDMpointer();
	G4HCofThisEvent* hitsCollections = anEvent->GetHCofThisEvent();	
	for (G4int iDet = 0; iDet < NofMySensitiveDetector; ++iDet) {
		MySensitiveDetectorHitsCollection* hitCollection 
			= static_cast<MySensitiveDetectorHitsCollection*>(hitsCollections->GetHC(fHitCollID[iDet]));
		//G4cout << "---fHitCollID["<<iDet<<"]" << " = " << fHitCollID[iDet] <<G4endl;
		//G4cout << "---fHitCollID.size(): " << fHitCollID.size() << G4endl;
		if (!hitCollection) return;
		for(unsigned int i = 0; i < hitCollection->GetSize(); ++i){
			MySensitiveDetectorHit* aHit = static_cast<MySensitiveDetectorHit*> (hitCollection->GetHit(i));
			if (!aHit) continue;
			aHit->Print();
			//TH1
			//G4cout<< "---fMySensitiveDetectorHistoID["<<iDet<<"][0] = "<<fMySensitiveDetectorHistoID[iDet][0]<<G4endl;
			//G4cout<< "---fMySensitiveDetectorHistoID["<<iDet<<"][1] = "<<fMySensitiveDetectorHistoID[iDet][1]<<G4endl;
			//G4cout<< "---fMySensitiveDetectorHistoID["<<iDet<<"][2] = "<<fMySensitiveDetectorHistoID[iDet][2]<<G4endl;
			//analysisManager->FillH1(fMySensitiveDetectorHistoID[iDet][0], aHit->GetTime());
			analysisManager->FillH1(fMySensitiveDetectorHistoID[iDet][1], aHit->GetPos().z());
			analysisManager->FillH1(fMySensitiveDetectorHistoID[iDet][2], aHit->GetEdep());
			//TH2
			//analysisManager->FillH2(fH2ID[0], aHit->GetPos().x(), aHit->GetPos().y());
		}
		
	}*/
	

	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	
	//chiudo le ntuple riempite in MySteppingAction
	analysisManager->AddNtupleRow(2);
	analysisManager->AddNtupleRow(3);
	analysisManager->AddNtupleRow(4);

	// MySensitiveDetector hits
	std::vector<G4int> totalDetHit;
	std::vector<G4double> totalDetEdep;
	//loop over all MAPS volumes (this loop excludes PCB)
	for (G4int iDet = 0; iDet < detHCName.size(); ++iDet) {
		auto hc = GetHC(anEvent, fHitCollID[iDet]);
		
		if (!hc) return;
		totalDetHit.push_back(0);
		totalDetEdep.push_back(0.);
		for (unsigned long i = 0; i < hc->GetSize(); ++i) {
			G4double edep = 0.;
			// The EM and Had calorimeter hits are of different types
			auto hit = static_cast<MySensitiveDetectorHit*>(hc->GetHit(i));
			edep = hit->GetEdep();
			//G4cout << "---EndOfEventAction---Edep: " << edep/MeV << " MeV" << G4endl;
			if (edep > 0.) {
				totalDetHit[iDet]++;
				totalDetEdep[iDet] += edep;
			}
		}
		// columns 2, 3
		if(totalDetEdep[iDet] != 0.) analysisManager->FillNtupleDColumn(0, iDet, totalDetEdep[iDet]);
	}
	analysisManager->AddNtupleRow(0);
	
	
	//loop over all PCB volumes (this loop excludes MAPS)
	for (G4int iDet = 0; iDet < PCBHCName.size(); ++iDet) {
		auto hc = GetHC(anEvent, fPCBHitCollID[iDet]);
		if (!hc) return;
		
		//totalDetHit[iDet] = fPCBedep;
		//G4cout<<"---EndOfEvent()---iDet: "<<iDet<<" fPCBedep: "<<fPCBedep<<G4endl;
		if(fPCBedep != 0.) analysisManager->FillNtupleDColumn(1, iDet, fPCBedep);
	}
	if(PCBHCName.size() > 0) analysisManager->AddNtupleRow(1);


	// print --------------------------------------------------------------------------------------------------------
	if(!StaticInfo::GetDetectorFlag("verboseDetConstruction")) return;
	
	auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
	if (printModulo == 0 || anEvent->GetEventID() % printModulo != 0) return;

	auto primary = anEvent->GetPrimaryVertex(0)->GetPrimary(0);
	G4cout << G4endl << ">>> Event " << anEvent->GetEventID() << " >>> Simulation truth : " << primary->GetG4code()->GetParticleName() << " "
															 << primary->GetMomentum() << G4endl;

	for (G4int iDet = 0; iDet < detHCName.size(); ++iDet) {
		G4cout << detHCName[iDet] << " has " << totalDetHit[iDet] << " hits." << " Total Edep is " << totalDetEdep[iDet] / MeV << " (MeV)" << G4endl;
	}

	
}


// Utility function which finds a hit collection with the given Id
// and print warnings if not found
G4VHitsCollection* MyEventAction::GetHC(const G4Event* event, G4int collId) {
	auto hce = event->GetHCofThisEvent();
	if (!hce) {
		G4ExceptionDescription msg;
		msg << "No hits collection of this event found." << G4endl;
		G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
		return nullptr;
	}

	auto hc = hce->GetHC(collId);
	if (!hc) {
		G4ExceptionDescription msg;
		msg << "Hits collection " << collId << " of this event not found." << G4endl;
		G4Exception("EventAction::EndOfEventAction()", "Code001", JustWarning, msg);
	}
	return hc;
}
