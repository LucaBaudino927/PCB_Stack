#include "MySteppingAction.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction) { 
	fEventAction = eventAction;
	fFirstMomDirectionInVolume = {0., 0., 0.};
	fLastMomDirectionInVolume = {0., 0., 0.};
	fTrackID = -1;
	fDeltaEnergyStep = 0.;
}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step* step){

	//Salvo la distribuzione degli angoli di scattering (e dell'energia depositata nella PCB) dei soli PROTONI
	G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
	if(particleName.compare("proton") == 0){
	
		G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
		if(volume->GetName().substr(0,8).compare("LV_SOLID") != 0 && volume->GetName().find("World") == std::string::npos) {
			
			auto analysisManager = G4AnalysisManager::Instance();
		
			if(step->IsFirstStepInVolume()) {
			
				fTrackID = step->GetTrack()->GetTrackID();
				fFirstMomDirectionInVolume = step->GetPreStepPoint()->GetMomentumDirection();
				
				G4double entryEnergy = step->GetPreStepPoint()->GetTotalEnergy();
				G4String detectorName = volume->GetName();
				if(detectorName.compare("logicAlpide") == 0) {
					analysisManager->FillNtupleDColumn(3, 0, entryEnergy);
				} else if(detectorName.compare("logicLowerGlue") == 0 && StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) {
					analysisManager->FillNtupleDColumn(3, 1, entryEnergy);
				} else if(detectorName.compare("logicUpperGlue") == 0 && StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) {
					analysisManager->FillNtupleDColumn(3, 2, entryEnergy);
				} else if(detectorName.compare("logicLowerKapton") == 0 && StaticInfo::GetDetectorFlag("constructKaptonLayer")) {
					analysisManager->FillNtupleDColumn(3, 3, entryEnergy);
				} else if(detectorName.compare("logicUpperKapton") == 0 && StaticInfo::GetDetectorFlag("constructKaptonLayer")) {
					analysisManager->FillNtupleDColumn(3, 4, entryEnergy);
				} else if(detectorName.compare("logicCopper") == 0 && StaticInfo::GetDetectorFlag("constructCopperLayer")) {
					analysisManager->FillNtupleDColumn(3, 5, entryEnergy);
				} else {
					G4cerr << "Detector non trovato!!!" << G4endl;
				}
				
				fDeltaEnergyStep = step->GetDeltaEnergy();
			}
			
			if(fTrackID == step->GetTrack()->GetTrackID()){
				if(!step->IsFirstStepInVolume()){
					fDeltaEnergyStep += step->GetDeltaEnergy();
				}
			} else {
				fDeltaEnergyStep = 0.;
			}
			
			//la seconda condizione esclude urti inelastici
			if(step->IsLastStepInVolume() && fTrackID == step->GetTrack()->GetTrackID()) {
				
				// G4Step::GetDeltaEnergy definisce il deltaE come (postStepPoint->Ekin - preStepPoint->Ekin) 
				// quindi se perdo energia nel materiale è normale che il deltaE sia negativo
				
				fLastMomDirectionInVolume = step->GetPostStepPoint()->GetMomentumDirection();
				G4double scatteringAngle = fLastMomDirectionInVolume.getTheta() - fFirstMomDirectionInVolume.getTheta();
				//G4cout<<"---UserSteppingAction---scatteringAngle: "<<scatteringAngle/deg<<" deg = "<<scatteringAngle/rad<<" rad"<<G4endl;
				//G4cout<<"---UserSteppingAction---fDeltaEnergyStep: "<<fDeltaEnergyStep/MeV<<" MeV"<<G4endl;
				G4String detectorName = volume->GetName();
				if(detectorName.compare("logicAlpide") == 0) {
					analysisManager->FillNtupleDColumn(2, 0, scatteringAngle);
					analysisManager->FillNtupleDColumn(4, 0, fDeltaEnergyStep);
				} else if(detectorName.compare("logicLowerGlue") == 0 && StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) {
					analysisManager->FillNtupleDColumn(2, 1, scatteringAngle);
					analysisManager->FillNtupleDColumn(4, 1, fDeltaEnergyStep);
				} else if(detectorName.compare("logicUpperGlue") == 0 && StaticInfo::GetDetectorFlag("constructEpoxyGlueLayer")) {
					analysisManager->FillNtupleDColumn(2, 2, scatteringAngle);
					analysisManager->FillNtupleDColumn(4, 2, fDeltaEnergyStep);
				} else if(detectorName.compare("logicLowerKapton") == 0 && StaticInfo::GetDetectorFlag("constructKaptonLayer")) {
					analysisManager->FillNtupleDColumn(2, 3, scatteringAngle);
					analysisManager->FillNtupleDColumn(4, 3, fDeltaEnergyStep);
				} else if(detectorName.compare("logicUpperKapton") == 0 && StaticInfo::GetDetectorFlag("constructKaptonLayer")) {
					analysisManager->FillNtupleDColumn(2, 4, scatteringAngle);
					analysisManager->FillNtupleDColumn(4, 4, fDeltaEnergyStep);
				} else if(detectorName.compare("logicCopper") == 0 && StaticInfo::GetDetectorFlag("constructCopperLayer")) {
					analysisManager->FillNtupleDColumn(2, 5, scatteringAngle);
					analysisManager->FillNtupleDColumn(4, 5, fDeltaEnergyStep);
				} else {
					G4cerr << "Detector non trovato!!!" << G4endl;
				}
				
				fDeltaEnergyStep = 0.;
				
			}
			
		} else if(volume->GetName().substr(0,8).compare("LV_SOLID") == 0) {
			//PCB energy deposition
			G4double edep = step->GetTotalEnergyDeposit();
			fEventAction->AddEdep(edep);
			
			auto analysisManager = G4AnalysisManager::Instance();
		
			if(step->IsFirstStepInVolume()) {
				fTrackID = step->GetTrack()->GetTrackID();
				fFirstMomDirectionInVolume = step->GetPreStepPoint()->GetMomentumDirection();
				
				G4double entryEnergy = step->GetPreStepPoint()->GetTotalEnergy();
				analysisManager->FillNtupleDColumn(3, 6, entryEnergy);
				
				fDeltaEnergyStep = step->GetDeltaEnergy();
			}
			
			if(fTrackID == step->GetTrack()->GetTrackID()){
				if(!step->IsFirstStepInVolume()){
					fDeltaEnergyStep += step->GetDeltaEnergy();
				}
			} else {
				fDeltaEnergyStep = 0.;
			}
			
			//la seconda condizione esclude urti inelastici
			if(step->IsLastStepInVolume() && fTrackID == step->GetTrack()->GetTrackID()) {
				
				// G4Step::GetDeltaEnergy definisce il deltaE come (postStepPoint->Ekin - preStepPoint->Ekin) 
				// quindi se perdo energia nel materiale è normale che il deltaE sia negativo
				
				fLastMomDirectionInVolume = step->GetPostStepPoint()->GetMomentumDirection();
				G4double scatteringAngle = fLastMomDirectionInVolume.getTheta() - fFirstMomDirectionInVolume.getTheta();
				//G4cout<<"---UserSteppingAction---scatteringAngle: "<<scatteringAngle/deg<<" deg = "<<scatteringAngle/rad<<" rad"<<G4endl;
				analysisManager->FillNtupleDColumn(2, 6, scatteringAngle);
				
				analysisManager->FillNtupleDColumn(4, 6, fDeltaEnergyStep);
				fDeltaEnergyStep = 0.;
				
			}
			
		}
	
	}
	
	
	
	
        
}
