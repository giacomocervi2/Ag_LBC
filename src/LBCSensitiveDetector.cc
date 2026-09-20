#include "LBCSensitiveDetector.hh"

LBCSensitiveDetector::LBCSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
	fTotalEnergyDeposited = 0.;
}

LBCSensitiveDetector::~LBCSensitiveDetector()
{
}

void LBCSensitiveDetector::Initialize(G4HCofThisEvent *)
{
	fTotalEnergyDeposited = 0.;
}


G4bool LBCSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
	
	G4double fEnergyDeposited = aStep->GetTotalEnergyDeposit();

	if (fEnergyDeposited > 0.)
	{
		fTotalEnergyDeposited += fEnergyDeposited;
	}

	return true;
}

void LBCSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
if (fTotalEnergyDeposited > 0.)
	{
		G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

		// Riempie l'istogramma 1D (H1) con l'energia totale dell'evento
		analysisManager->FillH1(0, fTotalEnergyDeposited);

		// SALVATAGGIO NTUPLE (TTREE)
		// Salva una singola riga per evento con l'energia totale
		G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
		analysisManager->FillNtupleIColumn(0, 0, eventID);
		analysisManager->FillNtupleDColumn(0, 1, fTotalEnergyDeposited);
		analysisManager->AddNtupleRow(0);
}
}
