#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

using namespace std;


void truth_stopped_muons()
{
    TFile* in_file = new TFile("../output/truth_out.root", "READ");
    TTree* treeA = (TTree*) in_file->Get("monA");
    TTree* treeB = (TTree*) in_file->Get("monB");
    
    
}