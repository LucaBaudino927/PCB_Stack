#ifndef STATIC_INFO_HH
#define STATIC_INFO_HH

#include <vector>
#include <map>
#include "G4ThreeVector.hh"

// Classe con info condivise da tutto il progetto e ottenute dal G4GenericMessenger
class StaticInfo
{

    public:

        StaticInfo();
        ~StaticInfo();
        static void Clear();
        
        static std::map<G4String, G4bool> GetDetectorConstructionFlags() { return detectorConstructionFlags; };
        static G4bool GetDetectorFlag(G4String flagName) { return detectorConstructionFlags[flagName]; };
        static void AddToDetectorFlagMap(G4String flagName, G4bool flag) { detectorConstructionFlags[flagName] = flag; };
        
        static G4int GetRunIdOnMasterThread() { return fRunIdOnMasterThread; };
        static void SetRunIdOnMasterThread(G4int newRunID) { fRunIdOnMasterThread = newRunID; };
        
        static G4int GetNumberOfLayer() { return fNumberOfLayer; };
        static void SetNumberOfLayer(G4int NumberOfLayer) { fNumberOfLayer = NumberOfLayer; };
        
        static G4double GetScanWidth() { return fScanWidth; };
        static void SetScanWidth(G4double ScanWidth) { fScanWidth = ScanWidth; };     

    private:
    	
        static std::map<G4String, G4bool> detectorConstructionFlags;
        static G4int fRunIdOnMasterThread;
        static G4int fNumberOfLayer;
        static G4double fScanWidth; //member variable of MyPrimaryGenerator. Unit: mm

};

#endif
