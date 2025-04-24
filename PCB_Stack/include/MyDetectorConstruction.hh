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

        MyDetectorConstruction();
        ~MyDetectorConstruction() override;
        G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }//energy deposit
        G4VPhysicalVolume* Construct() override;
        void ConstructSDandField() override;
        

    private:
   
        void DefineMaterials();
        void ConstructMapsFoil(G4AssemblyVolume* assemblyDetector);
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
        G4Element *C			 = nullptr;
        G4Element *Na			 = nullptr;
        G4Element *I			 = nullptr;
        G4Element *N			 = nullptr;
        G4Element *O			 = nullptr;
        G4Element *H			 = nullptr;
        
        // G4GenericMessenger settings
        G4bool verboseDetConstr = false;
        //flags to declare the detector
        G4bool isMapsFoil;
        //flags to declare the setup of the detector
        G4bool constructAlpidePads, constructEpoxyGlueLayer, constructKaptonLayer, constructCopperLayer, constructSolderBalls, constructPCB;
        //flags to tune the setup: if they are specified in the macro -> use those values otherwise use default values
        G4double alpideXFromMessenger 		= 0.;
        G4double alpideYFromMessenger 		= 0.;
        G4double alpideThicknessFromMessenger 	= 0.;
        G4double alpidePadRadiusFromMessenger 	= 0.;
        G4double glueThicknessFromMessenger 	= 0.;
        G4double kaptonThicknessFromMessenger 	= 0.;
        G4double copperThicknessFromMessenger 	= 0.;
        
        //Logical Volumes
        G4LogicalVolume *fLogicAlpide 		= nullptr;
        G4LogicalVolume *fLogicLowerGlue 	= nullptr;
        G4LogicalVolume *fLogicUpperGlue 	= nullptr;
        G4LogicalVolume *fLogicLowerKapton 	= nullptr;
        G4LogicalVolume *fLogicUpperKapton 	= nullptr;
        G4LogicalVolume *fLogicCopper		= nullptr;
        //G4LogicalVolume *fPCBUpperLayerLV	= nullptr;
        //G4LogicalVolume *fPCBMiddleLayerLV	= nullptr;
        //G4LogicalVolume *fPCBLowerLayerLV	= nullptr;
        std::vector<G4LogicalVolume*> fLogicalAlpidePad1;
        std::vector<G4LogicalVolume*> fLogicalAlpidePad2;
        std::vector<G4LogicalVolume*> fLogicSolderBalls;
               

};

#endif
