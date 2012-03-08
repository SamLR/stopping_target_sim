//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Mar  6 18:59:03 2012 by ROOT version 5.32/00
// from TTree monA/monA
// found on file: ../output/truth_out.root
//////////////////////////////////////////////////////////

#ifndef truth_analysis_h
#define truth_analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class truth_analysis {
    public :
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
    Int_t           eventNo;
    Int_t           pid;
    Float_t         posX;
    Float_t         posY;
    Float_t         posZ;
    Float_t         time;

   // List of branches
    TBranch        *b_eventNo;   //!
    TBranch        *b_PID;   //!
    TBranch        *b_posX;   //!
    TBranch        *b_posY;   //!
    TBranch        *b_posZ;   //!
    TBranch        *b_time;   //!

    truth_analysis(TTree *tree=0);
    virtual ~truth_analysis();
    virtual Int_t    Cut(Long64_t entry);
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TTree *tree);
    virtual void     Loop();
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef truth_analysis_cxx
truth_analysis::truth_analysis(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
    if (tree == 0) {
        TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../output/truth_out.root");
        if (!f || !f->IsOpen()) {
            f = new TFile("../output/truth_out.root");
        }
        f->GetObject("monA",tree);

    }
    Init(tree);
}

truth_analysis::~truth_analysis()
{
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t truth_analysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}
Long64_t truth_analysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent) {
        fCurrent = fChain->GetTreeNumber();
        Notify();
    }
    return centry;
}

void truth_analysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("eventNo", &eventNo, &b_eventNo);
    fChain->SetBranchAddress("pid", &pid, &b_PID);
    fChain->SetBranchAddress("posX", &posX, &b_posX);
    fChain->SetBranchAddress("posY", &posY, &b_posY);
    fChain->SetBranchAddress("posZ", &posZ, &b_posZ);
    fChain->SetBranchAddress("time", &time, &b_time);
    Notify();
}

Bool_t truth_analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

    return kTRUE;
}

void truth_analysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}
Int_t truth_analysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
    return 1;
}
#endif // #ifdef truth_analysis_cxx
