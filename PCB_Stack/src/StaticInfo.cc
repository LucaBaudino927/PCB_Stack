#include "StaticInfo.hh"

//std::vector<G4ThreeVector> StaticInfo::alpidePadCoordinates;
std::map<G4String, G4bool> StaticInfo::detectorConstructionFlags;
G4int StaticInfo::fRunIdOnMasterThread;

StaticInfo::StaticInfo() {};

StaticInfo::~StaticInfo() {
	Clear();
};

void StaticInfo::Clear() {
	detectorConstructionFlags.clear();
};
