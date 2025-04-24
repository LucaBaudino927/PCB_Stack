#include "Kapton.hh"

//Notice that: "mother volume" refers to the volume in which the object has to be placed:
//it can be inside other objects or in a more general frame.

//constructor
Kapton::Kapton(G4double x, G4double y, G4double kaptonThickness, G4Material* holeMaterial) {

    fKaptonThickness = kaptonThickness;
    fKaptonXDimension = x;
    fKaptonYDimension = y;
    fHoleMaterial = holeMaterial;
    
}

//destructor
Kapton::~Kapton(){};

/*
//Construction and placement of single kapton layer physical volume, covering all the detection system
void Kapton::ConstructLowerKaptonLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector)
{

    G4Box* solidKapton = new G4Box("solidLowerKapton", GetKaptonXDimension()*0.5, GetKaptonYDimension()*0.5, GetKaptonThickness()*0.5);
    G4LogicalVolume* logicKapton = new G4LogicalVolume(solidKapton, GetKaptonMaterial(), "logicLowerKapton", 0);
    MapsFoilDetectorList::AddToLogicalDetectorList(logicKapton);
    G4VisAttributes* brown = new G4VisAttributes(G4Colour::Brown());
    brown->SetVisibility(true);
    logicKapton->SetVisAttributes(brown);
    
    // Rotation and translation of a plate inside the assembly
    G4RotationMatrix Ra;
    G4ThreeVector Ta;
    G4Transform3D Tr;

    // Fill the assembly by the plates
    Ta.setX(xInWorld); 
    Ta.setY(yInWorld);
    Ta.setZ(zInWorld);
    Tr = G4Transform3D(Ra,Ta);
    assemblyDetector->AddPlacedVolume(logicKapton, Tr);
    
    //placement of the layer logical volume into its mother frame
    //new G4PVPlacement(0, {xInWorld, yInWorld, zInWorld}, logicKapton, "physLowerKapton", worldLog, false, 1, true);

}

//Construction and placement of single kapton layer physical volume, covering all the detection system
void Kapton::ConstructUpperKaptonLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector, Alpide* alpide)
{

    std::vector<G4ThreeVector> padPositions = alpide->GetPadCoordinates();
    G4VSolid* solidKapton = new G4Box("solidUpperKapton", GetKaptonXDimension()*0.5, GetKaptonYDimension()*0.5, GetKaptonThickness()*0.5);
    for(int i = 0; i < alpide->GetNOfPads(); i++){
    
    	G4Tubs* solidPadHole = new G4Tubs("solidPadHole", 0., alpide->GetPadRadius(), GetKaptonThickness()*0.5, 0., 360.*degree);
    	G4ThreeVector* translation = new G4ThreeVector(padPositions[i].x(), padPositions[i].y(), padPositions[i].z());
    	solidKapton = new G4SubtractionSolid("solidUpperGlue", solidKapton, solidPadHole, 0, *translation);
    	
    	G4cout << "solidPadHoleThickness: " << GetKaptonThickness()/um << " um" << G4endl;
    	G4cout << "solidPadHole Z center: " << zInWorld/um << " um" << G4endl;
    	G4cout << "solidPadHole Z goes from: "<<(zInWorld - GetKaptonThickness()*0.5)/um <<" um to "<< (zInWorld + GetKaptonThickness()*0.5)/um << " um" << G4endl;
    	
    }
    
    G4LogicalVolume* logicKapton = new G4LogicalVolume(solidKapton, GetKaptonMaterial(), "logicUpperKapton", 0);
    MapsFoilDetectorList::AddToLogicalDetectorList(logicKapton);
    G4VisAttributes* brown = new G4VisAttributes(G4Colour::Brown());
    brown->SetVisibility(true);
    logicKapton->SetVisAttributes(brown);
    
    G4cout << "solidUpperKaptonThickness: " << GetKaptonThickness()/um << " um" << G4endl;
    G4cout << "solidUpperKapton Z center: " << zInWorld/um << " um" << G4endl;
    G4cout << "solidUpperKapton Z goes from: "<<(zInWorld - GetKaptonThickness()*0.5)/um <<" um to "<< (zInWorld + GetKaptonThickness()*0.5)/um << " um" << G4endl;
    
    // Rotation and translation of a plate inside the assembly
    G4RotationMatrix Ra;
    G4ThreeVector Ta;
    G4Transform3D Tr;

    // Fill the assembly by the plates
    Ta.setX(xInWorld); 
    Ta.setY(yInWorld);
    Ta.setZ(zInWorld);
    Tr = G4Transform3D(Ra,Ta);
    assemblyDetector->AddPlacedVolume(logicKapton, Tr);
    
    //placement of the layer logical volume into its mother frame
    //new G4PVPlacement(0, {xInWorld, yInWorld, zInWorld}, logicKapton, "physUpperKapton", worldLog, false, 1, true);
    

}

*/
