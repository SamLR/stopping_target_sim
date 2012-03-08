#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

struct entry 
{
    int index; // the index of this entry within its tree
    int eventNo; // NB this is the _geant4_ event number
    int pid;
    float posX;
    float posY;
    float posZ;
    float time;
};

void set_monitor_branches(entry &entry, TTree* tree) 
{
    tree->SetBranchAddress("eventNo", &entry.eventNo);
    tree->SetBranchAddress("pid", &entry.pid);
    tree->SetBranchAddress("posX", &entry.posX);
    tree->SetBranchAddress("posY", &entry.posY);
    tree->SetBranchAddress("posZ", &entry.posZ);
    tree->SetBranchAddress("time", &entry.time);
}

void n_stopped_muons()
{
    TFile* in_file = new TFile("../output/truth_out.root", "READ");
    TTree* in_tree_monA = (TTree*) in_file->Get("monA");
    TTree* in_tree_monB = (TTree*) in_file->Get("monB");

    entry entryA;
    entry entryB;
    entryA.index=0;
    entryB.index=0;

    set_monitor_branches(entryA, in_tree_monA);
    set_monitor_branches(entryB, in_tree_monB);

// int monA_i = 0; // maintain separate indexes
// int monB_i = 0;

    int max_A = in_tree_monA->GetEntries();
    int max_B = in_tree_monB->GetEntries();

    int events_in_both = 0; // number of events that occur in both trees (unique)

    bool more_entries = (max_A != 0) && (max_B != 0);
    if (!more_entries)
    {
        cout << "No entries in one of the trees, stopping"<< endl;
        exit(1);
    }

    while (more_entries)
    {
        in_tree_monA->GetEntry(entryA.index);
        in_tree_monB->GetEntry(entryB.index);

        more_entries = (entryA.index != max_A) && (entryB.index != max_B);
    }

}

void find_event_in_both_trees(entry &lower, entry &higher, TTree* lowerT, TTree* higherT)
{
    if (lower.eventNo < higher.eventNo)
    {
        do {
            lowerT->GetEntry(++lower.index);
            } while (lower.eventNo < higher.eventNo);
        
        if (lower.eventNo == higher.eventNo)
        {
            return; // the returned events should be correct
        } else { // higher < lower 
            find_event_in_both_trees(higher, lower, higherT, lowerT);
        }
        
    }
}