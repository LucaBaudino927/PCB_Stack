#include "Alpide.hh"

//Notice that: "mother volume" refers to the volume in which the object has to be placed:
//it can be inside other objects or in a more general frame.

//constructor
Alpide::Alpide(G4double x, G4double y, G4double alpideThickness, G4double padRadius) {

    fPadRadius = padRadius;
    fAlpideThickness = alpideThickness;
    fAlpideXDimension = x;
    fAlpideYDimension = y;
       
    // ##### info dei pad da 300 um dell'alpide 
    //per ora non configurabile
    //fPadCoordinates = {{-0.*mm, -0.*mm, 0.}, {-1.*mm, -1.*mm, 0.}, {-1.*mm, +1.*mm, 0.}, {+1.*mm, -1.*mm, 0.}, {+1.*mm, +1.*mm, 0.}};
    //fPadCoordinates = {{-0.*mm, -0.*mm, 0.}};
    //fNOfPads = fPadCoordinates.size();
    //StaticInfo::SetNOfAlpidePads(fNOfPads);
    
}

//destructor
Alpide::~Alpide(){};

/*
//Construction and placement of single alpide layer physical volume, covering all the detection system
void Alpide::ConstructAlpideLayerPV(G4double xInWorld, G4double yInWorld, G4double zInWorld, G4AssemblyVolume* assemblyDetector)
{

    G4Box* solidAlpide = new G4Box("solidAlpide", GetAlpideXDimension()*0.5, GetAlpideYDimension()*0.5, GetAlpideThickness()*0.5);
    //G4LogicalVolume* logicAlpide = new G4LogicalVolume(solidAlpide, GetAlpideMaterial(), "logicAlpide", 0, new MySensitiveDetector("sensAlpide"), 0, true);
    G4LogicalVolume* logicAlpide = new G4LogicalVolume(solidAlpide, GetAlpideMaterial(), "logicAlpide");
    MapsFoilDetectorList::AddToLogicalDetectorList(logicAlpide);
    G4VisAttributes* yellow = new G4VisAttributes(G4Colour::Yellow());
    yellow->SetVisibility(true);
    logicAlpide->SetVisAttributes(yellow);
    
    // Rotation and translation of a plate inside the assembly
    G4RotationMatrix Ra;
    G4ThreeVector Ta;
    G4Transform3D Tr;

    // Fill the assembly by the plates
    Ta.setX(xInWorld); 
    Ta.setY(yInWorld);
    Ta.setZ(zInWorld);
    Tr = G4Transform3D(Ra,Ta);
    assemblyDetector->AddPlacedVolume(logicAlpide, Tr);
    
    G4cout << "AlpideX: " << Alpide::GetAlpideXDimension()/cm << " cm" << G4endl;
    G4cout << "AlpideY: " << Alpide::GetAlpideYDimension()/cm << " cm" << G4endl;
    G4cout << "AlpideThickness: " << Alpide::GetAlpideThickness()/um << " um" << G4endl;
    
    //placement of the layer logical volume into its mother frame
    //new G4PVPlacement(0, {xInWorld, yInWorld, zInWorld}, logicAlpide, "physAlpide", worldLog, false, 1, true);
    
    //pads
    for(int i = 0; i < GetNOfPads(); i++){
    
    	std::vector<G4ThreeVector> padPositions = GetPadCoordinates();
    	G4double pad1Center = padPositions[i].z()+GetAlpideThickness()*0.5+GetPadThickness1()*0.5;
    	G4double pad2Center = padPositions[i].z()+GetAlpideThickness()*0.5+GetPadThickness1()+GetPadThickness2()*0.5;
    	
    	G4cout << "PadRadius1: " << Alpide::GetPadRadius()/um << " um" << G4endl;
	G4cout << "PadThickness1: " << Alpide::GetPadThickness1()/um << " um" << G4endl;
	//G4cout << "PadMaterial1: " << Alpide::GetPadMaterial1() << G4endl;
	G4cout << "Pad1 Z center: " << pad1Center/um << " um" << G4endl;
	G4cout << "Pad1 Z goes from: " << (pad1Center - GetPadThickness1()*0.5)/um << " um to " << (pad1Center + GetPadThickness1()*0.5)/um << " um" << G4endl;
    	G4cout << "PadRadius2: " << Alpide::GetPadRadius()/um << " um" << G4endl;
	G4cout << "PadThickness2: " << Alpide::GetPadThickness2()/nm << " nm" << G4endl;
	//G4cout << "PadMaterial2: " << Alpide::GetPadMaterial2() << G4endl;
	G4cout << "Pad2 Z center: " << (pad2Center)/um << " um" << G4endl;
	G4cout << "Pad2 Z goes from: " << (pad2Center - GetPadThickness2()*0.5)/um << " um to " << (pad2Center + GetPadThickness2()*0.5)/um << " um" << G4endl;
	
    	G4Tubs* solidPad1 = new G4Tubs("solidPad1_"+std::to_string(i), 0., GetPadRadius(), GetPadThickness1()*0.5, 0., 360.*degree);
    	G4LogicalVolume* logicPad1 = new G4LogicalVolume(solidPad1, GetPadMaterial1(), "logicPad1_"+std::to_string(i));
    	G4VisAttributes* grey = new G4VisAttributes(G4Colour::Grey());
    	grey->SetVisibility(true);
    	logicPad1->SetVisAttributes(grey);
    	MapsFoilDetectorList::AddToLogicalDetectorList(logicPad1);
    	//new G4PVPlacement(0, {padPositions[i].x(), padPositions[i].y(), pad1Center}, logicPad1, "physPad1_"+std::to_string(i), worldLog, false, 1, true);
    	
    	// Fill the assembly by the plates
	Ta.setX(padPositions[i].x()); 
	Ta.setY(padPositions[i].y());
	Ta.setZ(pad1Center);
	Tr = G4Transform3D(Ra,Ta);
	assemblyDetector->AddPlacedVolume(logicPad1, Tr);
    	
    	G4Tubs* solidPad2 = new G4Tubs("solidPad2_"+std::to_string(i), 0., GetPadRadius(), GetPadThickness2()*0.5, 0., 360.*degree);
    	G4LogicalVolume* logicPad2 = new G4LogicalVolume(solidPad2, GetPadMaterial2(), "logicPad2_"+std::to_string(i));
    	G4VisAttributes* yellow = new G4VisAttributes(G4Colour::Yellow());
    	yellow->SetVisibility(true);
    	logicPad2->SetVisAttributes(yellow);
    	//MapsFoilDetectorList::AddToLogicalDetectorList(logicPad2);
    	//new G4PVPlacement(0, {padPositions[i].x(), padPositions[i].y(), pad2Center}, logicPad2, "physPad2_"+std::to_string(i), worldLog, false, 1, true);
    	
    	// Fill the assembly by the plates
	Ta.setX(padPositions[i].x()); 
	Ta.setY(padPositions[i].y());
	Ta.setZ(pad2Center);
	Tr = G4Transform3D(Ra,Ta);
	assemblyDetector->AddPlacedVolume(logicPad2, Tr);
    	
    }

}
*/

