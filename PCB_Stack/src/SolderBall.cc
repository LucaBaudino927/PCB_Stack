#include "SolderBall.hh"

//Notice that: "mother volume" refers to the volume in which the object has to be placed:
//it can be inside other objects or in a more general frame.

//constructor
SolderBall::SolderBall(Alpide* alpide, G4Material* material) 
{

    fSolderBall = material;
    fAlpide = alpide;
    
}

//destructor
SolderBall::~SolderBall(){};

/*
//Construction and placement of single solderBall layer physical volume, covering all the detection system
void SolderBall::ConstructSolderBallLayerPV(G4double zInWorld, G4AssemblyVolume* assemblyDetector)
{

    for(int i = 0; i < fAlpide->GetNOfPads(); i++){
    	
    	G4Sphere* solidSolderBall = new G4Sphere("solidSolderBall_"+std::to_string(i), 0., fAlpide->GetPadRadius(), 0., 360.*degree, 0., 360.*degree);
	G4LogicalVolume* logicSolderBall = new G4LogicalVolume(solidSolderBall, GetSolderBallMaterial(), "logicSolderBall_"+std::to_string(i));
	//MapsFoilDetectorList::AddToLogicalDetectorList(logicSolderBall);
	G4VisAttributes* blue = new G4VisAttributes(G4Colour::Blue());
	blue->SetVisibility(true);
	logicSolderBall->SetVisAttributes(blue);
	
	G4cout << "solidSolderBall Radius: " << fAlpide->GetPadRadius()/um << " um" << G4endl;
	G4cout << "solidSolderBall Z center: " << zInWorld/um << " um" << G4endl;
	G4cout << "solidSolderBall Z goes from: " << (zInWorld - fAlpide->GetPadRadius())/um << " um to " 
			<< (zInWorld + fAlpide->GetPadRadius())/um << " um" << G4endl;
	G4cout<<"solidSolderBall_"<<std::to_string(i)<<" volume: "<<solidSolderBall->GetCubicVolume()<<G4endl;
    	
    	std::vector<G4ThreeVector> ballPositions = fAlpide->GetPadCoordinates();
    	// Rotation and translation of a plate inside the assembly
	G4RotationMatrix Ra;
	G4ThreeVector Ta;
	G4Transform3D Tr;

	// Fill the assembly by the plates
	Ta.setX(ballPositions[i].x()); 
	Ta.setY(ballPositions[i].y());
	Ta.setZ(zInWorld);
	Tr = G4Transform3D(Ra,Ta);
	assemblyDetector->AddPlacedVolume(logicSolderBall, Tr);
    	//new G4PVPlacement(0,{ballPositions[i].x(), ballPositions[i].y(), zInWorld}, logicSolderBall, "physSolderBall_"+std::to_string(i), worldLog, false, 1, true);
    	
    }

}*/

