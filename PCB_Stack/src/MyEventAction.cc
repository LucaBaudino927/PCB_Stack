#include "MyEventAction.hh"

// ######## Constructor
MyEventAction::MyEventAction(MySteppingAction* steppingAction){
	
	// set printing per each event
	//G4RunManager::GetRunManager()->SetPrintProgress(1);
	fHitCollID.push_back(-1);
	fSteppingAction = steppingAction;
	
}

// ######## Start of Event -> call MySensitiveDetector::Initialize() and then MyEventAction::BeginOfEventAction()
void MyEventAction::BeginOfEventAction(const G4Event*){

	fSteppingAction->SetNewEventFlag();
		
	detHCName.clear();
	
	if(StaticInfo::GetDetectorFlag("SingleLayer") || StaticInfo::GetDetectorFlag("StackUp")){
		auto LVStore = G4LogicalVolumeStore::GetInstance();
		for(logicalVolumeIterator i = LVStore->begin(); i != LVStore->end(); i++){
			if((*i)->GetName().compare("logicWorld") == 0) continue;
			detHCName.push_back((*i)->GetName()+"/MySensitiveDetectorColl");
		}
	}
	
	fHitCollID.clear();
	fHitCollID.push_back(-1);
	
	
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
	
}


void MyEventAction::EndOfEventAction(const G4Event* anEvent){	
	
	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	
	//chiudo le ntuple riempite in MySteppingAction
	analysisManager->AddNtupleRow(2);
	analysisManager->AddNtupleRow(3);

	// MySensitiveDetector hits
	G4double totalEnergyDep = 0.;
	for (G4int iDet = 0; iDet < detHCName.size(); ++iDet) {
		auto hc = GetHC(anEvent, fHitCollID[iDet]);
		
		if (!hc) return;
		for (unsigned long i = 0; i < hc->GetSize(); ++i) {
			G4double edep = 0.;
			// The EM and Had calorimeter hits are of different types
			auto hit = static_cast<MySensitiveDetectorHit*>(hc->GetHit(i));
			totalEnergyDep += hit->GetEdep();
			//G4cout << "---EndOfEventAction---Edep: " << hit->GetEdep()/MeV << " MeV" << G4endl;
		}
	}
	if(totalEnergyDep != 0.) analysisManager->FillNtupleDColumn(0, 0, totalEnergyDep);
	analysisManager->AddNtupleRow(1);
	
	
	G4double sum = 0;
	std::vector<G4double> R_X0_Vector = fSteppingAction->GetRX0Vector();
	for(int i = 0; i < R_X0_Vector.size(); i++){
		sum += R_X0_Vector[i];
	}
	analysisManager->FillNtupleDColumn(0, 0, sum);
	analysisManager->AddNtupleRow(0);
	
	analysisManager->FillH2(0, fSteppingAction->GetBeamXPosition(), fSteppingAction->GetBeamYPosition(), sum);
	analysisManager->FillH2(1, fSteppingAction->GetBeamXPosition(), fSteppingAction->GetBeamYPosition(), 1);


	// print --------------------------------------------------------------------------------------------------------
	if(!StaticInfo::GetDetectorFlag("verboseDetConstruction")) return;
	
	auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
	if (printModulo == 0 || anEvent->GetEventID() % printModulo != 0) return;

	auto primary = anEvent->GetPrimaryVertex(0)->GetPrimary(0);
	G4cout << G4endl << ">>> Event " << anEvent->GetEventID() << " >>> Simulation truth : " << primary->GetG4code()->GetParticleName() << " "
															 << primary->GetMomentum() << G4endl;
	
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
