#include <iostream.h>
#include "TChain.h"
#include "TString.h"
#include <TFile.h>
#include <TTree.h>
#include "TROOT.h"

extern void InitGui();
VoidFuncPtr_t initfuncs[] = {InitGui, 0};
TROOT root("Rint","The ROOT Interactive Interface", initfuncs);

using namespace std;

int main (int argc, char *argv[]) {

  if (argc < 3) {
    cout << "usage: merge ntp1 merged.root *.root" << endl;
    return 0;
  }

  const char *n1, *n2, *f;
  n1 = argv[1];
  f  = argv[2];

  cout << "   ntuple: " << n1 << endl;
  cout << "   filename: " << f << endl;
  cout << "   Files to be merged:  " << endl;

  TChain cha(n1);

  if (argc > 3) {
    for (int i = 3; i < argc; ++i) {
      cha.Add(argv[i]);   cout << "       " << argv[i] << endl;
    }
  }

  cha.Merge(f);
  
  return 0;
}