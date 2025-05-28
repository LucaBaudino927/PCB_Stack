#ifndef MY_STEPPING_ACTION_HH
#define MY_STEPPING_ACTION_HH

#include <math.h>
#include <vector>
#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"

class MySteppingAction : public G4UserSteppingAction
{

public:

	MySteppingAction();
	~MySteppingAction();
	virtual void UserSteppingAction(const G4Step*);
	void SetNewEventFlag() { newEvent = true; };
	std::vector<G4double> GetRX0Vector() { return R_X0; };
	G4double GetBeamXPosition() { return X; };
	G4double GetBeamYPosition() { return Y; };
    
private:

	G4ThreeVector fExitAnglePreviousLayer;
	G4ThreeVector fEntryAnglePreviousLayer;
	G4int fTrackID;
	G4bool newEvent;
	std::vector<G4double> R_X0;
	G4double R;
	G4double X;
	G4double Y;
    
};

#endif
