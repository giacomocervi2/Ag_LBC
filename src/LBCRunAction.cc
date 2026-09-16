#include "LBCRunAction.hh"

LBCRunAction::LBCRunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->CreateH1("Edep","Energy deposited", 3000, 0., 3 * MeV);

    analysisManager->CreateNtuple("Photons","Photons"); //we create an Ntuple to store photons informations
    analysisManager->CreateNtupleIColumn("iEvent"); //I stands for integer (we want to store the number of events)
    analysisManager->CreateNtupleDColumn("fX"); //D (f in the name would be float) stands for double (we want to store the x position of the event)
    analysisManager->CreateNtupleDColumn("fY");
    analysisManager->CreateNtupleDColumn("fZ");
    analysisManager->CreateNtupleDColumn("fGlobalTime"); //record the global time since start acquisition
    analysisManager->CreateNtupleDColumn("fWlen"); //record wave-lenght of the photons
    analysisManager->FinishNtuple(0); //the defintion of the Ntuple columns is finisged

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