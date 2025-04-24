#include "Glue.hh"

//Notice that: "mother volume" refers to the volume in which the object has to be placed:
//it can be inside other objects or in a more general frame.

//constructor
Glue::Glue(G4double x, G4double y, G4double glueThickness, G4Material* glue, G4Material* holeMaterial) {

    fGlue = glue;

    if (fGlue == nullptr)
    {

        G4cerr << "  ERROR Glue() " << G4endl
               << "  G4Material* epoxyGlue was not correctly created in MyDetectorConstruction::DefineMaterials()!"
               << G4endl;
    }

    fGlueThickness = glueThickness;
    fGlueXDimension = x;
    fGlueYDimension = y;
    fHoleMaterial = holeMaterial;
    
}

//destructor
Glue::~Glue(){};

/*
//Construction and placement of single glue layer physical volume, covering all the detection system
void Glue::ConstructLowerGlueLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector) {

    //G4cout << "Lower glue X: " << GetGlueXDimension()/cm << "cm" << G4endl;
    //G4cout << "Lower glue Y: " << GetGlueYDimension()/cm << "cm" << G4endl;
    G4cout << "Lower glue Z thickness: " << GetGlueThickness()/CLHEP::um << " um" << G4endl;
    G4cout << "Lower xInWorld: " << xInWorld/cm << "cm" << G4endl;
    G4cout << "Lower yInWorld: " << yInWorld/cm << "cm" << G4endl;
    G4cout << "Lower zInWorld: " << zInWorld/CLHEP::um << " um" << G4endl;
    G4cout << "Lower glue goes from: " << (zInWorld - GetGlueThickness()*0.5)/um << " um to " << (zInWorld + GetGlueThickness()*0.5)/um << " um" << G4endl;
	
    G4Box* solidGlue = new G4Box("solidLowerGlue", GetGlueXDimension()*0.5, GetGlueYDimension()*0.5, GetGlueThickness()*0.5);
    G4LogicalVolume* logicGlue = new G4LogicalVolume(solidGlue, GetGlueMaterial(), "logicLowerGlue", 0);
    G4VisAttributes* green = new G4VisAttributes(G4Colour::Green());
    green->SetVisibility(true);
    logicGlue->SetVisAttributes(green);
    
    // Rotation and translation of a plate inside the assembly
    G4RotationMatrix Ra;
    G4ThreeVector Ta;
    G4Transform3D Tr;

    // Fill the assembly by the plates
    Ta.setX(xInWorld); 
    Ta.setY(yInWorld);
    Ta.setZ(zInWorld);
    Tr = G4Transform3D(Ra,Ta);
    assemblyDetector->AddPlacedVolume(logicGlue, Tr);
    
    //placement of the layer logical volume into its mother frame
    //new G4PVPlacement(0, {xInWorld, yInWorld, zInWorld}, logicGlue, "physLowerGlue", worldLog, false, 1, true);

}



//Construction and placement of single glue layer physical volume, covering all the detection system
void Glue::ConstructUpperGlueLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector, Alpide* alpide){

    std::vector<G4ThreeVector> padPositions = alpide->GetPadCoordinates();
    
    //G4cout << "Upper glue X: " << GetGlueXDimension()/cm << "cm" << G4endl;
    //G4cout << "Upper glue Y: " << GetGlueYDimension()/cm << "cm" << G4endl;
    G4cout << "Upper glue Z thickness: " << GetGlueThickness()/CLHEP::um << " um" << G4endl;
    G4cout << "Upper xInWorld: " << xInWorld/cm << "cm" << G4endl;
    G4cout << "Upper yInWorld: " << yInWorld/cm << "cm" << G4endl;
    G4cout << "Upper zInWorld: " << zInWorld/CLHEP::um << " um" << G4endl;
    G4cout << "Upper glue goes from: " << (zInWorld - GetGlueThickness()*0.5)/um << " um to " << (zInWorld + GetGlueThickness()*0.5)/um << " um" << G4endl;
	
    G4VSolid* solidGlue = new G4Box("solidGlue", GetGlueXDimension()*0.5, GetGlueYDimension()*0.5, GetGlueThickness()*0.5);
    for(int i = 0; i < alpide->GetNOfPads(); i++){
    
    	G4Tubs* solidPadHole = new G4Tubs("solidPadHole", 0., alpide->GetPadRadius(), GetGlueThickness()*0.5, 0., 360.*degree);
    	G4ThreeVector* translation = new G4ThreeVector(padPositions[i].x(), padPositions[i].y(), padPositions[i].z());
    	solidGlue = new G4SubtractionSolid("solidUpperGlue", solidGlue, solidPadHole, 0, *translation);
    	
    	//logs
    	G4cout << "solidPadHole Z: "<< padPositions[i].z()+(alpide->GetAlpideThickness()+GetGlueThickness())*0.5/um << " um" << G4endl;
    	G4cout << "solidPadHole goes from: " 
    	<< (padPositions[i].z()+(alpide->GetAlpideThickness()+GetGlueThickness())*0.5 - GetGlueThickness()*0.5)/um << " um to " 
    	<< (padPositions[i].z()+(alpide->GetAlpideThickness()+GetGlueThickness())*0.5 + (GetGlueThickness())*0.5)/um << " um" << G4endl;
    	G4cout << "solidGlue volume: " << solidGlue->GetCubicVolume() << G4endl;
    	
    }
    
    G4LogicalVolume* logicGlue = new G4LogicalVolume(solidGlue, GetGlueMaterial(), "logicUpperGlue", 0);
    MapsFoilDetectorList::AddToLogicalDetectorList(logicGlue);
    G4VisAttributes* green = new G4VisAttributes(G4Colour::Green());
    green->SetVisibility(true);
    logicGlue->SetVisAttributes(green);
    
    // Rotation and translation of a plate inside the assembly
    G4RotationMatrix Ra;
    G4ThreeVector Ta;
    G4Transform3D Tr;

    // Fill the assembly by the plates
    Ta.setX(xInWorld); 
    Ta.setY(yInWorld);
    Ta.setZ(zInWorld);
    Tr = G4Transform3D(Ra,Ta);
    assemblyDetector->AddPlacedVolume(logicGlue, Tr);
    
    //placement of the layer logical volume into its mother frame
    //new G4PVPlacement(0, {xInWorld, yInWorld, zInWorld}, logicGlue, "physUpperGlue", worldLog, false, 1, true);

}

*/
