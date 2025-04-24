#ifndef ALPIDE_HH
#define ALPIDE_HH

#include <vector>
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "StaticInfo.hh"
#include "G4AssemblyVolume.hh"

//Notice that in this system:
//Thickness refers to the Z coordinate
//
class Alpide {

public:

    Alpide(G4double x, G4double y, G4double alpideThickness, G4double padRadius);
    virtual ~Alpide();

    G4Material* GetAlpideMaterial() { return fAlpide; }
    G4double GetAlpideThickness()   { return fAlpideThickness; }
    G4double GetAlpideXDimension()  { return fAlpideXDimension; }
    G4double GetAlpideYDimension()  { return fAlpideYDimension; }
    G4double GetPadRadius()	    { return fPadRadius; }
    //G4int GetNOfPads()		    { return fNOfPads; }
    G4Material* GetPadMaterial1()   { return fPadMaterial1; }
    G4double GetPadThickness1()     { return fPadThickness1; }
    G4Material* GetPadMaterial2()   { return fPadMaterial2; }
    G4double GetPadThickness2()     { return fPadThickness2; }
    //std::vector<G4ThreeVector> GetPadCoordinates() { return fPadCoordinates; }
    void ConstructAlpideLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector);
    

private:

    G4Material* fAlpide = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
    G4double fAlpideThickness;
    G4double fAlpideXDimension;
    G4double fAlpideYDimension;
    G4double fPadRadius;
    //G4int fNOfPads;
    G4Material* fPadMaterial1 = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ni");
    G4double fPadThickness1 = 2.5*um;
    G4Material* fPadMaterial2 = G4NistManager::Instance()->FindOrBuildMaterial("G4_Au");
    G4double fPadThickness2 = 50.*nm;
    //std::vector<G4ThreeVector> fPadCoordinates;
    
};

#endif
