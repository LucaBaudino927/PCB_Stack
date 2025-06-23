#ifndef MY_RUN_ACTION_HH
#define MY_RUN_ACTION_HH

#include <vector>
#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

#include "Constants.hh"
#include "StaticInfo.hh"
#include "G4UImanager.hh"
#include "G4GenericMessenger.hh"

class G4Run;


class MyRunAction : public G4UserRunAction
{  
  
public:
    
        MyRunAction();
        ~MyRunAction() override = default;
        
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
        virtual void DefineCommands();

private:

        G4GenericMessenger* fMessenger = nullptr;
        G4double Y_BeamPosition = 0.*mm;
        G4double X_BeamPosition = 0.*mm;
        G4double fScanWidth = 20.*mm;

};

#endif
