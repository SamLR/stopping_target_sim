struct bfield2d{
    float x;
    float z;
    float bx;
    float bz;
    float bmod;   
};

void display_magfield(){
    // FILE *pFile = fopen("magfield.table", "r");
    FILE *pFile = fopen(
        "../MUSIC-3D-filed-map/MUSIC-FILL-map1-X=+-2000-Y=0-1000-Z=-1000-5000.table", "r");
    
    TFile *outFile = new TFile("magfield.root", "RECREATE");
    TTree *outTree = new TTree("bfield", "bfield");
    
    bfield2d myfield;
    outTree->Branch("field", &myfield, "x/F:z:bx:bz");
    TH2F *bhist = new TH2F("Bfield", "Bfield", 200, -2000, 2000,
                                               300, -1000, 5000);
    
    float y, by, junk;
    char dat_format [] = "%f %f %f %e %e %e %e";
    char line [150];
    int linecount = 0;
    while (fgets(line, 150, pFile) != NULL)
    {
        ++ linecount;
        if (linecount < 10) continue;
        sscanf(line, dat_format, &myfield.x, &y, 
            &myfield.z, &myfield.bx, &by, &myfield.bz, &myfield.bmod);
        
        if (y < 5) continue; // take only the y = 0 slice
        outTree->Fill();
        bhist->Fill(myfield.x, myfield.z, myfield.bmod);
    }
    // bhist->Draw("SURF2");
    bhist->Draw("CONT1");
    
}
