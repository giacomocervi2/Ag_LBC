#ifndef	LBCSENSITIVEDETECTOR_HH
#define LBCSENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class LBCSensitiveDetector : public G4VSensitiveDetector
{
public:
	LBCSensitiveDetector(G4String);
	~LBCSensitiveDetector();

private:
	G4double fTotalEnergyDeposited;

	virtual void Initialize(G4HCofThisEvent *) override; //HC stands for Hit Collection (important only if you have to do recontruction or analysis, like including electronic noise, dead time, but you still need to include it
	virtual void EndOfEvent(G4HCofThisEvent *) override;

	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};


#endif
