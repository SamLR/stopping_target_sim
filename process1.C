// simple root macro that reads in and displays the central region of the histograms in out.root

void process1()
{
    TFile* file = new TFile("build/out.root", "READ");
    
    float time;
    float x;
    float y;
    float z;
    
    TTree* data = (TTree*) file->Get("MPPC_Data");
    data->SetBranchAddress("posX", &x);
    data->SetBranchAddress("posY", &y);
    data->SetBranchAddress("posZ", &z);
    data->SetBranchAddress("time", &time);
    
    TH1F* hist_x = new TH1F("x", "x", 100, -300.0, 300.0);
    TH1F* hist_y = new TH1F("y", "y", 100, -300.0, 300.0);
    TH1F* hist_z = new TH1F("z", "z", 100, -300.0, 300.0);
    TH1F* hist_time = new TH1F("time", "time", 100, -50.0, 50.0);
    
    for(int i = 0; i < (int)data->GetEntries(); ++i) 
    {
        data->GetEntry(i);
        hist_x->Fill(x);
        hist_y->Fill(y);
        hist_z->Fill(z);
        hist_time->Fill(time);
        
    } // close for(i < tree->GetEntries) loop
    
    TCanvas* c = new TCanvas("c1","c1");
    c->Divide(2,2);
    c->cd(1);
    hist_x->Draw();
    
    c->cd(2);
    hist_y->Draw();
    
    c->cd(3);
    hist_z->Draw();
    
    c->cd(4);
    hist_time->Draw();
}