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
	deltaE = 0;
	pathLength = 0;
	particleName = "";
}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step* step){	
	
	if(newEvent) {
		particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
		fTrackID = step->GetTrack()->GetTrackID();
		R = 0;
		R_X0.clear();
		X = step->GetPreStepPoint()->GetPosition().x();
		Y = step->GetPreStepPoint()->GetPosition().y();
		deltaE = 0;
		pathLength = 0;
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
		
		G4double entryEnergy = step->GetPreStepPoint()->GetTotalEnergy();
		analysisManager->FillNtupleDColumn(3, 0, entryEnergy);
		
	}
	
	if(detectorName.compare("logicWorld") != 0 && particleName.compare(step->GetTrack()->GetParticleDefinition()->GetParticleName()) == 0) {
		R += step->GetStepLength();
		//if(fTrackID == step->GetTrack()->GetTrackID()) deltaE += step->GetDeltaEnergy();
		//const G4TrackVector* secondaryTracks = step->GetSecondary();
		//if(step->GetSecondary()->size() == 0) {
			deltaE += step->GetDeltaEnergy();
			pathLength += step->GetStepLength();
		//}
	}
	
	//la seconda condizione esclude urti inelastici
	if(step->IsLastStepInVolume() && fTrackID == step->GetTrack()->GetTrackID()) {
	
		if(detectorName.compare("logicWorld") != 0){
			R_X0.push_back(R/volume->GetMaterial()->GetRadlen());
			R = 0;
			//if(volume->GetMaterial()->GetName().compare("EpoxyGlue") == 0){
			//G4cout<<"---steppingAction X0="<<volume->GetMaterial()->GetRadlen()<<", X0="<<volume->GetMaterial()->GetRadlen()/CLHEP::cm<<" cm"<<G4endl;
			//}
		}
		
		if(detectorName.compare("logicWorld") == 0){
			fEntryAnglePreviousLayer = step->GetPostStepPoint()->GetMomentumDirection();
		}

	}

}
