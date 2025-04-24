#include "Copper.hh"

//Notice that: "mother volume" refers to the volume in which the object has to be placed:
//it can be inside other objects or in a more general frame.

//constructor
Copper::Copper(G4double x, G4double y, G4double copperThickness) {

    fCopperThickness = copperThickness;
    fCopperXDimension = x;
    fCopperYDimension = y;
    
}

//destructor
Copper::~Copper(){};

/*
//Construction and placement of single copper layer physical volume, covering all the detection system
void Copper::ConstructCopperLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector)
{

    G4Box* solidCopper = new G4Box("solidCopper", GetCopperXDimension()*0.5, GetCopperYDimension()*0.5, GetCopperThickness()*0.5);
    G4LogicalVolume* logicCopper = new G4LogicalVolume(solidCopper, GetCopperMaterial(), "logicCopper");
    MapsFoilDetectorList::AddToLogicalDetectorList(logicCopper);
    G4VisAttributes* red = new G4VisAttributes(G4Colour::Red());
    red->SetVisibility(true);
    logicCopper->SetVisAttributes(red);
   
    G4cout << "CopperThickness: " << GetCopperThickness()/um << " um" << G4endl;
    G4cout << "Copper Z center: " << zInWorld/um << " um" << G4endl;
    G4cout << "Copper Z goes from: " << (zInWorld - GetCopperThickness()*0.5)/um << " um to " << (zInWorld + GetCopperThickness()*0.5)/um << " um" << G4endl;
    
    // Rotation and translation of a plate inside the assembly
    G4RotationMatrix Ra;
    G4ThreeVector Ta;
    G4Transform3D Tr;

    // Fill the assembly by the plates
    Ta.setX(xInWorld); 
    Ta.setY(yInWorld);
    Ta.setZ(zInWorld);
    Tr = G4Transform3D(Ra,Ta);
    assemblyDetector->AddPlacedVolume(logicCopper, Tr);
    
    //placement of the layer logical volume into its mother frame
    //new G4PVPlacement(0, {xInWorld, yInWorld, zInWorld}, logicCopper, "physCopper", worldLog, false, 1, true);
    
    	

}

*/
