

void process2()
{
    float posX, posY, posZ, time;
    TFile* inFile = new TFile("out_60_initial_good.root", "READ");
    TTree* data = (TTree*) inFile->Get("MPPC_Data");
    data->SetBranchAddress("posX", &posX);
    data->SetBranchAddress("posY", &posY);
    data->SetBranchAddress("posZ", &posZ);
    data->SetBranchAddress("time", &time);
    
    TH2F *x_z = new TH2F("xz","xz",500, -250, 250, 20, -10, 10);
    
    TH1F *time_hist [] = {NULL,NULL,NULL,NULL};
    for(int i = 0; i < 4; ++i) 
    {
        char name [10];
        sprintf(name, "time%i", i);
        time_hist[i] = new TH1F(name, name, 100, 0, 100);
    } // close for(i < 4) loop
    
    for(int i = 0; i < (int) data->GetEntries(); ++i) 
    {
        data->GetEntry(i);
        x_z->Fill(posX, posZ);
        int id = eventToMPPCiD(posX, posZ);
        if (id <= 0) continue;
        time_hist[id - 1]->Fill(time);
    } // close for(i < (int) data->GetEntries()) loop
    
    x_z->Draw();
    TCanvas *can_time = new TCanvas("time","time");
    can_time->Divide(2,2);
    
    for(int i = 0; i < 4; ++i) 
    {
        can_time->cd(i+1);
        time_hist[i]->Draw();
    } // close for(i < 4) loop
}

int eventToMPPCiD(float x, float z)
{
    // this returns an MPPC number given an input position
    // MPPCs are co-planar in y
    // each value is ±0.5 mm
    // 
    // ______|_-199mm_|_+199mm_    MPPC ID
    // -10mm_|-209(B2)|+189(B1) => 4 | 3
    // +10mm_|-189(A2)|+209(A1)    2 | 1
    //
    
    float id = x+z;
    
    if        (-210.6 < id && id < -208.4) 
    {
        return 4;
    } else if ( 188.4 < id && id <  190.6) 
    {
        return 3;
    } else if (-190.6 < id && id < -188.4) 
    {
        return 2;
    } else if ( 208.4 < id && id <  210.6) 
    {
        return 1;
    } else 
    {
        return 0;
    }
}

void idToString(int id, char *name)
{
    if (id == 1)
    {
        strcpy(name, "A1");
    } else if (id == 2)
    {
        strcpy(name, "A2");
    } else if (id == 3)
    {
        strcpy(name, "B1");
    } else if (id == 4)
    {
        strcpy(name, "B2");
    } else 
    {
        strcpy(name, "Er");
    }
}

void digitisation()
{
    // this will convert an event into a realistic looking MPPC responce 
    
}