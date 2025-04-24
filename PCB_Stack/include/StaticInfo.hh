#ifndef STATIC_INFO_HH
#define STATIC_INFO_HH

#include <vector>
#include <map>
#include "G4ThreeVector.hh"

// Classe con info condivise da tutto il progetto e ottenute dal G4GenericMessenger
class StaticInfo
{

   	/************************************************************************************************
	 *	Cosa salvare in questa classe?								*
	 *	-G4ThreeVector delle coordinate dei pad dell'alpide (per ora le coordinate sono fisse)	*
	 *	-numero di sensitive detector (da usare nella RunAction e EventAction)			*
	 *	-flag dei detector costruiti								*
	 ************************************************************************************************/

    public:

        StaticInfo();
        ~StaticInfo();
        static void Clear();
        
        //static std::vector<G4ThreeVector> GetAlpidePadCoordinates() { return alpidePadCoordinates; };
        //static G4ThreeVector GetAlpidePadCoordinates(G4int index) { return alpidePadCoordinates[index]; };
        //static void AddToAlpidePadCoordinatesList(G4ThreeVector v) { alpidePadCoordinates.push_back(v); };
        
        //static G4int GetNOfSensitiveDetector() { return NOfSensitiveDetector; };
        //static void AddOneSensitiveDetector() { NOfSensitiveDetector++; };
        
        static std::map<G4String, G4bool> GetDetectorConstructionFlags() { return detectorConstructionFlags; };
        static G4bool GetDetectorFlag(G4String flagName) { return detectorConstructionFlags[flagName]; };
        static void AddToDetectorFlagMap(G4String flagName, G4bool flag) { detectorConstructionFlags[flagName] = flag; };
        
        //static G4int GetNOfAlpidePads() { return NOfAlpidePads; };
        //static void SetNOfAlpidePads(G4int num) { NOfAlpidePads = num; };
        
        static G4int GetRunIdOnMasterThread() { return fRunIdOnMasterThread; };
        static void SetRunIdOnMasterThread(G4int newRunID) { fRunIdOnMasterThread = newRunID; };

    private:
    	
    	//static std::vector<G4ThreeVector> alpidePadCoordinates;
        //static G4int NOfSensitiveDetector;
        static std::map<G4String, G4bool> detectorConstructionFlags;
        static G4int fRunIdOnMasterThread;

};

#endif
