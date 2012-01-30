// root macro to output various information of input particles to simulation

struct particle_info
{
    int pid;

    float x_pos;
    float y_pos;
    float z_pos;

    float x_mom;
    float y_mom;
    float z_mom;
    float total_mom;
};

enum particles_t {PI_MINUS, ANTI_NU_MU, MU_PLUS, E_PLUS, // -211, -14, -13, -11
    E_MINUS, NU_E, MU_MINUS, NU_MU, GAMMA, PI_PLUS, // 11, 12, 13, 14, 22, 211
    NEUTRON, PROTON, // 2112, 2212
    ODD1, ODD2, ODD3, ODD4}; // 1000010048, 1000020032, 1000030080, 1000040064
    
char particle_names[][11] = {"pi-", "anti Nu Mu", "mu+", "e+",
    "e-", "nu e", "mu-", "nu mu", "gamma", "pi+", "n", "p",
    "odd1", "odd2", "odd3", "odd4"};
    
int particle_counts[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

void fill_hist(char*, TH2F*&, TH1F*&, float, float, float);
void setAxis(TH2F*&);
particles_t getEnum(int);

void particle_distribution()
{
    FILE* pfile = fopen("../input/music_particle_dist.txt", "r");
    TFile* outFile = new TFile("input_particle.root", "RECREATE");
    TTree* outTree = new TTree("input_particles","input_particles");

    particle_info particle;
    outTree->Branch("particle", &particle, 
        "pid/I:x_pos/F:y_pos:z_pos:x_mom:y_mom:z_mom:total_mom");

    // 2D plot of momentum distribution accross the beam pipe
    TH2F* mom_dist = new TH2F("mom_dist", "mom_dist", 200, -2000, 2000, 
        200, -2000, 2000);
    
    // 1D plot momentum plot
    TH1F* mom = new TH1F("mom", "mom", 110, -10, 250);
    // 2D plots of where the different particles are
    TH2F* pid_dist [] = {NULL,NULL,NULL,NULL,
        NULL,NULL,NULL,NULL,
        NULL,NULL,NULL,NULL,
        NULL,NULL,NULL,NULL};
    // 1D plots of momentum on a per particle basis
    TH1F* mom_dist1d [] = {NULL,NULL,NULL,NULL,
        NULL,NULL,NULL,NULL,
        NULL,NULL,NULL,NULL,
        NULL,NULL,NULL,NULL};

    char data_format [] = "%f %f %f %f %f %f %f %f"; // format of file
    char line [130];
    while (fgets(line, 150, pfile) != NULL) // loop over all lines of file
    {
        float eventNo, pid; // pid and event number are also floats
        sscanf(line, data_format, &eventNo, &pid, 
            &particle.x_pos, &particle.y_pos, &particle.z_pos,
            &particle.x_mom, &particle.y_mom, &particle.z_mom);
        
        particle.pid = (int) pid;
        float xsq = particle.x_mom*particle.x_mom;
        float ysq = particle.y_mom*particle.y_mom;
        float zsq = particle.z_mom*particle.z_mom;

        particle.total_mom = TMath::Sqrt(xsq + ysq + zsq); // total momentum
        
        mom->Fill(particle.total_mom);
        mom_dist->Fill(particle.x_pos, particle.y_pos, particle.total_mom);
        
         // convert from PDG encoding to simple enumeration (ie -211 -> 0 = pi+)
        particles_t index = getEnum(particle.pid);
        char* pid_name = particle_names[index]; // get the name
        ++particle_counts[index];
        // initialse and fill the histograms
        fill_hist(pid_name, pid_dist[index], mom_dist1d[index], 
            particle.x_pos, particle.y_pos,particle.total_mom);
    }

    TCanvas *can_mom = new TCanvas("mom","mom");
    mom_dist->SetTitle("Momentum distribution at end of beam pipe");
    mom_dist->Draw("COLZ");
    TCanvas *can_pid = new TCanvas("pid","pid");
    can_pid->Divide(4,4);
    for(int i = 0; i < 16; ++i) 
    {
        can_pid->cd(i+1);
        pid_dist[i]->Draw("cont1");
    } // close for(i < 16) loop
    TCanvas *can_mom2 = new TCanvas("mom2","mom2");
    mom->Draw();
    TCanvas *can_mom_main = new TCanvas("mom_main","mom_main");    
    mom->GetXaxis()->SetTitle("Momentum (MeV)");
    mom->SetTitle("Momentum distribution at beam end");
    mom->Draw();
    mom_dist1d[4]->SetLineColor(2); // e-, red
    mom_dist1d[4]->Draw("SAME");
    mom_dist1d[8]->SetLineColor(3); // gamma, green
    mom_dist1d[8]->Draw("SAME");
    mom_dist1d[10]->SetLineColor(4); // n, dark blue
    mom_dist1d[10]->Draw("SAME");
    TLegend* leg_mom_main = new TLegend(0.1, 0.7, 0.48, 0.9);
    leg_mom_main->SetHeader("Most common particles & their momentum");
    leg_mom_main->AddEntry(mom, "Summed momenta for all particles", "l");
    leg_mom_main->AddEntry(mom_dist1d[4], "e- momentum", "l");
    leg_mom_main->AddEntry(mom_dist1d[8], "gamma momentum", "l");
    leg_mom_main->AddEntry(mom_dist1d[10], "neutron momentum", "l");
    leg_mom_main->Draw();
    
    TCanvas *can_charged = new TCanvas("charged","charged");
    mom_dist1d[4]->GetXaxis()->SetTitle("Momentum (MeV)"); // e-, black
    mom_dist1d[4]->SetTitle("Charged particle momentum distribution");
    mom_dist1d[4]->SetLineColor(1);
    mom_dist1d[4]->Draw();
    mom_dist1d[2]->SetLineColor(2); // mu+ red
    mom_dist1d[2]->Draw("SAME");
    mom_dist1d[3]->SetLineColor(3); // e+ green
    mom_dist1d[3]->Draw("SAME");
    mom_dist1d[0]->SetLineColor(4); // pi- blue
    mom_dist1d[0]->Draw("SAME");
    mom_dist1d[6]->SetLineColor(6); // mu- pink
    mom_dist1d[6]->Draw("SAME");
    mom_dist1d[9]->SetLineColor(7); // pi+ light blue
    mom_dist1d[9]->Draw("SAME");
    mom_dist1d[11]->SetLineColor(8); // proton off green
    mom_dist1d[11]->Draw("SAME");
    TLegend* leg_charged_mom = new TLegend(0.1, 0.7, 0.48, 0.9);
    leg_charged_mom->SetHeader("Charged particle momentum distributions");
    leg_charged_mom->AddEntry(mom_dist1d[0], "pi-", "l");
    leg_charged_mom->AddEntry(mom_dist1d[2], "mu+", "l");
    leg_charged_mom->AddEntry(mom_dist1d[3], "e+", "l");
    leg_charged_mom->AddEntry(mom_dist1d[4], "e-", "l");
    leg_charged_mom->AddEntry(mom_dist1d[6], "mu-", "l");
    leg_charged_mom->AddEntry(mom_dist1d[9], "pi+", "l");
    leg_charged_mom->AddEntry(mom_dist1d[11], "p", "l");
    leg_charged_mom->Draw();
    
    
    TCanvas *can_pos2 = new TCanvas("pos2","pos2");
    can_pos2->Divide(2,2);
    can_pos2->cd(1);
    setAxis(pid_dist[10]);
    pid_dist[10]->Draw("COLZ"); // n (most common)
    can_pos2->cd(2);
    setAxis(pid_dist[8]);
    pid_dist[8]->Draw("COLZ"); // gamma
    can_pos2->cd(3);
    setAxis(pid_dist[4]);
    pid_dist[4]->Draw("COLZ"); // e-
    can_pos2->cd(4);
    setAxis(pid_dist[3]); 
    pid_dist[3]->Draw("COLZ"); // e+ (least common)
    
    TCanvas *can_charged_pos = new TCanvas("charged_pos","charged_pos");
    can_charged_pos->Divide(3,3);
    can_charged_pos->cd(1);
    setAxis(pid_dist[4]); 
    pid_dist[4]->Draw("COLZ");
    can_charged_pos->cd(2);
    setAxis(pid_dist[3]);
    pid_dist[3]->Draw("COLZ");
    can_charged_pos->cd(3);
    setAxis(pid_dist[2]);
    pid_dist[2]->Draw("COLZ");
    can_charged_pos->cd(4);
    setAxis(pid_dist[9]);
    pid_dist[9]->Draw("COLZ");
    can_charged_pos->cd(5);
    setAxis(pid_dist[11]);
    pid_dist[11]->Draw("COLZ");
    can_charged_pos->cd(6);
    setAxis(pid_dist[0]);
    pid_dist[0]->Draw("COLZ");
    can_charged_pos->cd(7);
    setAxis(pid_dist[6]);
    pid_dist[6]->Draw("COLZ");
    can_charged_pos->cd(8);
    TLegend* leg_charged_mom = new TLegend(0.1, 0.7, 0.48, 0.9);
    
    
    // for(int i = 0; i < 16; ++i) 
    //     {
    //         cout << particle_names[i]<< " "<< particle_counts[i]<<endl;
    //     } // close for(i < 16) loop
}

void setAxis(TH2F*& hist)
{
    hist->GetXaxis()->SetTitle("x position (mm)");
    hist->GetXaxis()->SetLabelSize(0.03);
    hist->GetXaxis()->SetTitleOffset(0.9);
    hist->GetXaxis()->CenterTitle();
    
    hist->GetYaxis()->SetTitle("y position (mm)");
    hist->GetYaxis()->SetLabelSize(0.03);
    hist->GetYaxis()->SetTitleOffset(0.9);
    hist->GetYaxis()->CenterTitle();
}

void fill_hist(char* particle_name, TH2F* &pHist, TH1F* &mHist, float xpos, float ypos, 
    float momentum)
{
    if(pHist == NULL) 
    {    
        char name [20];
        sprintf(name, "position, %s", particle_name);
        pHist = new TH2F(name, name, 200, -2000, 2000, 
            200, -2000, 2000);

        sprintf(name, "momentum, %s", particle_name);
        mHist = new TH1F(name, name, 160, -10, 250);

        pHist->Fill(xpos, ypos);
        mHist->Fill(momentum);
        // return pHist;
    } else {
        pHist->Fill(xpos, ypos);
        mHist->Fill(momentum);
        // return pHist;
    }// close if (pid_dist[1] == NULL)
}

particles_t getEnum(int pid)
{
    // converts a PID to an enumerated particles_t
    switch(pid)
    {
        case -211: return PI_MINUS;
        case -14: return ANTI_NU_MU;
        case -13: return MU_PLUS;
        case -11: return E_PLUS;
        case  11: return E_MINUS;
        case  12: return NU_E;
        case  13: return MU_MINUS;
        case  14: return NU_MU;
        case  22: return GAMMA;
        case  211: return PI_PLUS;
        case  2112: return NEUTRON;
        case  2212: return PROTON;
        case  1000010048: return ODD1;
        case  1000020032: return ODD2;
        case  1000030080: return ODD3;
        case  1000040064: return ODD4;
    }
}

