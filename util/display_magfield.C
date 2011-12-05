struct bfield2d{
    float x;
    float z;
    float bx;
    float bz;
    float bmod;   
};

void display_magfield(){
    // FILE *pFile = fopen("../../PION-DIPOLE-bfield/Dipole fring Bmap_XZ_X-2000-0_Z0-2000.table", "r");
    // FILE *pFile = fopen("../../PION-DIPOLE-bfield/Dipole Bmap_XZ_X0-220_Z0-500.table", "r");
    // FILE *pFile = fopen("bfield_roi.table", "r");
    // FILE *pFile = fopen("magfield.table", "r");
    FILE *pFile = fopen("../output/vec_field_out.table", "r");
    // FILE *pFile = fopen(
    //     "../../MUSIC-3D-filed-map/MUSIC-FILL-map1-X=+-2000-Y=0-1000-Z=-1000-5000.table", "r");
    
    TFile *outFile = new TFile("magfield.root", "RECREATE");
    TTree *outTree = new TTree("bfield", "bfield");
    
    bfield2d myfield;
    outTree->Branch("field", &myfield, "x/F:z:bx:bz");
    // TH2F *bhist = new TH2F("Bfield", "Bfield", 200, -2000, 2000,
    //                                            300, -1000, 5000);
    // TH2F *bhist = new TH2F("Bfield", "Bfield", 200, -2000, 2000,
    //                                                300, -1000, 11000);
    // TH2F *bhist = new TH2F("Bfield", "Bfield", 50, 400, 1400,
    //                                        50, 3100, 4100);
    // TH2F *bhist = new TH2F("Bfield", "Bfield", 25, -10, 230,
    //                                        53, -10, 510);
    // TH2F *bhist = new TH2F("Bfield", "Bfield", 101, -2010, 10,
    //                                        101, -10, 2010);
    TH2F *bhist = new TH2F("Bfield", "Bfield", 54, -540, 540,
                                           54, -540, 540);
    float y, by, junk;
    char dat_format [] = "%f %f %f %e %e %e %e";
    char line [150];
    int linecount = 0;
    float max_bfield = 0;
    int test =0;
    while (fgets(line, 150, pFile) != NULL)
    {
        ++ linecount;
        // if (linecount < 10) continue;
        sscanf(line, dat_format, &myfield.x, &y, 
            &myfield.z, &myfield.bx, &by, &myfield.bz, &myfield.bmod);
        if (myfield.bmod > max_bfield) max_bfield = myfield.bmod;
        if (y > 15) continue; // take only the y = 0 slice
        ++test;
        outTree->Fill();
        bhist->Fill(myfield.x, myfield.z, myfield.bmod);
    }
    outFile->Write();
    // bhist->Draw("SURF2");
    bhist->SetTitle("Magnetic dipole fringe field");
    // bhist->SetTitle("Magnetic field in simulated region");
    // bhist->
    // bhist->Draw("CONT1");
    bhist->Draw("COLZ");
    cout << "Max magnetic field: "<<max_bfield << endl;
    cout << "Number of lines read in: "<<test << endl;
    
}
