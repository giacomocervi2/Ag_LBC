#ifndef LBCPRIMARYGENERATOR_HH
#define LBCPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4GeneralParticleSource.hh"


class LBCPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
	LBCPrimaryGenerator();
	~LBCPrimaryGenerator();

	virtual  void GeneratePrimaries(G4Event *);
private:
	G4GeneralParticleSource * fParticleSource;
};


#endif
