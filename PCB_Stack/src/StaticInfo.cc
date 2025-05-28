#include "StaticInfo.hh"

//std::vector<G4ThreeVector> StaticInfo::alpidePadCoordinates;
std::map<G4String, G4bool> StaticInfo::detectorConstructionFlags;
G4int StaticInfo::fRunIdOnMasterThread;
G4int StaticInfo::fNumberOfLayer;
G4double StaticInfo::fScanWidth;

StaticInfo::StaticInfo() {};

StaticInfo::~StaticInfo() {
	Clear();
};

void StaticInfo::Clear() {
	detectorConstructionFlags.clear();
};
