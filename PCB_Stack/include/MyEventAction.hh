#ifndef MY_EVENT_ACTION_HH
#define MY_EVENT_ACTION_HH

#include <vector>
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4THitsCollection.hh"
#include "MySensitiveDetectorHit.hh"
#include "G4RunManager.hh"
#include "G4HCtable.hh"

//User defined
#include "Constants.hh"
#include "StaticInfo.hh"

class MyEventAction : public G4UserEventAction
{

public:

    MyEventAction();
    ~MyEventAction() override = default;
    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;
    void AddEdep(G4double edep) { fPCBedep += edep; };
    
private:

    G4VHitsCollection* GetHC(const G4Event* event, G4int collId);

    G4double fPCBedep;
    //G4double fScatteringAngle;
    // problema per rendere tutto configurabile: fHitCollID funziona solo se è un array di lunghezza FISSA -> non posso usare vector
    std::vector<G4int> fHitCollID;
    std::vector<G4int> fPCBHitCollID;
    //Array che contiene NofMySensitiveDetector vector. Ogni vector contiene le energie delle N hit del detector
    //std::array<std::vector<G4double>, NofMySensitiveDetector> fDetEdep;
    std::vector<G4String> detHCName;
    std::vector<G4String> PCBHCName;
    
};

#endif
