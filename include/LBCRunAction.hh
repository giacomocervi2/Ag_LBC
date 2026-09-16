#ifndef LBCRUNACTION_HH
#define LBCRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"


class LBCRunAction : public G4UserRunAction
{
public:
    LBCRunAction();
    ~LBCRunAction();

    virtual void BeginOfRunAction(const G4Run *);
    virtual void EndOfRunAction(const G4Run *);
};


#endif