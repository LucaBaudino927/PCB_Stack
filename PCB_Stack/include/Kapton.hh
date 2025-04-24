#ifndef KAPTON_HH
#define KAPTON_HH

#include <vector>
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "Alpide.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "StaticInfo.hh"
#include "G4AssemblyVolume.hh"
#include "G4SubtractionSolid.hh"

//Notice that in this system:
//Thickness refers to the Z coordinate
//
class Kapton {

public:

    Kapton(G4double x, G4double y, G4double kaptonThickness, G4Material* holeMaterial);
    virtual ~Kapton();

    G4Material* GetKaptonMaterial() { return fKapton; }
    G4Material* GetHoleMaterial()   { return fHoleMaterial; }
    G4double GetKaptonThickness()   { return fKaptonThickness; }
    G4double GetKaptonXDimension()  { return fKaptonXDimension; }
    G4double GetKaptonYDimension()  { return fKaptonYDimension; }
    void ConstructLowerKaptonLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector);
    void ConstructUpperKaptonLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector, Alpide* alpide);
    
    /*static G4VPhysicalVolume *ConstructKaptonMatrixPhVolume(Kapton *kapton, G4double xDimKaptondPad,
                                                          G4double yDimKaptondPad, G4int nOfColumn,
                                                          G4int nOfRows, G4double xInWorld,
                                                          G4double yInWorld, G4double zInWorld,
                                                          G4LogicalVolume *worldLog);*/

private:

    G4Material* fKapton = G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
    G4Material* fHoleMaterial;
    G4double fKaptonThickness;
    G4double fKaptonXDimension;
    G4double fKaptonYDimension;
    
};

#endif
