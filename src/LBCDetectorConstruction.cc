#include "LBCDetectorConstruction.hh"

LBCDetectorConstruction::LBCDetectorConstruction()
{
}

LBCDetectorConstruction::~LBCDetectorConstruction()
{
}

G4VPhysicalVolume *LBCDetectorConstruction::Construct()
{
	G4bool checkOverlaps = true;

	//materials definition
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	G4Material *AlMat = nist->FindOrBuildMaterial("G4_Al");
	G4Material *glassMat = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
	G4Material * caseMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
	G4Material *plasticMat = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
	G4Material *reflMat = nist->FindOrBuildMaterial("G4_TEFLON");

	
	//cellulose HPMC
	G4Element *elH = nist->FindOrBuildElement("H");
	G4Element *elC = nist->FindOrBuildElement("C");
	G4Element *elO = nist->FindOrBuildElement("O");
	G4double HPMC_density = 1.33 * g/cm3;
	G4Material *HPMC = new G4Material("HPMC", HPMC_density, 3);
	HPMC->AddElement(elC, 56);
	HPMC->AddElement(elH, 108);
	HPMC->AddElement(elO, 30);

	//compritol 888 ATO
	G4double compritol_density = 1.0 * g/cm3;
	G4Material *compritol = new G4Material("compritol", compritol_density, 3);
	compritol->AddElement(elC, 56);
	compritol->AddElement(elH, 108);
	compritol->AddElement(elO, 30);
	
	//tablet HPMC (0.99) compritol (0.1)
	G4double tablet_density = 1.0 * g/cm3;
	G4Material *tablet = new G4Material("tablet", tablet_density, 2);
	tablet->AddMaterial(HPMC, 0.99);
	tablet->AddMaterial(compritol, 0.01);

	
	//LBC material definition
	G4Element *elLa = nist->FindOrBuildElement("La");
	G4Element *elBr = nist->FindOrBuildElement("Br");
	G4Element *elCe = nist->FindOrBuildElement("Ce");
	G4Element *elCl = nist->FindOrBuildElement("Cl");
	G4double densityLBC = 4.90 * g / cm3; //scionix
	G4int nComponents = 4; 
	G4Material *detMat = new G4Material("LBC", densityLBC, nComponents); 
	detMat->AddElement(elLa, 0.373316); 
	detMat->AddElement(elBr, 0.612028);
	detMat->AddElement(elCl,0.014729);
	detMat->AddElement(elCe, 0.000377);

	//vacuum material
	G4double atomicNumber = 1.;
	G4double massOfMole = 1.008*g/mole;
	G4double density = 1.e-25*g/cm3;
	G4double temperature = 2.73*kelvin;
	G4double pressure = 3.e-18*pascal;
	G4Material *vacuumMat = new G4Material("vacuumMat", atomicNumber, massOfMole, density, kStateGas,temperature, pressure);


	
	// World
	G4double xWorld = 1. * m;
	G4double yWorld = 1. * m;
	G4double zWorld = 1. * m;

	G4Box *solidWorld = new G4Box("solidworld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);


	//Aluminum placement: 46 mm x 43 mm tube
	G4double Al_z = 21.50 * mm;
	G4double Al_rMax = 23.0 * mm;
	G4double Al_thickness = 1.25 * mm;

	G4Tubs *solidAl = new G4Tubs("solidAl", 0., Al_rMax, Al_z, 0. * deg, 360. * deg);
	G4LogicalVolume *logicAl = new G4LogicalVolume(solidAl, AlMat, "logicAl");
	G4VPhysicalVolume *physAl = new G4PVPlacement(0, G4ThreeVector(0., 0., -Al_z - 2.6 * mm), logicAl, "physAl", logicWorld, false, 0 , checkOverlaps);
	

	//Quartz window placement: 46 mm diameter disk with thickness Al_z (43) - LBC_z (38) -  Al_thickness (1.25) - refl_thickness (0.8)
	G4double window_z =  1.475 * mm; 
	G4double window_rMax = Al_rMax - Al_thickness;
	
	G4Tubs *solidWindow = new G4Tubs("solidWindow", 0., window_rMax, window_z, 0. *deg, 360. * deg);
	G4LogicalVolume *logicWindow = new G4LogicalVolume(solidWindow, glassMat, "logicWindow");
	G4VPhysicalVolume *physWindow = new G4PVPlacement(0, G4ThreeVector(0. ,0. , -Al_z + window_z), logicWindow, "physWindow", logicAl, false, 0, checkOverlaps);

	//vacuum
	G4double vac_z = Al_z - window_z - Al_thickness/2.;
	G4double vac_rMax = Al_rMax - Al_thickness;

	G4Tubs *solidVac = new G4Tubs("solidVac", 0., vac_rMax, vac_z, 0. * deg, 360. *deg);
	G4LogicalVolume *logicVac = new G4LogicalVolume(solidVac, vacuumMat, "logicVac");
	G4VPhysicalVolume *physVac = new G4PVPlacement(0, G4ThreeVector(0., 0., window_z - Al_thickness/2.), logicVac, "physVac", logicAl, false, 0, checkOverlaps);

	//reflector placement
	G4double refl_z =  0.4 * mm; //reflector is a 0.8 mm layer outside the LBC crystal (in front)
	G4double refl_rMin = 19 * mm; // LBC crystal radius
	G4double refl_rMax = vac_z; //reflector completely fills lateral space between LBC crystal and aluminum cover
	G4double refl_z2 = 19 * mm; //LBC crystal length

	G4Tubs *solidRefl1 = new G4Tubs("solidRefl1", 0., refl_rMax, refl_z, 0. * deg, 360. * deg);
	G4LogicalVolume *logicRefl1 = new G4LogicalVolume(solidRefl1, reflMat, "logicRefl1");
	G4VPhysicalVolume *physRefl1 = new G4PVPlacement(0, G4ThreeVector(0, 0, vac_z - refl_z), logicRefl1, "physRefl1", logicVac, false, 0, checkOverlaps);

	G4Tubs *solidRefl2 = new G4Tubs("solidRefl2", refl_rMin, refl_rMax, refl_z2, 0. * deg, 360. * deg);
	G4LogicalVolume *logicRefl2 = new G4LogicalVolume(solidRefl2, reflMat, "logicRefl2");
	G4VPhysicalVolume *physRefl2 = new G4PVPlacement(0, G4ThreeVector(0, 0, -refl_z), logicRefl2, "physRefl2", logicVac, false, 0, checkOverlaps);


	//LBC placement
	G4double det_z = 19 * mm;
	G4double det_rMax = 19 * mm;

	G4Tubs *solidDetector = new G4Tubs("solidDetector", 0., det_rMax, det_z, 0. * deg, 360. * deg);
	logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");
	G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0, 0, -refl_z), logicDetector, "physDetector", logicVac, false, 0, checkOverlaps);


	//plastic case placement
	G4RotationMatrix *rotation = new G4RotationMatrix();
	rotation->rotateX(-90. *deg);
	auto mesh = CADMesh::TessellatedMesh::FromSTL("/home/cervig/geant4_LBC/HOLDER_FRONT_CAP.stl");
	auto solidCase = mesh->GetSolid();
	G4LogicalVolume *logicCase = new G4LogicalVolume(solidCase, caseMat, "logicCase");
	G4VPhysicalVolume *physCase = new G4PVPlacement(rotation, G4ThreeVector(0.,0.,-10 * cm), logicCase, "physCase", logicWorld, false, 0, checkOverlaps);

	G4VisAttributes *caseVisAtt = new G4VisAttributes(G4Color(0.5,0.5,0.5));
	caseVisAtt->SetForceWireframe(true);
	logicCase->SetVisAttributes(caseVisAtt);


	
	//source placement
	G4RotationMatrix *rotationSource = new G4RotationMatrix();
    rotationSource->rotateY(90. * deg);
	G4double thickness = 0.55 * mm; 
	G4double r_min_tablet = 0.0 * mm;
	G4double r_max_tablet = 6.5 * mm;
	G4Tubs *solidSource = new G4Tubs("solidSource", r_min_tablet, r_max_tablet, thickness, 0. * deg, 360. * deg);
	G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource, tablet, "logicSource");
	G4VPhysicalVolume *physSource = new G4PVPlacement(rotationSource, G4ThreeVector(-1.3 * cm - thickness, 0., 8.5 * cm), logicSource,"physSource", logicWorld, false, 0, checkOverlaps);
	

	
	return physWorld;

}

void LBCDetectorConstruction::ConstructSDandField()
{
	LBCSensitiveDetector *sensDet = new LBCSensitiveDetector("SensitiveDetector");
	logicDetector->SetSensitiveDetector(sensDet);
	G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}
