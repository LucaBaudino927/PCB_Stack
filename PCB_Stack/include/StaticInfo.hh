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
        
        static G4double GetBeamXPosition() { return fBeamXPosition; };
        static void SetBeamXPosition(G4double BeamXPosition) { fBeamXPosition = BeamXPosition; };
        
        static G4double GetBeamYPosition() { return fBeamYPosition; };
        static void SetBeamYPosition(G4double BeamYPosition) { fBeamYPosition = BeamYPosition; };

        static G4double GetResolution() { return resolution; };
        static void SetResolution(G4double res) { resolution = res; };

    private:

        static std::map<G4String, G4bool> detectorConstructionFlags;
        static G4int fRunIdOnMasterThread;
        static G4int fNumberOfLayer;
        static G4double fScanWidth; //member variable of MyPrimaryGenerator. Unit: mm
        static G4double fBeamXPosition; //member variable of MyPrimaryGenerator. Unit: mm
        static G4double fBeamYPosition; //member variable of MyPrimaryGenerator. Unit: mm
        static G4double resolution; // Used in MyRunAction to define the bin size of histograms, Unit: mm

};

#endif
