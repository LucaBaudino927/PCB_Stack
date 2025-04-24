#ifndef COPPER_HH
#define COPPER_HH

#include <vector>
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "StaticInfo.hh"
#include "G4AssemblyVolume.hh"

//Notice that in this system:
//Thickness refers to the Z coordinate
//
class Copper {

public:

    Copper(G4double x, G4double y, G4double copperThickness);
    virtual ~Copper();

    G4Material* GetCopperMaterial() { return fCopper; }
    G4double GetCopperThickness()   { return fCopperThickness; }
    G4double GetCopperXDimension()  { return fCopperXDimension; }
    G4double GetCopperYDimension()  { return fCopperYDimension; }
    void ConstructCopperLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector);
    
    /*static G4VPhysicalVolume *ConstructCopperMatrixPhVolume(Copper *copper, G4double xDimCopperdPad,
                                                          G4double yDimCopperdPad, G4int nOfColumn,
                                                          G4int nOfRows, G4double xInWorld,
                                                          G4double yInWorld, G4double zInWorld,
                                                          G4LogicalVolume *worldLog);*/

private:

    G4Material* fCopper = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
    G4double fCopperThickness;
    G4double fCopperXDimension;
    G4double fCopperYDimension;
    
};

#endif
