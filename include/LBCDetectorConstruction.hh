#ifndef LBCDETECTORCONSTRUCTION_HH
#define LBCDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"

#include "CADMesh.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"

#include "LBCSensitiveDetector.hh"


class LBCDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	LBCDetectorConstruction();
	virtual ~LBCDetectorConstruction();

	virtual G4VPhysicalVolume *Construct();

private:
	G4LogicalVolume *logicDetector;

	virtual void ConstructSDandField();
};


#endif
