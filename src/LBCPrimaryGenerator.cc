#include "LBCPrimaryGenerator.hh"

LBCPrimaryGenerator::LBCPrimaryGenerator()
{
	fParticleSource = new G4GeneralParticleSource();


}

LBCPrimaryGenerator::~LBCPrimaryGenerator()
{
	delete fParticleSource;
}

void LBCPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	fParticleSource->GeneratePrimaryVertex(anEvent);
}
