#ifndef MY_STEPPING_ACTION_HH
#define MY_STEPPING_ACTION_HH

#include <math.h>
#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"

//User defined
#include "MyEventAction.hh"

class MySteppingAction : public G4UserSteppingAction
{

public:

	MySteppingAction(MyEventAction* eventAction);
	~MySteppingAction();
	virtual void UserSteppingAction(const G4Step*);
    
private:

	MyEventAction *fEventAction;
	G4ThreeVector fFirstMomDirectionInVolume;
	G4ThreeVector fLastMomDirectionInVolume;
	G4int fTrackID;
	G4double fDeltaEnergyStep;
    
};

#endif
