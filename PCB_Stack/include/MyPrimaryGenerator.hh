#ifndef MY_PRIMARY_GENERATOR_HH
#define MY_PRIMARY_GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4Geantino.hh"
#include "G4GenericMessenger.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "StaticInfo.hh"

using namespace CLHEP;

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{

public:

	MyPrimaryGenerator();
	~MyPrimaryGenerator();
	virtual void GeneratePrimaries(G4Event*);
	
	void SetMomentum(G4double val) { fMomentum = val; }
	G4double GetMomentum() const { return fMomentum; }

	void SetSigmaMomentum(G4double val) { fSigmaMomentum = val; }
	G4double GetSigmaMomentum() const { return fSigmaMomentum; }

	void SetSigmaAngle(G4double val) { fSigmaAngle = val; }
	G4double GetSigmaAngle() const { return fSigmaAngle; }

	void SetRandomize(G4bool val) { fRandomizePrimary = val; }
	G4bool GetRandomize() const { return fRandomizePrimary; }
    
private:

	void DefineCommands();

	G4ParticleGun* fParticleGun = nullptr;
	G4GenericMessenger* fMessenger = nullptr;
	G4ParticleDefinition* fPositron = nullptr;
	G4ParticleDefinition* fElectron = nullptr;
	G4ParticleDefinition* fKaon = nullptr;
	G4ParticleDefinition* fNeutron = nullptr;
	G4ParticleDefinition* fProton = nullptr;
	G4double fMomentum = 10.*GeV;
	G4double Y_BeamPosition = -7.4*mm;
	G4double fSigmaMomentum = 1.*MeV;
	G4double fSigmaAngle = 1.*deg;
	G4bool fRandomizePrimary = false;
    
};

#endif
