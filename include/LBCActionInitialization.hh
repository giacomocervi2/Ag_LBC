#ifndef LBCACTIONINITIALIZATION_HH
#define LBCACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include "LBCPrimaryGenerator.hh"
#include "LBCRunAction.hh"

class LBCActionInitialization : public G4VUserActionInitialization
{
public:
	LBCActionInitialization();
	~LBCActionInitialization();

	virtual void BuildForMaster() const;
	virtual void Build() const;

};


#endif
