#include <iostream>
#include <vector>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4MTRunManager.hh"
#include "FTFP_BERT.hh"//Standard Physics List for collider use and energy > 1 GeV
//#include "QGSP_BERT.hh"//old library replaced by FTFP_BERT
#include "QBBC.hh"//Physics List for energy < 1 GeV. It is identical to FTFP_BERT for higher energy

// User defined
#include "MyDetectorConstruction.hh"
#include "MyActionInitialization.hh"

//main take directly argc and argv from the command line -> ./sim ciccio.mac -> argc = 2, argv = {./sim, ciccio.mac}. argv[1]=ciccio.mac
int main(int argc, char** argv){

	if (argc < 8) {
	    G4cout << "Error! Mandatory input files are not specified!" << G4endl;
	    G4cout << G4endl;
	    return -1;
  	}
  
    	// ######## runManager initialization. 
        
        #ifdef G4MULTITHREADED
            G4MTRunManager* runManager = new G4MTRunManager;
        #else
            G4RunManager* runManager = new G4RunManager;
        #endif
        
        // ######## This is the real simulation architecture construction 
        std::vector<G4String> gdmlFileNames;
        gdmlFileNames.push_back(argv[1]);
        gdmlFileNames.push_back(argv[2]);
        gdmlFileNames.push_back(argv[3]);
        gdmlFileNames.push_back(argv[4]);
        gdmlFileNames.push_back(argv[5]);
        gdmlFileNames.push_back(argv[6]);
        gdmlFileNames.push_back(argv[7]);
	G4cout<<"test"<<G4endl;
        //First: construction -> creates detectors
        runManager->SetUserInitialization(new MyDetectorConstruction(gdmlFileNames));
        //Second: physics
        G4VModularPhysicsList *physicsList = new QBBC();
	    physicsList->SetVerboseLevel(0);
	    runManager->SetUserInitialization(physicsList);
        //Third: action -> 1. generator, 2. event, 3. run, 4. stepping 
        runManager->SetUserInitialization(new MyActionInitialization);

        //Only for cosmic showers in the atmosphere. It takes into account hadronic decays. 
        //G4VModularPhysicsList *physics = new QGSP_BERT();
        //physics->RegisterPhysics(new G4DecayPhysics());//decadimento dei pioni, mesoni, ecc...
        //runManager->SetUserInitialization(physics);

    	// ######## UI interface initialization. Argc = number of argument calls when writing ./sim in the terminal. ./sim ciccio.mac -> argc=2. 

        G4UIExecutive* ui = 0;

        if(argc==8){ 
            ui = new G4UIExecutive(argc, argv);
        }
	
        G4VisManager * visManager = new G4VisExecutive();
        visManager->Initialize(); 
        G4UImanager* UImanager = G4UImanager::GetUIpointer();  

        if(ui){//no argument after ./sim
            UImanager->ApplyCommand("/control/execute vis.mac");
            if (ui->IsGUI()) {
      		UImanager->ApplyCommand("/control/execute gui.mac");
    	    }
            ui->SessionStart();
        }else{//a file is called after ./sim, e.g. ./sim ciccio.mac
            G4String command = "/control/execute ";
            G4String fileName = argv[8];
            UImanager->ApplyCommand(command+fileName);
        }

        return 0;
}
