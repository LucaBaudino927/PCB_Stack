#include "MySensitiveDetectorHit.hh"

G4ThreadLocal G4Allocator<MySensitiveDetectorHit>* MySensitiveDetectorHitAllocator;

MySensitiveDetectorHit::MySensitiveDetectorHit(G4int id) : fId(id) {}

G4bool MySensitiveDetectorHit::operator==(const MySensitiveDetectorHit&) const
{
  return false;
}

void MySensitiveDetectorHit::Draw()
{
	auto visManager = G4VVisManager::GetConcreteInstance();
	if (!visManager) return;

	G4Transform3D trans(fRot.inverse(), fPos);
	G4VisAttributes attribs;
	auto pVA = fLogVolume->GetVisAttributes();
	if (pVA) attribs = *pVA;
	attribs.SetColour(G4Colour::Cyan());
	attribs.SetForceSolid(true);
	visManager->Draw(*fLogVolume, attribs, trans);
}


const std::map<G4String, G4AttDef>* MySensitiveDetectorHit::GetAttDefs() const
{
	G4bool isNew;
	auto store = G4AttDefStore::GetInstance("MySensitiveDetectorHit", isNew);

	if (isNew) {
		(*store)["HitType"] = G4AttDef("HitType", "Hit Type", "Physics", "", "G4String");

		(*store)["ID"] = G4AttDef("ID", "ID", "Physics", "", "G4int");

		(*store)["Time"] = G4AttDef("Time", "Time", "Physics", "G4BestUnit", "G4double");

		(*store)["Pos"] = G4AttDef("Pos", "Position", "Physics", "G4BestUnit", "G4ThreeVector");

		(*store)["LVol"] = G4AttDef("LVol", "Logical Volume", "Physics", "", "G4String");
	}
	return store;
}


std::vector<G4AttValue>* MySensitiveDetectorHit::CreateAttValues() const
{
	auto values = new std::vector<G4AttValue>;

	values->push_back(G4AttValue("HitType", "MySensitiveDetectorHit", ""));
	values->push_back(G4AttValue("ID", G4UIcommand::ConvertToString(fId), ""));
	values->push_back(G4AttValue("Time", G4BestUnit(fTime, "Time"), ""));
	values->push_back(G4AttValue("Pos", G4BestUnit(fPos, "Length"), ""));

	if (fLogVolume)
		values->push_back(G4AttValue("LVol", fLogVolume->GetName(), ""));
	else
		values->push_back(G4AttValue("LVol", " ", ""));

	return values;
}


void MySensitiveDetectorHit::Print()
{
	G4cout<<"MySensitiveDetector["<<fLogVolume->GetName() << "]-> time: " << fTime / ns 
			<< " ns; Pos: " << fPos.z()/um << " um; Edep: " << fEdep/keV <<" keV"<< G4endl;
}

