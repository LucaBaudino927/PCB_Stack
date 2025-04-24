#ifndef SOLDERBALL_HH
#define SOLDERBALL_HH

#include <vector>
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "StaticInfo.hh"
#include "Alpide.hh"
#include "G4Sphere.hh"
#include "G4AssemblyVolume.hh"

//Notice that in this system:
//Thickness refers to the Z coordinate
//
class SolderBall {

public:

    SolderBall(Alpide* alpide, G4Material* SolderBallMaterial=G4NistManager::Instance()->FindOrBuildMaterial("G4_Sn"));
    virtual ~SolderBall();

    G4Material* GetSolderBallMaterial() { return fSolderBall; }
    G4double GetSolderBallRadius()	{ return fSolderBallRadius; }
    Alpide* GetAlpide()			{ return fAlpide; }
    void ConstructSolderBallLayerPV(G4double zInWorld, G4AssemblyVolume* assemblyDetector);
    
    /*static G4VPhysicalVolume *ConstructSolderBallMatrixPhVolume(SolderBall *SolderBall, G4double xDimSolderBalldSolderBall,
                                                          G4double yDimSolderBalldSolderBall, G4int nOfColumn,
                                                          G4int nOfRows, G4double xInWorld,
                                                          G4double yInWorld, G4double zInWorld,
                                                          G4LogicalVolume *worldLog);*/

private:

    G4Material* fSolderBall = G4NistManager::Instance()->FindOrBuildMaterial("G4_Sn");
    G4double fSolderBallRadius;
    Alpide* fAlpide;
        
};

#endif
