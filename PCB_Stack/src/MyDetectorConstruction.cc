#include "MyDetectorConstruction.hh"


// ######## Constructor 
MyDetectorConstruction::MyDetectorConstruction(std::vector<G4String> GDMLReadStructure){

        // ######## User Defined Messages
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	fMessenger->DeclareProperty("verboseDetConstruction",  	verboseDetConstr, 			"Verbose Detector Construction");
	
	//configurazione per la simulazione del singolo layer
	fMessenger->DeclareProperty("SingleLayer",  		SingleLayer, 				"No stackup, just a single layer of material");
	fMessenger->DeclareProperty("LayerThickness",  		LayerThicknessFromMessenger, 		"LayerThickness");
	fMessenger->DeclareProperty("LayerMaterial",  		LayerMaterialFromMessenger, 		"LayerMaterial with G4Material Notation");	
	
	//configurazione per la simulazione del singolo layer
	fMessenger->DeclareProperty("StackUp",  		StackUp, 				"StackUp simulation");
	fMessenger->DeclareProperty("NofLayers",  		NofLayersFromMessenger, 		"The number of layers in the stackup");
	fMessenger->DeclareProperty("coverlayThickness", 	coverlayThicknessFromMessenger, 	"Coverlay Thickness");
	fMessenger->DeclareProperty("metalThickness",	 	metalThicknessFromMessenger, 		"Metal Thickness");
	fMessenger->DeclareProperty("glueThickness", 		glueThicknessFromMessenger, 		"Glue Thickness");
	fMessenger->DeclareProperty("dielectricThickness", 	dielectricThicknessFromMessenger, 	"Dielectric Thickness");
	fMessenger->DeclareProperty("constructMetal", 		constructMetal, 			"Build Metal Layers");
	fMessenger->DeclareProperty("constructGlue", 		constructGlue, 				"Build Glue Layers");
	fMessenger->DeclareProperty("constructDielectric",    	constructDielectric, 			"Build Dielectric Layers");
	fMessenger->DeclareProperty("constructCoverlay",	constructCoverlay,	 		"Build Coverlay");
	fMessenger->DeclareProperty("coverlayMaterial",  	coverlayMaterialFromMessenger, 		"Coverlay Material with G4Material Notation");
	fMessenger->DeclareProperty("metalMaterial",  		metalMaterialFromMessenger, 		"Metal Material with G4Material Notation");
	fMessenger->DeclareProperty("glueMaterial",  		glueMaterialFromMessenger, 		"Glue Material with G4Material Notation");
	fMessenger->DeclareProperty("dielectricMaterial",  	dielectricMaterialFromMessenger, 	"Dielectric Material with G4Material Notation");
	
	//configurazione per la simulazione della PCB
	fMessenger->DeclareProperty("PCB",	  		PCB, 					"PCB simulation");
	fMessenger->DeclareProperty("NofPCBLayers",  		NofPCBLayersFromMessenger, 		"The number of PCB layers");

        // ######## World dimension definition
	xWorld = 10.*cm;
	yWorld = 10.*cm;
	zWorld = 10.*cm;
	
	fGDMLReadStructure = GDMLReadStructure;
	
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
	//KaptonMaterial = nist->FindOrBuildMaterial("G4_KAPTON");

        // ######## Al
	Al = nist->FindOrBuildMaterial("G4_Al");
	
	// ######## Cu
	Cu = nist->FindOrBuildMaterial("G4_Cu");
	
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
	
	singleLayerMaterial = nist->FindOrBuildMaterial(LayerMaterialFromMessenger);
	coverlayMaterial = nist->FindOrBuildMaterial(coverlayMaterialFromMessenger.empty() ? "G4_KAPTON" : coverlayMaterialFromMessenger);
	metalMaterial = nist->FindOrBuildMaterial(metalMaterialFromMessenger.empty() ? "G4_Al" : metalMaterialFromMessenger);
	glueMaterial = glueMaterialFromMessenger.empty() ? epoxyGlue : nist->FindOrBuildMaterial(glueMaterialFromMessenger);
	dielectricMaterial = nist->FindOrBuildMaterial(dielectricMaterialFromMessenger.empty() ? "G4_KAPTON" : dielectricMaterialFromMessenger);

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
	StaticInfo::AddToDetectorFlagMap("SingleLayer",  		SingleLayer);
	StaticInfo::AddToDetectorFlagMap("StackUp",  			StackUp);
	StaticInfo::AddToDetectorFlagMap("PCB",  			PCB);
	StaticInfo::AddToDetectorFlagMap("constructCoverlay", 		constructCoverlay);
	StaticInfo::AddToDetectorFlagMap("constructMetal", 		constructMetal);
	StaticInfo::AddToDetectorFlagMap("constructGlue", 		constructGlue);
	StaticInfo::AddToDetectorFlagMap("constructDielectric", 	constructDielectric);
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
	
	if(SingleLayer) {
		G4double thickness = (LayerThicknessFromMessenger != 0.) ? LayerThicknessFromMessenger*um : 100.*um;
		ConstructLayer(assemblyDetector, singleLayerMaterial, 0, thickness, 0, "layer");
		// Inserisco l'assembly nel world logical volume
		G4int NofPCBStack = NofLayersFromMessenger > 0 ? NofLayersFromMessenger : 1;
		for(unsigned int i = 0; i < NofPCBStack; i++) {
			// Rotation of the assembly inside the world
			G4RotationMatrix Rm;
			// Translation of the assembly inside the world
			G4double offset = 1*cm;
			G4ThreeVector Tm(0., 0., 0. + i*offset);
			G4Transform3D Tr = G4Transform3D(Rm,Tm);
			assemblyDetector->MakeImprint(logicWorld, Tr);
		}
	} else if(StackUp){
		ConstructStackup(assemblyDetector);
		// Inserisco l'assembly nel world logical volume
		G4int NofPCBStack = NofLayersFromMessenger > 0 ? NofLayersFromMessenger : 1;
		for(unsigned int i = 0; i < NofPCBStack; i++) {
			// Rotation of the assembly inside the world
			G4RotationMatrix Rm;
			// Translation of the assembly inside the world
			G4double offset = 1*cm;
			G4ThreeVector Tm(0., 0., 0. + i*offset);
			G4Transform3D Tr = G4Transform3D(Rm,Tm);
			assemblyDetector->MakeImprint(logicWorld, Tr);
		}
	} else if(PCB){
		ConstructPCB(assemblyDetector);
		// Inserisco l'assembly nel world logical volume
		G4int NofPCBStack = NofPCBLayersFromMessenger > 0 ? NofPCBLayersFromMessenger : 1;
		for(unsigned int i = 0; i < NofPCBStack; i++) {
			// Rotation of the assembly inside the world
			G4RotationMatrix Rm;
			// Translation of the assembly inside the world
			G4double offset = 0.5*cm;
			G4ThreeVector Tm(0., 0., 0. + i*offset);
			G4Transform3D Tr = G4Transform3D(Rm,Tm);
			assemblyDetector->MakeImprint(logicWorld, Tr);
		}
	} else throw std::domain_error("Type of simulation not specified in the macro file");		
	
        return physWorld;//mother volume
}

void MyDetectorConstruction::ConstructLayer(G4AssemblyVolume* assemblyDetector, G4Material* material, G4double Z, G4double thickness, G4int layerID, G4String type)
{
	// Rotation and translation of a layer inside the assembly
	G4RotationMatrix Ra;
	G4ThreeVector Ta;
	G4Transform3D Tr;
	G4Box* SV = new G4Box("SV"+type+std::to_string(layerID), 2.*cm, 2.*cm, thickness*0.5);
	fLogicVolumeList.push_back(new G4LogicalVolume(SV, material, "LV"+type+std::to_string(layerID)));

	// Fill the assembly by the plates
	Ta.setX(0.*um); 
	Ta.setY(0.*um);
	Ta.setZ(Z);
	Tr = G4Transform3D(Ra,Ta);
	assemblyDetector->AddPlacedVolume(fLogicVolumeList.back(), Tr);
}

void MyDetectorConstruction::ConstructStackup(G4AssemblyVolume* assemblyDetector) {

	coverlayThickness 	=	(coverlayThicknessFromMessenger != 0.) 		? coverlayThicknessFromMessenger*um 	: 100.*um;
	metalThickness	 	=	(metalThicknessFromMessenger != 0.) 		? metalThicknessFromMessenger*um  	: 100.*um;
	glueThickness	 	=	(glueThicknessFromMessenger != 0.) 		? glueThicknessFromMessenger*um   	: 100.*um;
	dielectricThickness 	=	(dielectricThicknessFromMessenger != 0.) 	? dielectricThicknessFromMessenger*um 	: 100.*um;
	
	double Z = 0.;
	//each layer is made of 2 metal layers and one polyimide layer
	//if NofDielectricLayers == 0 ==> Al,Glue,Al. if NofDielectricLayers == 1 ==> Al,Glue,Al, kapton, Al,Glue,Al
	//if NofLayersFromMessenger == 2 ==> NofDielectricLayers = NofLayersFromMessenger - 1
	if(constructCoverlay){
		ConstructLayer(assemblyDetector, coverlayMaterial, Z, coverlayThickness, 0, "coverlay");
		if(verboseDetConstr) G4cout << G4endl << "Coverlay built at position Z = " << Z << G4endl;
		Z += coverlayThickness;
	}
        for(int i = 0; i < NofLayersFromMessenger; i++){
        	if(constructMetal){
        		ConstructLayer(assemblyDetector, metalMaterial, Z, metalThickness, i+1, "metal1_");
        		if(verboseDetConstr) G4cout << G4endl << "Metal layer built at position Z = " << Z << G4endl;
        		Z += metalThickness;
        	}
        	if(constructGlue){
        		ConstructLayer(assemblyDetector, glueMaterial, Z, glueThickness, i+1, "glue");
        		if(verboseDetConstr) G4cout << G4endl << "Glue layer built at position Z = " << Z << G4endl;
        		Z += glueThickness;
        	}
        	if(constructMetal){
        		ConstructLayer(assemblyDetector, metalMaterial, Z, metalThickness, i, "metal2_");
        		if(verboseDetConstr) G4cout << G4endl << "Metal layer built at position Z = " << Z << G4endl;
        		Z += metalThickness;
        	}
        	if(constructDielectric){
        		if(i != NofLayersFromMessenger - 1){
				ConstructLayer(assemblyDetector, dielectricMaterial, Z, dielectricThickness, i+1, "dielectric");
				if(verboseDetConstr) G4cout << G4endl << "Dielectric layer built at position Z = " << Z << G4endl;
				Z += dielectricThickness;
			}
        	}
        }
        if(constructCoverlay){
        	ConstructLayer(assemblyDetector, coverlayMaterial, Z, coverlayThickness, 999, "coverlay");
        	if(verboseDetConstr) G4cout << G4endl << "Coverlay built at position Z = " << Z << G4endl;
        }
        
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

	for(int i = 0; i < fLogicVolumeList.size(); i++){
		if(!(fLogicVolumeList[i]->GetName().find("coverlay")   == std::string::npos)) 	fLogicVolumeList[i]->SetVisAttributes(green);
		if(!(fLogicVolumeList[i]->GetName().find("metal")      == std::string::npos))   fLogicVolumeList[i]->SetVisAttributes(cgray);
		if(!(fLogicVolumeList[i]->GetName().find("glue")       == std::string::npos))   fLogicVolumeList[i]->SetVisAttributes(yellow);
		if(!(fLogicVolumeList[i]->GetName().find("dielectric") == std::string::npos))	fLogicVolumeList[i]->SetVisAttributes(brown);
	}
}

void MyDetectorConstruction::ConstructPCB(G4AssemblyVolume* assemblyDetector)
{
	G4AssemblyStore* assemblyStore = G4AssemblyStore::GetInstance();
	/*
	for(assemblyIterator i = assemblyStore->begin(); i != assemblyStore->end(); i++){
		G4cout<<"---ConstructPCB---GetAssemblyID(): "<<std::to_string((*i)->GetAssemblyID())<<G4endl;
	}
	*/
	
	G4GDMLParser parser;
	// Uncomment the following if wish to avoid names stripping
	// parser.SetStripFlag(false);
	// Uncomment the following and set a string with proper absolute path and
	// schema filename if wishing to use alternative schema for parsing validation
	// parser.SetImportSchema("");
	parser.SetOverlapCheck(true);
	
	G4RotationMatrix Ra;
	Ra.rotateZ(270*deg); // Ruota attorno a Z -> Ruota piano XY
	G4ThreeVector Ta;
	G4Transform3D Tr;
	
	
	//Parsing of PCB_UpperLayer--------------------------------------------------------------------------
	if(verboseDetConstr) G4cout << "Parsing of PCB_UpperLayer" << G4endl;
	parser.Read(fGDMLReadStructure[0]);
	G4AssemblyVolume* assemblyPCBUpperLayer = assemblyStore->GetAssembly(2); //AssemblyID = 2

	Ta.setX(0.*mm);
	Ta.setY(0.*mm);
	Ta.setZ(0.2*mm);
	Tr = G4Transform3D(Ra,Ta);
	assemblyDetector->AddPlacedAssembly(assemblyPCBUpperLayer, Tr);
	if(verboseDetConstr) G4cout << "End of PCB_UpperLayer parsing" << G4endl;
	parser.Clear();
	
	/*
	//Parsing of PCB_MiddleLayer-------------------------------------------------------------------------
	if(verboseDetConstr) G4cout << "Parsing of PCB_MiddleLayer" << G4endl;
	parser.Read(fGDMLReadStructure[1]);
	G4AssemblyVolume* assemblyPCBMiddleLayer = assemblyStore->GetAssembly(3); //AssemblyID = 3

	Ta.setX(0.*mm);
	Ta.setY(0.*mm);
	Ta.setZ(-20.8*mm);
	Tr = G4Transform3D(Ra,Ta);	
	assemblyDetector->AddPlacedAssembly(assemblyPCBMiddleLayer, Tr);
	if(verboseDetConstr) G4cout << "End of PCB_MiddleLayer parsing" << G4endl;
	parser.Clear();
	
	//Parsing of PCB_LowerLayer--------------------------------------------------------------------------
	if(verboseDetConstr) G4cout << "Parsing of PCB_LowerLayer" << G4endl;
	parser.Read(fGDMLReadStructure[2]);
	G4AssemblyVolume* assemblyPCBLowerLayer = assemblyStore->GetAssembly(4); //AssemblyID = 4

	Ta.setX(0.*mm); 
	Ta.setY(0.*mm);
	Ta.setZ(13.8*mm);
	Tr = G4Transform3D(Ra,Ta);	
	assemblyDetector->AddPlacedAssembly(assemblyPCBLowerLayer, Tr);
	if(verboseDetConstr) G4cout << "End of PCB_LowerLayer parsing" << G4endl;
	parser.Clear();
	*/
	
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

	//fPCBUpperLayerLV->SetVisAttributes(yellow);
	//fPCBMiddleLayerLV->SetVisAttributes(red);
	//fPCBLowerLayerLV->SetVisAttributes(green);
}

// ######## Construction of Sensitive Detector 
void MyDetectorConstruction::ConstructSDandField(){
	
	auto sdManager = G4SDManager::GetSDMpointer();
	G4String SDname;
	
	if(SingleLayer || StackUp){
		for(int i = 0; i < fLogicVolumeList.size(); i++){
			auto sensitiveDetector = new MySensitiveDetector(SDname = "/"+fLogicVolumeList[i]->GetName());
			sdManager->AddNewDetector(sensitiveDetector);
			fLogicVolumeList[i]->SetSensitiveDetector(sensitiveDetector);
		}
	}
	
}



