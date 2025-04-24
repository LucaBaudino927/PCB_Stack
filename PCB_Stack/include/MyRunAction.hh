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

class G4Run;


class MyRunAction : public G4UserRunAction
{  
  
public:
    
        MyRunAction();
        ~MyRunAction() override = default;
        
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);

};

#endif
