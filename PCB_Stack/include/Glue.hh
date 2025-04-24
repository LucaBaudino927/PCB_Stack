#ifndef GLUE_HH
#define GLUE_HH

#include <vector>
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "Alpide.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSolid.hh"
#include "StaticInfo.hh"
#include "G4AssemblyVolume.hh"

//Notice that in this system:
//Thickness refers to the Z coordinate
//
class Glue {

public:

    Glue(G4double x, G4double y, G4double glueThickness, G4Material* glue, G4Material* holeMaterial);
    virtual ~Glue();

    G4Material* GetGlueMaterial() { return fGlue; }
    G4Material* GetHoleMaterial() { return fHoleMaterial; }
    G4double GetGlueThickness()   { return fGlueThickness; }
    G4double GetGlueXDimension()  { return fGlueXDimension; }
    G4double GetGlueYDimension()  { return fGlueYDimension; }
    void ConstructLowerGlueLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector);
    void ConstructUpperGlueLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector, Alpide* alpide);
    
    /*static G4VPhysicalVolume *ConstructGlueMatrixPhVolume(Glue *glue, G4double xDimGluedPad,
                                                          G4double yDimGluedPad, G4int nOfColumn,
                                                          G4int nOfRows, G4double xInWorld,
                                                          G4double yInWorld, G4double zInWorld,
                                                          G4LogicalVolume *worldLog);*/

private:

    G4Material* fGlue;
    G4Material* fHoleMaterial;
    G4double fGlueThickness;
    G4double fGlueXDimension;
    G4double fGlueYDimension;
    
};

#endif
