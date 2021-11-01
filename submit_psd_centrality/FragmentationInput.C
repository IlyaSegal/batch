void FragmentationInput(char* input_dirname, char* output_dirname, Int_t files, Int_t A, Int_t P_beam)
{
	TChain* fChain=new TChain("events");
    TChain* fChain_glauber=new TChain("events");
	for (int i = 1; i <= files; i++) 
    {
        fChain->Add(Form("%s/root/dcmqgsm_%d.root", input_dirname, i));
        fChain_glauber->Add(Form("%s/glauber_mcini/dcmqgsm_glauber_%d.mcini.root", input_dirname, i));
    }
    cout<<"Numer of entries equal "<<fChain->GetEntries()<<endl;
    UEvent* fEvent = new UEvent;
    EventInitialState* fIniState = new EventInitialState;
    UEvent* fEvent_glauber = new UEvent;
    EventInitialState* fIniState_glauber = new EventInitialState;
    fChain->AddFriend(fChain_glauber, "glauber");
	fChain->SetBranchAddress("event", &fEvent); 
    fChain->SetBranchAddress("iniState", &fIniState); 
    fChain->SetBranchAddress("glauber.event", &fEvent_glauber); 
    fChain->SetBranchAddress("glauber.iniState", &fIniState_glauber); 
    
    TH1F* hImpactParameter =          new TH1F("B",";B, fm;counts",200,0,20);
    TH1F* hNpart =                    new TH1F("Npart",";Npart;counts",400,0,400);
    TH1F* hNspec =                    new TH1F("Nspec",";Nspec;counts",400,0,400);
    TH1F* hNfragTot =                 new TH1F("Atot_Fragments",";Total mass number of fragments;counts",400,0,400);
    TH1F* hNcoll =                    new TH1F("Ncoll",";Ncoll;counts",1000,0,1000);
    TH2F* hNpart_vs_ImpactParameter = new TH2F("Npart_VS_B",";Npart;B, fm",400,0,400,200,0,20);
    TH2F* hNspec_vs_ImpactParameter = new TH2F("Nspec_VS_B",";Nspec;B, fm",400,0,400,200,0,20);
    TH2F* hNcoll_vs_ImpactParameter = new TH2F("Ncoll_VS_B",";Ncoll;B, fm",1000,0,1000,200,0,20);
    TH2F* hNspec_vs_Npart =           new TH2F("Nspec_VS_Npart",";Nspec;Npart",400,0,400,400,0,400);
    TH2F* hNspec_vs_Ncoll =           new TH2F("Nspec_VS_Ncoll",";Nspec;Ncoll",400,0,400,1000,0,1000);
    TH2F* hNpart_vs_Ncoll =           new TH2F("Npart_VS_Ncoll",";Npart;Ncoll",400,0,400,1000,0,1000);
    
    TH1F* hNfrag =                              new TH1F("A_Fragments",";Mass number of fragments;counts",400,0,400);
    TH1F* hEnergy =                             new TH1F("Energy",";E, GeV;counts",250,0,2500);
    TH1F* hP =                                  new TH1F("P_nucleon",";P, GeV;counts",200,0,20);
    TH1F* hRapidity =                           new TH1F("Y_nucleon",";Rapidity;counts",500,0,5);
    TH2F* hNprotons_vs_Nneutrons =              new TH2F("Nprotons_VS_Nneutrons",";Nprotons;Nneutrons",400,0,400,400,0,400);
    TH2F* hNfrag_vs_ImpactParameter =           new TH2F("A_VS_B",";Mass number of fragments;B, fm",400,0,400,200,0,20);
    TH2F* hNfragTot_vs_ImpactParameter =        new TH2F("Atot_VS_B",";Total mass number of fragments;B, fm",400,0,400,200,0,20);
    TH2F* hNfragTot_vs_Nspec =                  new TH2F("Atot_VS_Nspec",";Total mass number of fragments;Nspec",400,0,400,400,0,400);
    TH2F* hNfrag_vs_Energy =                    new TH2F("A_VS_E",";Mass number of fragments;E, GeV",400,0,400,250,0,2500);
    TH2F* hNspec_vs_Energy =                    new TH2F("Nspec_VS_E",";Nspec;E, GeV",400,0,400,250,0,2500);
    TH2F* hNfrag_vs_Energy_scaled =             new TH2F("A_VS_E_scaled",";Mass number of fragments;E-E_{beam}(A-1), GeV",400,0,400,400,-100,100);
    TH2F* hNfrag_vs_Rapidity =                  new TH2F("A_VS_Y",";Mass number of fragments;Rapidity",400,0,400,500,0,5);
    TH2F* hImpactParameter_vs_Energy =          new TH2F("B_VS_Energy",";E, GeV;B, fm",250,0,2500,200,0,20);
    TH2F* hImpactParameter_vs_Energy_Fragment = new TH2F("B_VS_E",";E, GeV;B, fm",250,0,2500,200,0,20);
    TH2F* hImpactParameter_vs_Rapidity =        new TH2F("B_VS_Y",";B, fm;Rapidity",200,0,20,500,0,5);
    TH2F* hRapidity_vs_Energy =                 new TH2F("Y_VS_E",";E, GeV;Rapidity",250,0,2500,500,0,5);
    TH2F* hPx_vs_Py =                           new TH2F("Py_VS_Px",";Px, GeV;Py, GeV",200,-5,5,200,-5,5);
    
    TH1F* hX =          new TH1F("X",";X, fm;counts",200,-10,10);
    TH1F* hY =          new TH1F("Y",";X, fm;counts",200,-10,10);
    TH1F* hZ =          new TH1F("Z",";X, fm;counts",200,-10,10);
    TH1F* hR =          new TH1F("R",";R, fm;#frac{1}{4#pir}#frac{dN}{dr}",100,0,10);
    TH1F* hPhi =        new TH1F("Phi",";#phi, rad;counts",200,-4,4);
    TH1F* hTheta =      new TH1F("Theta",";#Theta, rad;counts",200,-4,4);
    TH2F* hX_VS_Y=      new TH2F("Y_VS_X",";X, fm;Y, fm",200,-10,10,200,-10,10);
    
    TH3F* hImpactParameter_Nfrag_vs_Energy_scaled = new TH3F("B_VS_A_VS_E_scaled",";B, fm;Mass number of fragments;E-E_{beam}(A-1), GeV",200,0,20,400,0,400,400,-100,100);
        
	Long64_t lNEvents = fChain->GetEntries();
    Long64_t fNpa;
    UParticle* fParticle;
    std::vector<Nucleon> fNucleons;
    
    Double_t fEnergy=0;
    Double_t fFragTot=0;
    Double_t px_beam, py_beam, pz_beam, E_beam;
    if (P_beam == 3) px_beam=0., py_beam=0., pz_beam=3.300000, E_beam=3.430721;
    else if (P_beam == 12) px_beam=0., py_beam=0., pz_beam=12.0, E_beam=12.036604;
    else px_beam=0., py_beam=0., pz_beam=0.0, E_beam=0.0;
    TLorentzVector Beam(px_beam, py_beam, pz_beam, E_beam);
    TLorentzVector Target(0., 0., 0, 0.938);
    TLorentzRotation rTransform;
    rTransform.Boost((Beam + Target).BoostVector());
            
	for (long i = 0; i < lNEvents; i++)
	{
		cout<<i<<endl;
        fChain->GetEntry(i);
        fEnergy=0;
        fFragTot=0;
        fNpa = fEvent->GetNpa();
        if (fIniState_glauber->getNPart() == 0) continue;

        for (int j=0;j<fNpa;j++)
        {
            fParticle = fEvent->GetParticle(j);
            TLorentzVector fMomentum = (fParticle->GetMomentum()).Transform(rTransform);
            
            if (fParticle->GetPdg()>1e9 && fParticle->Pz() >= 1.5) 
            {
                hNprotons_vs_Nneutrons                  ->Fill(fParticle->GetPdg()/10000%1000, fParticle->GetPdg()/10%1000-fParticle->GetPdg()/10000%1000);
                hNfrag                                  ->Fill(fParticle->GetPdg()/10%1000);
                hNfrag_vs_ImpactParameter               ->Fill(fParticle->GetPdg()/10%1000, fEvent->GetB());
                hNfrag_vs_Energy                        ->Fill(fParticle->GetPdg()/10%1000, fMomentum.E());
                hImpactParameter_Nfrag_vs_Energy_scaled ->Fill(fEvent->GetB(), fParticle->GetPdg()/10%1000, fMomentum.E()-E_beam*(fParticle->GetPdg()/10%1000-1));
                hNfrag_vs_Energy_scaled                 ->Fill(fParticle->GetPdg()/10%1000, fMomentum.E()-E_beam*(fParticle->GetPdg()/10%1000-1));
                hNfrag_vs_Rapidity                      ->Fill(fParticle->GetPdg()/10%1000, fMomentum.Rapidity());
                hImpactParameter_vs_Energy_Fragment     ->Fill(fEvent->GetB(), fMomentum.E());
                hImpactParameter_vs_Rapidity            ->Fill(fEvent->GetB(), fMomentum.Rapidity());
                hPx_vs_Py                               ->Fill(fMomentum.Px(), fMomentum.Py());
                hRapidity_vs_Energy                     ->Fill(fMomentum.E(), fMomentum.Rapidity());
                hP                                      ->Fill(fMomentum.P()-P_beam*(fParticle->GetPdg()/10%1000-1));
                hRapidity                               ->Fill(fMomentum.Rapidity());
                fEnergy=fEnergy+fMomentum.E();
                fFragTot=fFragTot+fParticle->GetPdg()/10%1000;
            }       
            else if (fParticle->GetPdg()==2212 && fParticle->Pz() >= 1.5) 
            {
                hNprotons_vs_Nneutrons                  ->Fill(1, 0.1);
                hNfrag                                  ->Fill(1);
                hNfrag_vs_ImpactParameter               ->Fill(1, fEvent->GetB());
                hNfrag_vs_Energy                        ->Fill(1,  fMomentum.E());
                hNfrag_vs_Rapidity                      ->Fill(1, fMomentum.Rapidity());
                hImpactParameter_Nfrag_vs_Energy_scaled ->Fill(fEvent->GetB(), 1, fMomentum.E());
                hNfrag_vs_Energy_scaled                 ->Fill(1, fMomentum.E());
                hImpactParameter_vs_Rapidity            ->Fill(fEvent->GetB(), fMomentum.Rapidity());
                hImpactParameter_vs_Energy_Fragment     ->Fill(fEvent->GetB(), fMomentum.E());
                hPx_vs_Py                               ->Fill(fMomentum.Px(), fMomentum.Py());
                hRapidity_vs_Energy                     ->Fill(fMomentum.E(), fMomentum.Rapidity());                
                hP                                      ->Fill(fMomentum.P());
                hRapidity                               ->Fill(fMomentum.Rapidity());
                fEnergy=fEnergy+fMomentum.E();
                fFragTot=fFragTot+1;
            }
            else if (fParticle->GetPdg()==2112 && fParticle->Pz() >= 1.5) 
            {
                hNprotons_vs_Nneutrons                  ->Fill(0.1, 1);
                hNfrag                                  ->Fill(1);
                hNfrag_vs_ImpactParameter               ->Fill(1, fEvent->GetB());
                hNfrag_vs_Energy                        ->Fill(1, fMomentum.E());
                hImpactParameter_Nfrag_vs_Energy_scaled ->Fill(fEvent->GetB(), 1, fMomentum.E());
                hNfrag_vs_Energy_scaled                 ->Fill(1, fMomentum.E());
                hNfrag_vs_Rapidity                      ->Fill(1, fMomentum.Rapidity());
                hImpactParameter_vs_Energy_Fragment     ->Fill(fEvent->GetB(), fMomentum.E());
                hImpactParameter_vs_Rapidity            ->Fill(fEvent->GetB(), fMomentum.Rapidity());
                hPx_vs_Py                               ->Fill(fMomentum.Px(), fMomentum.Py());
                hRapidity_vs_Energy                     ->Fill(fMomentum.E(), fMomentum.Rapidity());
                hP                                      ->Fill(fMomentum.P());
                hRapidity                               ->Fill(fMomentum.Rapidity());
                fEnergy=fEnergy+fMomentum.E();
                fFragTot=fFragTot+1;
            }
        }
        
        fNucleons = fIniState->getNucleons();
        for (int j=0;j<2*A;j++)
        {
            hX         ->Fill(fNucleons.at(j).getPosition().Px());
            hY         ->Fill(fNucleons.at(j).getPosition().Py());
            hZ         ->Fill(fNucleons.at(j).getPosition().Pz());
            hR         ->Fill(fNucleons.at(j).getPosition().P(), 1/(4*TMath::Pi()*fNucleons.at(j).getPosition().P()*fNucleons.at(j).getPosition().P()));
            hPhi       ->Fill(fNucleons.at(j).getPosition().Phi());
            hTheta     ->Fill(fNucleons.at(j).getPosition().Theta());
            hX_VS_Y    ->Fill(fNucleons.at(j).getPosition().Px(), fNucleons.at(j).getPosition().Py());
            
        }
        
        
        hEnergy                       ->Fill(fEnergy);
        hImpactParameter_vs_Energy    ->Fill(fEnergy, fEvent->GetB());
        hImpactParameter              ->Fill(fEvent->GetB());
        hNpart                        ->Fill(fIniState_glauber->getNPart());
        hNspec                        ->Fill(2*A-fIniState_glauber->getNPart());
        hNfragTot                     ->Fill(fFragTot);
        hNcoll                        ->Fill(fIniState_glauber->getNColl());
        hNpart_vs_ImpactParameter     ->Fill(fIniState_glauber->getNPart(), fEvent->GetB());
        hNspec_vs_ImpactParameter     ->Fill(2*A-fIniState_glauber->getNPart(), fEvent->GetB());
        hNfragTot_vs_ImpactParameter  ->Fill(fFragTot, fEvent->GetB());
        hNfragTot_vs_Nspec            ->Fill(fFragTot, 2*A-fIniState_glauber->getNPart());
        hNspec_vs_Energy              ->Fill(2*A-fIniState_glauber->getNPart(), fEnergy);
        hNcoll_vs_ImpactParameter     ->Fill(fIniState_glauber->getNColl(), fEvent->GetB());
        hNspec_vs_Npart               ->Fill(2*A-fIniState_glauber->getNPart(), fIniState_glauber->getNPart());
        hNspec_vs_Ncoll               ->Fill(2*A-fIniState_glauber->getNPart(), fIniState_glauber->getNColl());
        hNpart_vs_Ncoll               ->Fill(fIniState_glauber->getNPart(), fIniState_glauber->getNColl());
		
	}
    
    TFile *fout = new TFile(output_dirname, "recreate");
    
    hImpactParameter           ->Write();
    hNpart                     ->Write();
    hNspec                     ->Write();
    hNfragTot                  ->Write();
    hNcoll                     ->Write();
    hNpart_vs_ImpactParameter  ->Write();
    hNspec_vs_ImpactParameter  ->Write();
    hNcoll_vs_ImpactParameter  ->Write();
    hNspec_vs_Npart            ->Write();
    hNspec_vs_Ncoll            ->Write();
    hNpart_vs_Ncoll            ->Write();
    
    hNfrag                               ->Write();
    hEnergy                              ->Write();
    hP                                   ->Write();
    hRapidity                            ->Write();
    hNprotons_vs_Nneutrons               ->Write();
    hNfrag_vs_ImpactParameter            ->Write();
    hNfragTot_vs_ImpactParameter         ->Write();
    hNfragTot_vs_Nspec                   ->Write();
    hNfrag_vs_Energy                     ->Write();
    hNspec_vs_Energy                     ->Write();
    hNfrag_vs_Energy_scaled              ->Write();
    hNfrag_vs_Rapidity                   ->Write();
    hImpactParameter_vs_Energy           ->Write();
    hImpactParameter_vs_Energy_Fragment  ->Write();
    hImpactParameter_vs_Rapidity         ->Write();
    hRapidity_vs_Energy                  ->Write();
    hPx_vs_Py                            ->Write();
    
    hX         ->Write();
    hY         ->Write();
    hZ         ->Write();
    hR         ->Write();
    hPhi       ->Write();
    hTheta     ->Write();
    hX_VS_Y    ->Write();
    
    hImpactParameter_Nfrag_vs_Energy_scaled ->Write();
    
	fout->Close();
}
