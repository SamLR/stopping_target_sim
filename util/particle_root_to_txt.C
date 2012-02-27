// converts a root file of particles into a 6D txt file

#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>

struct particle
{
    float x, y, z, Px, Py, Pz, t; // branches on the tree
    float PID, event, track, parent, weight;
};

using namespace std;

int main()
{
    float x, y, z, Px, Py, Pz, t; // branches on the tree
    float PID, event, track, parent, weight;
    TFile* in_file = new TFile("../input/particle_dist_100M_unfiltered.root", "READ");
    
    ofstream out_file("../input/particle_dist_100M_charged.txt");
    
    TTree* in_tree = (TTree*) in_file->Get("monitor6");
    in_tree->SetBranchAddress("x", &x);
    in_tree->SetBranchAddress("y", &y);
    in_tree->SetBranchAddress("z", &z);
    in_tree->SetBranchAddress("Px", &Px);
    in_tree->SetBranchAddress("Py", &Py);
    in_tree->SetBranchAddress("Pz", &Pz);
    in_tree->SetBranchAddress("t", &t);
    in_tree->SetBranchAddress("PDGid", &PID);
    in_tree->SetBranchAddress("EventID", &event);
    in_tree->SetBranchAddress("TrackID", &track);
    in_tree->SetBranchAddress("ParentID", &parent);
    in_tree->SetBranchAddress("Weight", &weight);
        
    int count(0);
    for (int i(0); i< (int) in_tree->GetEntries(); ++i)
    {
        in_tree->GetEntry(i);
        bool charged_pid = abs((int)PID) == 11 || abs((int)PID) == 13 || abs((int)PID) == 211 || (int)PID == 2212;
        if (charged_pid) 
        {
            ++count;
            out_file <<" "<< event <<" "<< PID <<" "<< x <<" "<< y <<" "<< z <<" "<< Px <<" "<< Py <<" "<< Pz <<" "<< t << endl;
        }
        
    }
    cout << count << endl;
    
    return 0;
}