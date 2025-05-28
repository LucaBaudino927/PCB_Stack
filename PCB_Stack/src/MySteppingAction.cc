#include "MySteppingAction.hh"

MySteppingAction::MySteppingAction() {
	fExitAnglePreviousLayer = {0., 0., 0.};
	fEntryAnglePreviousLayer = {0., 0., 0.};
	fTrackID = -1;
	newEvent = true;
	R_X0.push_back(-1);
	R_X0.clear();
	R = 0;
	X = 0;
	Y = 0;
}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step* step){	

	G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
	if(particleName.compare("geantino") == 0){
	
		if(newEvent) {
			R = 0;
			R_X0.clear();
			X = step->GetPreStepPoint()->GetPosition().x();
			Y = step->GetPreStepPoint()->GetPosition().y();
			
		}
	
		G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
		G4String detectorName = volume->GetName();
			
		auto analysisManager = G4AnalysisManager::Instance();
	
		if(step->IsFirstStepInVolume()) {
		
			if(detectorName.compare("logicWorld") == 0 && !newEvent){
			
				fExitAnglePreviousLayer = step->GetPreStepPoint()->GetMomentumDirection();
				G4double thetaF = fExitAnglePreviousLayer.y() > 0. ? fExitAnglePreviousLayer.getTheta() : -fExitAnglePreviousLayer.getTheta();
				G4double thetaI = fEntryAnglePreviousLayer.y() > 0. ? fEntryAnglePreviousLayer.getTheta() : -fEntryAnglePreviousLayer.getTheta();
				G4double scatteringAngle = thetaF - thetaI;
				
				analysisManager->FillNtupleDColumn(2, 0, scatteringAngle);
				
			}
			
			newEvent = false;
		
			fTrackID = step->GetTrack()->GetTrackID();
			
			G4double entryEnergy = step->GetPreStepPoint()->GetTotalEnergy();
			analysisManager->FillNtupleDColumn(3, 0, entryEnergy);
			
		}
		
		if(detectorName.compare("logicWorld") != 0){
			R += step->GetStepLength();
		}
		
		//la seconda condizione esclude urti inelastici
		if(step->IsLastStepInVolume() && fTrackID == step->GetTrack()->GetTrackID()) {
		
			if(detectorName.compare("logicWorld") != 0){
				R_X0.push_back(R/volume->GetMaterial()->GetRadlen());
				R = 0;
				//G4cout<<"---steppingAction X0="<<volume->GetMaterial()->GetRadlen()
				//					<<", X0="<<volume->GetMaterial()->GetRadlen()/CLHEP::cm<<" cm"<<G4endl;
			}
			
			if(detectorName.compare("logicWorld") == 0){
				fEntryAnglePreviousLayer = step->GetPostStepPoint()->GetMomentumDirection();
			}			
			
		}
			
		
	
	}
	
	
	
	
        
}
