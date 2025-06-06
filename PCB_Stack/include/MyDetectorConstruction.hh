#ifndef MY_DETECTOR_CONSTRUCTION_HH
#define MY_DETECTOR_CONSTRUCTION_HH

#include "G4SystemOfUnits.hh"
#include <iostream>
#include <vector>
#include <iterator>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4GenericMessenger.hh"
#include "G4Tubs.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4MTRunManager.hh"
#include "G4SDManager.hh"
#include "G4AssemblyVolume.hh"
#include "G4Threading.hh"
#include "G4AssemblyStore.hh"
#include "G4GDMLParser.hh"

//User definted
#include "MySensitiveDetector.hh"
#include "PCBSensitiveDetector.hh"
#include "Glue.hh"
#include "Alpide.hh"
#include "Kapton.hh"
#include "Copper.hh"
#include "SolderBall.hh"
#include "StaticInfo.hh"
#include "Constants.hh"
#include "G4LogicalVolumeStore.hh"

class G4FieldManager;
class G4VPhysicalVolume;
class G4GenericMessenger;

typedef std::vector<G4LogicalVolume*>::iterator logicalVolumeIterator;

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
    public:

        MyDetectorConstruction(std::vector<G4String>);
        ~MyDetectorConstruction() override;
        G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }//energy deposit
        G4VPhysicalVolume* Construct() override;
        void ConstructSDandField() override;
        

    private:
   
        void DefineMaterials();
        void ConstructLayer(G4AssemblyVolume* assemblyDetector, G4Material* material, G4double Z, G4double thickness, G4int layerID, G4String type);
	void ConstructStackup(G4AssemblyVolume* assemblyDetector);
	void ConstructPCB(G4AssemblyVolume* assemblyDetector);
        
        G4GenericMessenger *fMessenger = nullptr;
        G4double xWorld, yWorld, zWorld;
        G4Box *solidWorld;
        G4LogicalVolume *logicWorld;
        G4VPhysicalVolume  *physWorld;
        G4LogicalVolume *fScoringVolume;
        
        G4Material *worldMat		 = nullptr;
        G4Material *KaptonMaterial	 = nullptr;
        G4Material *Si			 = nullptr;
        G4Material *Al			 = nullptr;
        G4Material *epoxyGlue		 = nullptr;
        G4Material *Cu			 = nullptr;
        G4Element *C			 = nullptr;
        G4Element *Na			 = nullptr;
        G4Element *I			 = nullptr;
        G4Element *N			 = nullptr;
        G4Element *O			 = nullptr;
        G4Element *H			 = nullptr;
        
        // G4GenericMessenger settings
        G4bool verboseDetConstr = false;
        //flags to declare the setup of the detector
        G4bool constructMetal, constructGlue, constructDielectric, constructCoverlay, SingleLayer, StackUp, PCB, CustomPCB;
        //flags to tune the setup: if they are specified in the macro -> use those values otherwise use default values
        G4double NofLayersFromMessenger 		= 0.;
        G4double coverlayThicknessFromMessenger 	= 0.;
        G4double metalThicknessFromMessenger 		= 0.;
        G4double glueThicknessFromMessenger 		= 0.;
        G4double dielectricThicknessFromMessenger 	= 0.;
        G4double LayerThicknessFromMessenger	 	= 0.;
        
        G4double coverlayThickness 			= 0.;
	G4double metalThickness	 			= 0.;
	G4double glueThickness	 			= 0.;
	G4double dielectricThickness			= 0.;
        
        //Logical Volumes
        std::vector<G4LogicalVolume*> fLogicVolumeList;
        
        G4String LayerMaterialFromMessenger;
        G4String coverlayMaterialFromMessenger;
        G4String metalMaterialFromMessenger;
        G4String glueMaterialFromMessenger;
        G4String dielectricMaterialFromMessenger;
        
        G4Material *singleLayerMaterial	 = nullptr;
        G4Material *coverlayMaterial	 = nullptr;
        G4Material *metalMaterial	 = nullptr;
        G4Material *glueMaterial	 = nullptr;
        G4Material *dielectricMaterial	 = nullptr;
        
        G4double NofPCBLayersFromMessenger 		= 0.;
        G4double NofCustomPCBLayersFromMessenger	= 0.;
        G4double AlpideThicknessFromMessenger		= 0.;
        G4double AlThicknessFromMessenger		= 0.;
        G4double KaptonThicknessFromMessenger		= 0.;
        G4double GlueThicknessFromMessenger		= 0.;
        
        std::vector<G4String> fGDMLReadStructure;
        
        
};

#endif
