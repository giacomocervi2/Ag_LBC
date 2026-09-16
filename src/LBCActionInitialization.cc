#include "LBCActionInitialization.hh"

LBCActionInitialization::LBCActionInitialization()
{
}


LBCActionInitialization::~LBCActionInitialization()
{
}

void LBCActionInitialization::BuildForMaster() const
{
	LBCRunAction *runAction = new LBCRunAction();
	SetUserAction(runAction);
}

void LBCActionInitialization::Build() const
{
	LBCPrimaryGenerator *generator = new LBCPrimaryGenerator();
	SetUserAction(generator);

	LBCRunAction *runAction = new LBCRunAction();
	SetUserAction(runAction);
}
