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

	//Tablet material definition: sorbitol
	G4Element *elH = nist->FindOrBuildElement("H");
	G4Element *elC = nist->FindOrBuildElement("C");
	G4Element *elO = nist->FindOrBuildElement("O");
	G4double sorb_density = 1.49 * g/cm3;
	G4Material *sorbitol = new G4Material("sorbitol", sorb_density, 3);
	sorbitol->AddElement(elC, 6);
	sorbitol->AddElement(elH, 14);
	sorbitol->AddElement(elO, 6);

	//cellulose HPMC
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
	detMat->AddElement(elLa, 0.385085); 
	detMat->AddElement(elBr, 0.631321);
	detMat->AddElement(elCl,0.014729);
	detMat->AddElement(elCe, 0.000389);

	
	//world
	G4double xWorld = 1. * m;
	G4double yWorld = 1. * m;
	G4double zWorld = 1. * m;

	G4Box *solidWorld = new G4Box("solidworld",0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalVolume");
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);



	//Aluminum placement
	G4double Al_z = 21.5 * mm;
	G4double Al_rMin = 0. * mm;
	G4double Al_rMax = 23 * mm;
	G4double Al_SPhi = 0. * deg;
	G4double Al_DPhi = 360. * deg;
	
	G4Tubs *solidAl = new G4Tubs("solidAl", Al_rMin, Al_rMax, Al_z, Al_SPhi, Al_DPhi);
	G4LogicalVolume *logicAl = new G4LogicalVolume(solidAl, AlMat, "logicAl");
	G4VPhysicalVolume *physAl = new G4PVPlacement(0, G4ThreeVector(0., 0., -Al_z - 2. * mm), logicAl, "physAl", logicWorld, false, 0, checkOverlaps); //0.2 è lo spessore del case

	G4VisAttributes *AlVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5));
	AlVisAtt->SetForceSolid(true);
	logicAl->SetVisAttributes(AlVisAtt);

	
	
	
	//LBC crystal placement
	G4double LBC_z = 19 * mm;
	G4double LBC_rMin = 0. * mm;
	G4double LBC_rMax = 19 * mm;
	G4double LBC_SPhi = 0. * deg;
	G4double LBC_DPhi = 360. * deg;
	
	G4Tubs *solidDetector = new G4Tubs("solidDetector", LBC_rMin, LBC_rMax, LBC_z, LBC_SPhi, LBC_DPhi);
	logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");
	G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicDetector, "physDetector", logicAl, false, 0, checkOverlaps);

	G4VisAttributes *detVisAtt = new G4VisAttributes(G4Color(1.0, 1., 0.0, 1));
	detVisAtt->SetForceSolid(true);
	logicDetector->SetVisAttributes(detVisAtt);



	//source placement
	G4RotationMatrix *rotationSource = new G4RotationMatrix();
    rotationSource->rotateY(90. * deg);
	G4double thickness = 0.55 * mm; 
	G4double r_min_tablet = 0.0 * mm;
	G4double r_max_tablet = 6.5 * mm;
	G4Tubs *solidSource = new G4Tubs("solidSource", r_min_tablet, r_max_tablet, thickness, 0. * deg, 360. * deg);
	G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource, tablet, "logicSource");
	G4VPhysicalVolume *physSource = new G4PVPlacement(rotationSource, G4ThreeVector(-1.3 * cm - thickness, 0., 8.5 * cm), logicSource,"physSource", logicWorld, false, 0, checkOverlaps);



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
	
	return physWorld;

}

void LBCDetectorConstruction::ConstructSDandField()
{
	LBCSensitiveDetector *sensDet = new LBCSensitiveDetector("SensitiveDetector");
	logicDetector->SetSensitiveDetector(sensDet);
	G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}
