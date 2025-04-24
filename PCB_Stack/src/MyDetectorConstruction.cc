#include "MyDetectorConstruction.hh"


// ######## Constructor 
MyDetectorConstruction::MyDetectorConstruction(){

        // ######## User Defined Messages
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	//fMessenger->DeclareProperty("nCols", nCols, "N cols");
	//fMessenger->DeclareProperty("nRows", nRows, "N rows");
	fMessenger->DeclareProperty("mapsFoil",  		isMapsFoil, 			"Construct Maps Foil");
	fMessenger->DeclareProperty("verboseDetConstruction",  	verboseDetConstr, 		"Verbose Detector Construction");
	fMessenger->DeclareProperty("constructAlpidePads", 	constructAlpidePads, 		"Build alpide pads");
	fMessenger->DeclareProperty("constructEpoxyGlueLayer", 	constructEpoxyGlueLayer, 	"Use Epoxy Glue Layers");
	fMessenger->DeclareProperty("constructKaptonLayer",    	constructKaptonLayer, 		"Use Kapton Layers");
	fMessenger->DeclareProperty("constructCopperLayer", 	constructCopperLayer, 		"Use Copper Layer");
	fMessenger->DeclareProperty("constructSolderBalls", 	constructSolderBalls, 		"Use Solder Balls");
	fMessenger->DeclareProperty("constructPCB", 		constructPCB, 			"Use PCB from GDML file");
	fMessenger->DeclareProperty("alpideXDimension", 	alpideXFromMessenger, 		"Alpide X-Dimension");
	fMessenger->DeclareProperty("alpideYDimension", 	alpideYFromMessenger, 		"Alpide Y-Dimension");
	fMessenger->DeclareProperty("alpideThickness",  	alpideThicknessFromMessenger, 	"Alpide Z-Thickness");
	fMessenger->DeclareProperty("alpidePadRadius",  	alpidePadRadiusFromMessenger, 	"Alpide Pad Radius");
	fMessenger->DeclareProperty("glueThickness", 		glueThicknessFromMessenger, 	"Glue Thickness");
	fMessenger->DeclareProperty("kaptonThickness", 		kaptonThicknessFromMessenger, 	"Kapton Thickness");
	fMessenger->DeclareProperty("copperThickness", 		copperThicknessFromMessenger, 	"Copper Thickness");

        // ######## World dimension definition
	xWorld = 10.*cm;
	yWorld = 10.*cm;
	zWorld = 10.*cm;
	
}

// ######## Destructor
MyDetectorConstruction::~MyDetectorConstruction(){}

// ######## Function to define material properties used in the simulation
void MyDetectorConstruction::DefineMaterials(){

	/************************************************************************
	 *									*
	 *	IMPORTANTE: 							*
	 *	Se costruisco un G4Material lo cerco come "G4_XYZ"		*
	 *	Se costruisco un G4Element lo cerco come "XYZ"			*
	 *	G4Material* Cu = nist->FindOrBuildMaterial("G4_Cu");		*
	 *	G4Element* C = nist->FindOrBuildElement("C");			*
	 *									*
	 ************************************************************************/
	
	auto nist = G4NistManager::Instance();
	
        // ######## AIR
	worldMat = nist->FindOrBuildMaterial("G4_Galactic");

        // ######## Si
	Si = nist->FindOrBuildMaterial("G4_Si");

        // ######## Kapton
	KaptonMaterial = nist->FindOrBuildMaterial("G4_KAPTON");

        // ######## Al
	Al = nist->FindOrBuildMaterial("G4_Al");
	
	// ####### C
	C = nist->FindOrBuildElement("C");
	
	// ######## H
	H = nist->FindOrBuildElement("H");
	
	// ######## N
	N = nist->FindOrBuildElement("N");
	
	// ######## O
	O = nist->FindOrBuildElement("O");
	
	// ####### Epoxy glue C8H20N2O
	if(epoxyGlue == nullptr){
		epoxyGlue = new G4Material("EpoxyGlue", 1.25 * CLHEP::g / CLHEP::cm3, 4);
	    	epoxyGlue->AddElement(C, 25.8064516*perCent);
	    	epoxyGlue->AddElement(H, 64.516129*perCent);
	    	epoxyGlue->AddElement(N, 6.4516129*perCent);
	    	epoxyGlue->AddElement(O, 3.2258065*perCent);
	}

	G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
}

// ######## Actual construction of the detector based on the boolean selections 
G4VPhysicalVolume* MyDetectorConstruction::Construct(){

	// Material properties definition
	DefineMaterials();
	
	//inizializzare geometria
        solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
        logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");        
        physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "physWorld", nullptr, false, 0, true);
        
        G4VisAttributes* white = new G4VisAttributes(G4Colour::White());
    	white->SetVisibility(true);
    	logicWorld->SetVisAttributes(white);
    	
    	// Add info to Static class
	StaticInfo::Clear();
	StaticInfo::AddToDetectorFlagMap("constructAlpidePads", 	constructAlpidePads);
	StaticInfo::AddToDetectorFlagMap("constructEpoxyGlueLayer", 	constructEpoxyGlueLayer);
	StaticInfo::AddToDetectorFlagMap("constructKaptonLayer", 	constructKaptonLayer);
	StaticInfo::AddToDetectorFlagMap("constructCopperLayer", 	constructCopperLayer);
	StaticInfo::AddToDetectorFlagMap("constructSolderBalls", 	constructSolderBalls);
	StaticInfo::AddToDetectorFlagMap("constructPCB", 		constructPCB);
	StaticInfo::AddToDetectorFlagMap("verboseDetConstruction", 	verboseDetConstr);
	//flag per notificare la EventAction che la geometria è cambiata e quindi sono cambiati gli istogrammi da riempire
	//StaticInfo::AddToDetectorFlagMap("geometryModified", 		true);
	G4String banner  = "===================================================================\n";
	banner 		+= "||              MyDetectorConstruction::Construct()              ||\n";
	banner 		+= "||             La geometria è stata reinizializzata!             ||\n";
	banner 		+= "==================================================================="  ;
	G4cout << banner << G4endl;

	
	//for (const auto& [key, value] : StaticInfo::GetDetectorConstructionFlags()) G4cout << '[' << key << "] = " << value << "; ";

	// Define one layer as one assembly volume
	G4AssemblyVolume* assemblyDetector = new G4AssemblyVolume(); //AssemblyID = 1
	
        if(isMapsFoil) 	 { ConstructMapsFoil(assemblyDetector); }
        //if(constructPCB) { ConstructPCB(assemblyDetector); }
        
        // Inserisco l'assembly nel world logical volume
	G4int NofLayer = 1;
	for(unsigned int i = 0; i < NofLayer; i++) {
		// Rotation of the assembly inside the world
		G4RotationMatrix Rm;
		// Translation of the assembly inside the world
		G4double offset = 1.*cm;
		G4ThreeVector Tm(0., 0., 0. + i*offset);
		G4Transform3D Tr = G4Transform3D(Rm,Tm);
		assemblyDetector->MakeImprint(logicWorld, Tr);
	}
	
        return physWorld;//mother volume
}

// ######## Counstruct Maps detector surrounded by foils of glue and polyimide
// TODO: tutte le dimensioni qui saranno da parametrizzare!!!
void MyDetectorConstruction::ConstructMapsFoil(G4AssemblyVolume* assemblyDetector){

	G4double alpideX 	 =	(alpideXFromMessenger != 0.) 		? alpideXFromMessenger*mm 	  : 30.*mm;
	G4double alpideY 	 = 	(alpideYFromMessenger != 0.) 		? alpideYFromMessenger*mm 	  : 15.*mm;
	G4double alpideThickness = 	(alpideThicknessFromMessenger != 0.) 	? alpideThicknessFromMessenger*um : 50.*um;
	G4double alpidePadRadius = 	(alpidePadRadiusFromMessenger != 0.) 	? alpidePadRadiusFromMessenger*um : 150.*um;
	G4double glueThickness 	 = 	(glueThicknessFromMessenger != 0.) 	? glueThicknessFromMessenger*um   : 25.*um;
	G4double kaptonThickness = 	(kaptonThicknessFromMessenger != 0.) 	? kaptonThicknessFromMessenger*um : 50.*um;
	G4double copperThickness = 	(copperThicknessFromMessenger != 0.) 	? copperThicknessFromMessenger*um : 5.*um;
	
	// Alpide----------------------------------------------------------------------------------------------------------------
	G4double Z = 0.*um;
	// Rotation and translation of a layer inside the assembly
	G4RotationMatrix Ra;
	G4ThreeVector Ta;
	G4Transform3D Tr;
	Alpide *alpide = new Alpide(alpideX, alpideY, alpideThickness, alpidePadRadius);
	
	G4Box* solidAlpide = new G4Box("solidAlpide", alpide->GetAlpideXDimension()*0.5, alpide->GetAlpideYDimension()*0.5, alpide->GetAlpideThickness()*0.5);
	fLogicAlpide = new G4LogicalVolume(solidAlpide, alpide->GetAlpideMaterial(), "logicAlpide");

	// Fill the assembly by the plates
	Ta.setX(0.*um); 
	Ta.setY(0.*um);
	Ta.setZ(Z);
	Tr = G4Transform3D(Ra,Ta);
	assemblyDetector->AddPlacedVolume(fLogicAlpide, Tr);

	if(verboseDetConstr){
		G4cout << "AlpideX: " << alpide->GetAlpideXDimension()/cm << " cm" << G4endl;
		G4cout << "AlpideY: " << alpide->GetAlpideYDimension()/cm << " cm" << G4endl;
		G4cout << "AlpideThickness: " << alpide->GetAlpideThickness()/um << " um" << G4endl;
	}

	// alpide pads
	if(constructAlpidePads){
		for(int i = 0; i < NofPads; i++){

			G4double pad1Center = padCoordinates[i].z()+alpide->GetAlpideThickness()*0.5+alpide->GetPadThickness1()*0.5;
			G4double pad2Center = padCoordinates[i].z()+alpide->GetAlpideThickness()*0.5
								   +alpide->GetPadThickness1()
								   +alpide->GetPadThickness2()*0.5;

			G4Tubs* solidPad1 = new G4Tubs("solidPad1_"+std::to_string(i), 0., alpide->GetPadRadius(), 
											   alpide->GetPadThickness1()*0.5, 
											   0., 
											   360.*degree);
			G4LogicalVolume* logicPad1 = new G4LogicalVolume(solidPad1, alpide->GetPadMaterial1(), "logicPad1_"+std::to_string(i));
			fLogicalAlpidePad1.push_back(logicPad1);

			// Fill the assembly
			Ta.setX(padCoordinates[i].x() - alpideX/2.); 
			Ta.setY(padCoordinates[i].y() - alpideY/2.);
			Ta.setZ(pad1Center);
			Tr = G4Transform3D(Ra,Ta);
			assemblyDetector->AddPlacedVolume(logicPad1, Tr);

			G4Tubs* solidPad2 = new G4Tubs("solidPad2_"+std::to_string(i), 0., alpide->GetPadRadius(), 
											   alpide->GetPadThickness2()*0.5, 
											   0., 
											   360.*degree);
			G4LogicalVolume* logicPad2 = new G4LogicalVolume(solidPad2, alpide->GetPadMaterial2(), "logicPad2_"+std::to_string(i));
			fLogicalAlpidePad2.push_back(logicPad2);

			// Fill the assembly
			Ta.setX(padCoordinates[i].x() - alpideX/2.); 
			Ta.setY(padCoordinates[i].y() - alpideY/2.);
			Ta.setZ(pad2Center);
			Tr = G4Transform3D(Ra,Ta);
			assemblyDetector->AddPlacedVolume(logicPad2, Tr);
			
			if(verboseDetConstr){
				G4cout << "PadRadius1: " << alpide->GetPadRadius()/um << " um" << G4endl;
				G4cout << "PadThickness1: " << alpide->GetPadThickness1()/um << " um" << G4endl;
				//G4cout << "PadMaterial1: " << alpide->GetPadMaterial1() << G4endl;
				G4cout << "Pad1 Z center: " << pad1Center/um << " um" << G4endl;
				G4cout << "Pad1 Z goes from: " << (pad1Center - alpide->GetPadThickness1()*0.5)/um << " um to "
								<<(pad1Center + alpide->GetPadThickness1()*0.5)/um <<" um"      <<G4endl;
				G4cout << "PadRadius2: " << alpide->GetPadRadius()/um << " um" << G4endl;
				G4cout << "PadThickness2: " << alpide->GetPadThickness2()/nm << " nm" << G4endl;
				//G4cout << "PadMaterial2: " << alpide->GetPadMaterial2() << G4endl;
				G4cout << "Pad2 Z center: " << (pad2Center)/um << " um" << G4endl;
				G4cout << "Pad2 Z goes from: " << (pad2Center - alpide->GetPadThickness2()*0.5)/um << " um to "
								<<(pad2Center + alpide->GetPadThickness2()*0.5)/um <<" um"      <<G4endl;
			}

		}
	}
	
	// Glue------------------------------------------------------------------------------------------------------------------
	if(constructEpoxyGlueLayer) {
		// Lower Glue Layer----------------------------------------------------------------------------------------------------------
		Glue *lowerGlue = new Glue(alpideX, alpideY, glueThickness, epoxyGlue, worldMat);
		Z = 0 - alpideThickness*0.5 - glueThickness*0.5;
		
		if(verboseDetConstr){
			//G4cout << "Lower glue X: " << lowerGlue->GetGlueXDimension()/cm << "cm" << G4endl;
			//G4cout << "Lower glue Y: " << lowerGlue->GetGlueYDimension()/cm << "cm" << G4endl;
			G4cout << "Lower glue Z thickness: " << lowerGlue->GetGlueThickness()/CLHEP::um << " um" << G4endl;
			G4cout << "Lower zInWorld: " << Z/CLHEP::um << " um" << G4endl;
			G4cout << "Lower glue goes from: " << (Z - lowerGlue->GetGlueThickness()*0.5)/um << " um to "
							   << (Z + lowerGlue->GetGlueThickness()*0.5)/um << " um"     <<G4endl;
		}

		G4Box* solidLowerGlue = new G4Box("solidLowerGlue", lowerGlue->GetGlueXDimension()*0.5, 
							       	    lowerGlue->GetGlueYDimension()*0.5, 
							            lowerGlue->GetGlueThickness()*0.5   );
		fLogicLowerGlue = new G4LogicalVolume(solidLowerGlue, lowerGlue->GetGlueMaterial(), "logicLowerGlue", 0);

		// Fill the assembly
		Ta.setX(0.*um); 
		Ta.setY(0.*um);
		Ta.setZ(Z);
		Tr = G4Transform3D(Ra,Ta);
		assemblyDetector->AddPlacedVolume(fLogicLowerGlue, Tr);
		
		// Upper Glue Layer----------------------------------------------------------------------------------------------------------
		Glue *upperGlue = new Glue(alpideX, alpideY, glueThickness, epoxyGlue, worldMat);
		Z = 0 + alpideThickness*0.5 + glueThickness*0.5;
    		
    		if(verboseDetConstr){
    			//G4cout << "Upper glue X: " << upperGlue->GetGlueXDimension()/cm << "cm" << G4endl;
			//G4cout << "Upper glue Y: " << upperGlue->GetGlueYDimension()/cm << "cm" << G4endl;
			G4cout << "Upper glue Z thickness: " << upperGlue->GetGlueThickness()/CLHEP::um << " um" << G4endl;
			G4cout << "Upper zInWorld: " << Z/CLHEP::um << " um" << G4endl;
			G4cout << "Upper glue goes from: " << (Z - upperGlue->GetGlueThickness()*0.5)/um << " um to " 
							   << (Z + upperGlue->GetGlueThickness()*0.5)/um << " um"     << G4endl;
    		}

		G4VSolid* solidUpperGlue = new G4Box("solidGlue", upperGlue->GetGlueXDimension()*0.5, 
								  upperGlue->GetGlueYDimension()*0.5, 
								  upperGlue->GetGlueThickness()*0.5   );
		for(int i = 0; i < NofPads; i++) {
			G4Tubs* solidPadHole = new G4Tubs("solidPadHole", 0., alpide->GetPadRadius(), upperGlue->GetGlueThickness()*0.5, 0., 360.*degree);
			G4ThreeVector* translation = new G4ThreeVector(padCoordinates[i].x() - alpideX/2., 
								       padCoordinates[i].y() - alpideY/2., 
								       0.*um);
			solidUpperGlue = new G4SubtractionSolid("solidUpperGlue", solidUpperGlue, solidPadHole, 0, *translation);

			if(verboseDetConstr){
				G4cout << "solidPadHole Z: " << padCoordinates[i].z() + (alpide->GetAlpideThickness()+upperGlue->GetGlueThickness())*0.5/um 
				       << " um"<<G4endl;
				G4cout << "solidPadHole goes from: " << (padCoordinates[i].z() + (alpide->GetAlpideThickness())*0.5)/um << " um to "
				       << (padCoordinates[i].z() + (alpide->GetAlpideThickness())*0.5+upperGlue->GetGlueThickness())/um << " um" << G4endl;
				//G4cout << "solidGlue volume: " << solidUpperGlue->GetCubicVolume() << G4endl;
			}
		}

		fLogicUpperGlue = new G4LogicalVolume(solidUpperGlue, upperGlue->GetGlueMaterial(), "logicUpperGlue", 0);

		// Fill the assembly
		Ta.setX(0.*um); 
		Ta.setY(0.*um);
		Ta.setZ(Z);
		Tr = G4Transform3D(Ra,Ta);
		assemblyDetector->AddPlacedVolume(fLogicUpperGlue, Tr);
	}
	// Kapton----------------------------------------------------------------------------------------------------------------
	if(constructKaptonLayer) {
		// Lower Kapton Layer--------------------------------------------------------------------------------------------------------
		Kapton *lowerKapton = new Kapton(alpideX, alpideY, kaptonThickness, worldMat);
		Z = 0 - alpideThickness*0.5 - ((constructEpoxyGlueLayer) ? glueThickness : 0.) - kaptonThickness*0.5;
		G4Box* solidLowerKapton = new G4Box("solidLowerKapton", lowerKapton->GetKaptonXDimension()*0.5,
									lowerKapton->GetKaptonYDimension()*0.5,
									lowerKapton->GetKaptonThickness()*0.5  );
		fLogicLowerKapton = new G4LogicalVolume(solidLowerKapton,lowerKapton->GetKaptonMaterial(), "logicLowerKapton", 0);
		
		// Fill the assembly
		Ta.setX(0.*um); 
		Ta.setY(0.*um);
		Ta.setZ(Z);
		Tr = G4Transform3D(Ra,Ta);
		assemblyDetector->AddPlacedVolume(fLogicLowerKapton, Tr);
		
		// Upper Kapton Layer--------------------------------------------------------------------------------------------------------
		Kapton *upperKapton = new Kapton(alpideX, alpideY, kaptonThickness, worldMat);
		Z = 0 + alpideThickness*0.5 + ((constructEpoxyGlueLayer) ? glueThickness : 0.) + kaptonThickness*0.5;
		G4VSolid* solidUpperKapton = new G4Box("solidUpperKapton", upperKapton->GetKaptonXDimension()*0.5,
									   upperKapton->GetKaptonYDimension()*0.5,
									   upperKapton->GetKaptonThickness()*0.5  );
									   
		if(verboseDetConstr){
			G4cout << "solidUpperKaptonThickness: " << upperKapton->GetKaptonThickness()/um << " um" << G4endl;
			G4cout << "solidUpperKapton Z center: " << Z/um << " um" << G4endl;
			G4cout << "solidUpperKapton Z goes from: " << (Z - upperKapton->GetKaptonThickness()*0.5)/um << " um to "
								   << (Z + upperKapton->GetKaptonThickness()*0.5)/um << " um"    << G4endl;
		}
		
		for(int i = 0; i < NofPads; i++){
			G4Tubs* solidPadHole = new G4Tubs("solidPadHole", 0., alpide->GetPadRadius(), upperKapton->GetKaptonThickness()*0.5, 0., 360.*degree);
			G4ThreeVector* translation = new G4ThreeVector(padCoordinates[i].x() - alpideX/2., 
								       padCoordinates[i].y() - alpideY/2., 
								       0.*um);
			solidUpperKapton = new G4SubtractionSolid("solidUpperKapton", solidUpperKapton, solidPadHole, 0, *translation);
			
			if(verboseDetConstr){
				G4cout << "solidPadHoleThickness: " << upperKapton->GetKaptonThickness()/um << " um" << G4endl;
				G4cout << "solidPadHole Z center: " << Z/um << " um" << G4endl;
				G4cout << "solidPadHole Z goes from: " << (Z - upperKapton->GetKaptonThickness()*0.5)/um << " um to "
								       << (Z + upperKapton->GetKaptonThickness()*0.5)/um << " um"     << G4endl;
			}
		}

		fLogicUpperKapton = new G4LogicalVolume(solidUpperKapton,upperKapton->GetKaptonMaterial(), "logicUpperKapton", 0);

		// Fill the assembly by the plates
		Ta.setX(0.*um); 
		Ta.setY(0.*um);
		Ta.setZ(Z);
		Tr = G4Transform3D(Ra,Ta);
		assemblyDetector->AddPlacedVolume(fLogicUpperKapton, Tr);
	}
	// Copper----------------------------------------------------------------------------------------------------------------
	if(constructCopperLayer) {
		Copper *copperLayer = new Copper(alpideX, alpideY, copperThickness);
		Z = 0+alpideThickness*0.5+((constructEpoxyGlueLayer) ? glueThickness : 0.)+((constructKaptonLayer) ? kaptonThickness : 0.)+copperThickness*0.5;
		G4Box* solidCopper = new G4Box("solidCopper", copperLayer->GetCopperXDimension()*0.5, 
							      copperLayer->GetCopperYDimension()*0.5,
							      copperLayer->GetCopperThickness()*0.5   );
		fLogicCopper = new G4LogicalVolume(solidCopper, copperLayer->GetCopperMaterial(), "logicCopper");
		
		if(verboseDetConstr){
			G4cout << "CopperThickness: " << copperLayer->GetCopperThickness()/um << " um" << G4endl;
			G4cout << "Copper Z center: " << Z/um << " um" << G4endl;
			G4cout << "Copper Z goes from: " << (Z - copperLayer->GetCopperThickness()*0.5)/um << " um to " 
							 << (Z + copperLayer->GetCopperThickness()*0.5)/um << " um"	<< G4endl;
		}

		// Fill the assembly by the plates
		Ta.setX(0.*um); 
		Ta.setY(0.*um);
		Ta.setZ(Z);
		Tr = G4Transform3D(Ra,Ta);
		assemblyDetector->AddPlacedVolume(fLogicCopper, Tr);
	}
	// Solder Balls----------------------------------------------------------------------------------------------------------
	if(constructSolderBalls) {
		SolderBall *solderBalls = new SolderBall(alpide);
		Z = alpideThickness*0.5 + ((constructEpoxyGlueLayer) ? glueThickness : 0.)
					+ ((constructKaptonLayer) ? kaptonThickness : 0.) 
					+ ((constructCopperLayer) ? copperThickness : 0.) 
					+ alpidePadRadius;
					
		for(int i = 0; i < NofPads; i++){

			G4Sphere* solidSolderBall = new G4Sphere("solidSolderBall_"+std::to_string(i), 0., alpide->GetPadRadius(), 0., 360.*degree, 
																   0., 360.*degree );
			G4LogicalVolume* logicSolderBall = new G4LogicalVolume(
							solidSolderBall, solderBalls->GetSolderBallMaterial(), "logicSolderBall_"+std::to_string(i));
			fLogicSolderBalls.push_back(logicSolderBall);

			if(verboseDetConstr){
				G4cout << "solidSolderBall Radius: " << alpide->GetPadRadius()/um << " um" << G4endl;
				G4cout << "solidSolderBall Z center: " << Z/um << " um" << G4endl;
				G4cout << "solidSolderBall Z goes from: " << (Z - alpide->GetPadRadius())/um << " um to " 
									  << (Z + alpide->GetPadRadius())/um << " um"     << G4endl;
				//G4cout << "solidSolderBall_" << std::to_string(i) << " volume: " << solidSolderBall->GetCubicVolume() << G4endl;
			}
			
			// Fill the assembly by the plates
			Ta.setX(padCoordinates[i].x() - alpideX/2.); 
			Ta.setY(padCoordinates[i].y() - alpideY/2.);
			Ta.setZ(Z);
			Tr = G4Transform3D(Ra,Ta);
			assemblyDetector->AddPlacedVolume(logicSolderBall, Tr);
		}
		
	}
	//*
	//*
	//*
	
	
	// visualization attributes ------------------------------------------------

	G4VisAttributes invisible(G4VisAttributes::GetInvisible());
	G4VisAttributes invisibleBlue(false, G4Colour::Blue());
	G4VisAttributes invisibleGreen(false, G4Colour::Green());
	G4VisAttributes invisibleYellow(false, G4Colour::Yellow());
	G4VisAttributes blue(G4Colour::Blue());
	G4VisAttributes cgray(G4Colour::Gray());
	G4VisAttributes green(G4Colour::Green());
	G4VisAttributes red(G4Colour::Red());
	G4VisAttributes yellow(G4Colour::Yellow());
	G4VisAttributes brown(G4Colour::Brown());

	fLogicAlpide->SetVisAttributes(yellow);
	for(unsigned int i = 0; i < fLogicalAlpidePad1.size(); i++) fLogicalAlpidePad1[i]->SetVisAttributes(cgray);
	for(unsigned int i = 0; i < fLogicalAlpidePad2.size(); i++) fLogicalAlpidePad2[i]->SetVisAttributes(yellow);
	fLogicLowerGlue->SetVisAttributes(green);
	fLogicUpperGlue->SetVisAttributes(green);
	fLogicLowerKapton->SetVisAttributes(brown);
	fLogicUpperKapton->SetVisAttributes(brown);
	fLogicCopper->SetVisAttributes(red);
	for(unsigned int i = 0; i < fLogicSolderBalls.size(); i++) fLogicSolderBalls[i]->SetVisAttributes(blue);
	
}


// ######## Construction of Sensitive Detector 
void MyDetectorConstruction::ConstructSDandField(){
	
	auto sdManager = G4SDManager::GetSDMpointer();
	G4String SDname;
	
	auto alpide = new MySensitiveDetector(SDname = "/Alpide");
	sdManager->AddNewDetector(alpide);
	fLogicAlpide->SetSensitiveDetector(alpide);
	//StaticInfo::AddOneSensitiveDetector();
	
	if(constructAlpidePads){
		for(unsigned int i = 0; i < fLogicalAlpidePad1.size(); i++){
			auto alpidePad1 = new MySensitiveDetector(SDname = "/"+fLogicalAlpidePad1[i]->GetName());
			sdManager->AddNewDetector(alpidePad1);
			fLogicalAlpidePad1[i]->SetSensitiveDetector(alpidePad1);
			//StaticInfo::AddOneSensitiveDetector();
		}
		
		for(unsigned int i = 0; i < fLogicalAlpidePad2.size(); i++){
			auto alpidePad2 = new MySensitiveDetector(SDname = "/"+fLogicalAlpidePad2[i]->GetName());
			sdManager->AddNewDetector(alpidePad2);
			fLogicalAlpidePad2[i]->SetSensitiveDetector(alpidePad2);
			//StaticInfo::AddOneSensitiveDetector();
		}
	}
	
	if(constructEpoxyGlueLayer) {
		auto lowerGlue = new MySensitiveDetector(SDname = "/LowerGlue");
		sdManager->AddNewDetector(lowerGlue);
		fLogicLowerGlue->SetSensitiveDetector(lowerGlue);
		//StaticInfo::AddOneSensitiveDetector();
		
		auto upperGlue = new MySensitiveDetector(SDname = "/UpperGlue");
		sdManager->AddNewDetector(upperGlue);
		fLogicUpperGlue->SetSensitiveDetector(upperGlue);
		//StaticInfo::AddOneSensitiveDetector();
	}
	
	if(constructKaptonLayer) {
		auto lowerKapton = new MySensitiveDetector(SDname = "/LowerKapton");
		sdManager->AddNewDetector(lowerKapton);
		fLogicLowerKapton->SetSensitiveDetector(lowerKapton);
		//StaticInfo::AddOneSensitiveDetector();
	
		auto upperKapton = new MySensitiveDetector(SDname = "/UpperKapton");
		sdManager->AddNewDetector(upperKapton);
		fLogicUpperKapton->SetSensitiveDetector(upperKapton);
		//StaticInfo::AddOneSensitiveDetector();
	}
	
	if(constructCopperLayer) {
		auto copperLayer = new MySensitiveDetector(SDname = "/CopperLayer");
		sdManager->AddNewDetector(copperLayer);
		fLogicCopper->SetSensitiveDetector(copperLayer);
		//StaticInfo::AddOneSensitiveDetector();
	}
	
	if(constructSolderBalls) {
		for(unsigned int i = 0; i < fLogicSolderBalls.size(); i++){
			auto solderBall = new MySensitiveDetector(SDname = "/"+fLogicSolderBalls[i]->GetName());
			sdManager->AddNewDetector(solderBall);
			fLogicSolderBalls[i]->SetSensitiveDetector(solderBall);
			//StaticInfo::AddOneSensitiveDetector();
		}
	}
	
	if(constructPCB){
		/*if(fPCBUpperLayerLV != nullptr){
			auto PCBUpperLayer = new MySensitiveDetector(SDname = "/PCB_UpperLayer");
			sdManager->AddNewDetector(PCBUpperLayer);
			fPCBUpperLayerLV->SetSensitiveDetector(PCBUpperLayer);
			//StaticInfo::AddOneSensitiveDetector();
		}
		if(fPCBMiddleLayerLV != nullptr){
			auto PCBMiddleLayer = new MySensitiveDetector(SDname = "/PCB_MiddleLayer");
			sdManager->AddNewDetector(PCBMiddleLayer);
			fPCBMiddleLayerLV->SetSensitiveDetector(PCBMiddleLayer);
			//StaticInfo::AddOneSensitiveDetector();
		}
		if(fPCBLowerLayerLV != nullptr){
			auto PCBLowerLayer = new MySensitiveDetector(SDname = "/PCB_LowerLayer");
			sdManager->AddNewDetector(PCBLowerLayer);
			fPCBLowerLayerLV->SetSensitiveDetector(PCBLowerLayer);
			//StaticInfo::AddOneSensitiveDetector();
		}*/
		
		auto LVStore = G4LogicalVolumeStore::GetInstance();
		auto PCB = new PCBSensitiveDetector(SDname = "/PCB");
		sdManager->AddNewDetector(PCB);
		for(logicalVolumeIterator i = LVStore->begin(); i != LVStore->end(); i++){
			if((*i)->GetName().substr(0,8).compare("LV_SOLID") != 0) continue;
			(*i)->SetSensitiveDetector(PCB);
		}	
	}
	
	
}



