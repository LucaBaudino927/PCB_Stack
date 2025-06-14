#include "MyPrimaryGenerator.hh"

MyPrimaryGenerator::MyPrimaryGenerator(){
	
	G4int nofParticles = 1; //# of primary particle per event
	fParticleGun = new G4ParticleGun(nofParticles); 
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	
	fProton = particleTable->FindParticle("proton");
	fElectron = particleTable->FindParticle("e-");
	fPositron = particleTable->FindParticle("e+");
	fKaon = particleTable->FindParticle("kaon+");
	fNeutron = particleTable->FindParticle("neutron");
	fGeantino = particleTable->FindParticle("geantino");

	fParticleGun->SetParticlePosition(G4ThreeVector(X_BeamPosition, Y_BeamPosition, -2.*cm));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
	//fParticleGun->SetParticleMomentum(fMomentum);

	// default particle: proton, p = 10 GeV, Ekin = 9 GeV (it is a MIP proton with this energy -> dE/dx = 2 MeV/gcm^-2)
	auto protonMass = fProton->GetPDGMass();
	auto protonKinEnergy = std::sqrt(fMomentum * fMomentum + protonMass * protonMass) - protonMass;
	fParticleGun->SetParticleEnergy(protonKinEnergy);
	fParticleGun->SetParticleDefinition(fProton);

	// define commands for this class
	DefineCommands();
	
	fScanWidth = 10.*mm;

}

// ######## Destructor 
MyPrimaryGenerator::~MyPrimaryGenerator()
{
  delete fParticleGun;
  delete fMessenger;
}

// ######## Generate primary particle 
void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent){

        G4ParticleDefinition* particle;
	if (fRandomizePrimary) {
		auto i = (int)(5. * G4UniformRand());
		switch (i) {
			case 0:
				particle = fProton;
				break;
			case 1:
				particle = fPositron;
				break;
			case 2:
				particle = fKaon;
				break;
			case 3:
				particle = fNeutron;
				break;
			default:
				particle = fElectron;
				break;
		}
		fParticleGun->SetParticleDefinition(particle);
		fParticleGun->SetParticlePosition(G4ThreeVector(X_BeamPosition, Y_BeamPosition, -2.*cm));
		auto pp = fMomentum + (G4UniformRand() - 0.5) * fSigmaMomentum;
		//fParticleGun->SetParticleMomentum(pp);
		auto mass = particle->GetPDGMass();
		auto ekin = std::sqrt(pp * pp + mass * mass) - mass;
		fParticleGun->SetParticleEnergy(ekin);
		auto angle = (G4UniformRand() - 0.5) * fSigmaAngle;
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(angle), 0., std::cos(angle)));
	} else {
		particle = fParticleGun->GetParticleDefinition();
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
		//fParticleGun->SetParticleMomentum(fMomentum);
		auto Mass = particle->GetPDGMass();
		auto KinEnergy = std::sqrt(fMomentum * fMomentum + Mass * Mass) - Mass;
		//G4cout<<"---GeneratePrimaries()---fMomentum = "<<fMomentum/GeV<<" GeV, KinEnergy = "<<KinEnergy/GeV<<" GeV"<<G4endl;
		fParticleGun->SetParticleEnergy(KinEnergy);	
		fParticleGun->SetParticlePosition(G4ThreeVector(X_BeamPosition, Y_BeamPosition, -2.*cm));
	}
		
	if(fRandomizeBeamPosition){
		auto x = X_BeamPosition + (G4UniformRand() - 0.5) * fScanWidth;
		auto y = Y_BeamPosition + (G4UniformRand() - 0.5) * fScanWidth;
		fParticleGun->SetParticlePosition(G4ThreeVector(x, y, -2.*cm));
	}
	
        fParticleGun->GeneratePrimaryVertex(anEvent);
        
}

void MyPrimaryGenerator::DefineCommands()
{
	// Define /generator command directory using generic messenger class
	fMessenger = new G4GenericMessenger(this, "/generator/", "Primary generator control");

	// randomizePrimary command
	auto& randomCmd = fMessenger->DeclareProperty("randomizePrimary", fRandomizePrimary);
	G4String guidance = "Boolean flag for randomizing primary particle types.\n";
	guidance 	 += "In case this flag is false, you can select the primary particle\n";
	guidance	 += " with /gun/particle command.";
	randomCmd.SetGuidance(guidance);
	randomCmd.SetParameterName("flag", true);
	randomCmd.SetDefaultValue("false");
	
	// momentum command
	auto& momentumCmd = fMessenger->DeclarePropertyWithUnit("momentum", "GeV", fMomentum, "Mean momentum of primaries in MeV");
	momentumCmd.SetParameterName("p", true);
	momentumCmd.SetRange("p>=0.");
	momentumCmd.SetDefaultValue("10.");

	// sigmaMomentum command
	auto& sigmaMomentumCmd = fMessenger->DeclarePropertyWithUnit("sigmaMomentum", "MeV", fSigmaMomentum, "Sigma momentum of primaries in MeV");
	sigmaMomentumCmd.SetParameterName("sp", true);
	sigmaMomentumCmd.SetRange("sp>=0.");
	sigmaMomentumCmd.SetDefaultValue("1.");

	// sigmaAngle command
	auto& sigmaAngleCmd = fMessenger->DeclarePropertyWithUnit("sigmaAngle", "deg", fSigmaAngle, "Sigma angle divergence of primaries in degree");
	sigmaAngleCmd.SetParameterName("t", true);
	sigmaAngleCmd.SetRange("t>=0.");
	sigmaAngleCmd.SetDefaultValue("1.");
	
	// beamYPosition command
	auto& beamYPositionCmd = fMessenger->DeclarePropertyWithUnit("beamYPosition", "mm", Y_BeamPosition, "Y coordinate of the beam in mm");
	beamYPositionCmd.SetParameterName("Ybp", true);
	//beamYPositionCmd.SetRange("Ybp>=0.");
	beamYPositionCmd.SetDefaultValue("0");
	
	// beamXPosition command
	auto& beamXPositionCmd = fMessenger->DeclarePropertyWithUnit("beamXPosition", "mm", X_BeamPosition, "X coordinate of the beam in mm");
	beamXPositionCmd.SetParameterName("Xbp", true);
	//beamXPositionCmd.SetRange("Xbp>=0.");
	beamXPositionCmd.SetDefaultValue("0");
	
	// randomizeBeamPosition command
	auto& randomBeamPosCmd = fMessenger->DeclareProperty("randomizeBeamPosition", fRandomizeBeamPosition, "Boolean flag for randomize particle beam position");
	randomBeamPosCmd.SetParameterName("flag", true);
	randomBeamPosCmd.SetDefaultValue("false");
	
	// scanWidth command
	auto& scanWidthCmd = fMessenger->DeclarePropertyWithUnit("scanWidth", "mm", fScanWidth, "Width of the possible beam positions.");
	scanWidthCmd.SetParameterName("scanW", true);
	scanWidthCmd.SetRange("scanW>=0.");
	scanWidthCmd.SetDefaultValue("10");
  
}

