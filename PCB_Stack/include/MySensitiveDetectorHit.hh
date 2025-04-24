#ifndef MY_SENSITIVE_DETECTOR_HIT_HH
#define MY_SENSITIVE_DETECTOR_HIT_HH

#include <map>
#include <vector>
#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4RotationMatrix.hh"
#include "G4THitsCollection.hh"
#include "G4Threading.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4LogicalVolume.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"
#include "G4AttDefStore.hh"

/// MySensitiveDetector hit
///
/// It records:
/// - the particle time
/// - the strip logical volume, its position and rotation

class MySensitiveDetectorHit : public G4VHit
{
  public:
    MySensitiveDetectorHit() = default;
    MySensitiveDetectorHit(G4int id);
    MySensitiveDetectorHit(const MySensitiveDetectorHit& right) = default;
    ~MySensitiveDetectorHit() override = default;

    MySensitiveDetectorHit& operator=(const MySensitiveDetectorHit& right) = default;
    G4bool operator==(const MySensitiveDetectorHit& right) const;

    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    void Draw() override;
    const std::map<G4String, G4AttDef>* GetAttDefs() const override;
    std::vector<G4AttValue>* CreateAttValues() const override;
    void Print() override;
    
    G4int GetID() const { return fId; }
    void SetID(G4int id) { fId = id; }

    void SetTime(G4double val) { fTime = val; }
    G4double GetTime() const { return fTime; }

    void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    G4ThreeVector GetPos() const { return fPos; }
    
    void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
    G4RotationMatrix GetRot() const { return fRot; }
    
    void SetEdep(G4double Edep) { fEdep = Edep; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }

    void SetLogVolume(G4LogicalVolume* val) { fLogVolume = val; }
    const G4LogicalVolume* GetLogVolume() const { return fLogVolume; }

  private:
    G4int fId = -1;
    G4double fTime = 0.;
    G4ThreeVector fPos;
    G4RotationMatrix fRot;
    G4double fEdep = 0.;
    const G4LogicalVolume* fLogVolume = nullptr;
};

using MySensitiveDetectorHitsCollection = G4THitsCollection<MySensitiveDetectorHit>;

extern G4ThreadLocal G4Allocator<MySensitiveDetectorHit>* MySensitiveDetectorHitAllocator;

inline void* MySensitiveDetectorHit::operator new(size_t)
{
  if (!MySensitiveDetectorHitAllocator) {
    MySensitiveDetectorHitAllocator = new G4Allocator<MySensitiveDetectorHit>;
  }
  return (void*)MySensitiveDetectorHitAllocator->MallocSingle();
}

inline void MySensitiveDetectorHit::operator delete(void* aHit)
{
  MySensitiveDetectorHitAllocator->FreeSingle((MySensitiveDetectorHit*)aHit);
}

//G4bool MySensitiveDetectorHit::operator==(const MySensitiveDetectorHit& /*right*/) const { return false; }

#endif
