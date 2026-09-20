#include "LBCRunAction.hh"

LBCRunAction::LBCRunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->CreateH1("Edep","Energy deposited", 3000, 0., 3 * MeV);

    
    analysisManager->CreateNtuple("Events","Events Data"); 
    analysisManager->CreateNtupleIColumn("iEvent");           
    analysisManager->CreateNtupleDColumn("fEnergyDeposited"); 
    analysisManager->FinishNtuple(0); 
}

LBCRunAction::~LBCRunAction()
{
}

void LBCRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID; //we convert our integer runID into a string
    strRunID << runID;

    analysisManager->OpenFile("output" + strRunID.str() + ".root"); //we convert our stringstream object into a string and add it to our output
    //All this is for define the auto-increment: for first file we get file "output0.root", the secondo "output1.root", etc.
}

void LBCRunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->Write(); //at the end of each run, this function stores the infos in teh histo and in the Ntuple above defined

    analysisManager->CloseFile(); //All open files are closed

    G4int runID = run->GetRunID();

    G4cout << "Finishing run " << runID << G4endl;
}