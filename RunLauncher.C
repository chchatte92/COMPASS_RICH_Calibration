//#define _COMPUTE_
//#define _DISPLAY_
#define _ANALYSIS_
//#define _PREVIOUS_
//#define _BKGMAPS_
#define _AVERAGE_
void RunLauncher(){
  //TString precmd = "xrdcp root://eospublic.cern.ch//eos/experiment/compass/generalprod/testcoral/transv2022W08t4.0/hist/";
  //TString precmd = "xrdcp root://eospublic.cern.ch//eos/experiment/compass/generalprod/testcoral/transv2022W06t2.1/histos/";
  TString precmd = "xrdcp root://eospublic.cern.ch//eos/experiment/compass/generalprod/testcoral/transv2022W09t2/histos/";
  //int RunNumber[]={297678,297698,297710,297729,297828,297842,297882,297935,297969,297978,297989,298001,298010,298017,298060,298068};
  //int RunNumber[]={/*297085,297084,297081,297098,297096,297202,297197,297173,297171,297160};*/297065,297064,297076,297075,297091,297199,297194,297181,297167,297164}; 
  int RunNumber[] = {298190, 298289, 298225, 298076, 298219, 298297, 298270, 298125, 298099, 298183, 298110, 298232, 298248, 298210, 298282, 298167, 298078, 298095, 298260, 298301};
  const int nFiles = sizeof(RunNumber)/sizeof(RunNumber[0]);
  TString cmd;
  TString fName;
  const int nRegions=4;
  double Mean[nFiles][nRegions]={0.0};
  double Sigma[nFiles][nRegions]={0.0};

  double MeanT[nFiles][nRegions]={0.0};
  double SigmaT[nFiles][nRegions]={0.0};

  double MeanG[nFiles][nRegions] = {0.0} ;
  double SigmaG[nFiles][nRegions]= {0.0};
  double erRun[nFiles]={0.0};
  vector<TH2F*> histCollection;
  int pmtbins[nRegions][4]={{39,74,0,17},{0,35,0,17},{39,74,20,37},{0,35,20,37}};
  for(unsigned ir=0; ir<nFiles; ir++){
    //printf("%d\n",RunNumber[ir]);
#ifdef _COMPUTE_
    fName.Form("histsum-%d-0-8.root",RunNumber[ir]);
    cmd.Form("%s%s .",precmd.Data(),fName.Data());
    //copy data
    printf("Copying data");
    system(cmd.Data());
    //do the job
    cmd.Form("root -l -b -q generate_background_map_nv.C'(%d)'",RunNumber[ir]);
    printf("Doing bkg analysis\n");
    system(cmd.Data());
    //Remove the prod file
    //cmd.Form("rm %s\n",fName.Data());
    //printf("Removing prod file\n");
    system(cmd);
#endif
#ifdef _DISPLAY_
    cmd.Form("root -l -b -q compare_backgrounds.C'(%d)'",RunNumber[ir]);
    system(cmd);
#endif
  }
#ifdef _ANALYSIS_
 
  const char * hNames[nRegions]={"top_saleve","top_jura","bottom_saleve","bottom_jura"};
  //TFile* refFile = new TFile("./rootfiles/BackgroundMap_297678_Display.root");
  TFile* refFile = new TFile("./rootfiles/BackgroundMap_2012_Display.root");
  TH2F* refHistos[nRegions];
  TCanvas *cRef = new TCanvas("cref","cRef",900,900);
  cRef->Divide(2,2);
  for(int ih = 0; ih<nRegions ; ih++){
    TString hName;
    hName.Form("h2_background_map1_%s",hNames[ih]);
    refHistos[ih]=static_cast<TH2F*>(refFile->Get(hName));
    cRef->cd(ih+1);
    refHistos[ih]->Draw("colz");
  }
  TString aFile;
  TCanvas*c[nFiles];
  TString cname;
  TH2F* iHistos[nFiles][nRegions];
  TH2F* iHistos_clone[nFiles][nRegions];  
#ifdef _AVERAGE_
  TH2F* iHistos_average[nRegions];
#endif
  for(unsigned iF=0; iF<nFiles; iF++){
    aFile.Form("./rootfiles/BackgroundMap_%d_Display.root",RunNumber[iF]);
    TFile *myFile = new TFile(aFile);
    myFile->ls();
    cname.Form("Ratio_Bkg_%d",RunNumber[iF]);
    c[iF] = new TCanvas(cname,cname,900,900);
    c[iF]->Divide(2,2);
    for(int i=0; i<nRegions; i++){
      TString hName;
      hName.Form("h2_background_map1_%s",hNames[i]);
      printf("%s\n",hName.Data());
      iHistos[iF][i] =dynamic_cast<TH2F*>(myFile->Get(hName));
      if(!iHistos[iF][i]) continue;
      //if(iHistos[iF][i]) printf("Histo Found\n");
      //c[i] = new TCanvas();
      //iHistos[iF][i]->Draw("colz");
      histCollection.push_back(iHistos[iF][i]);
      iHistos_clone[iF][i] =dynamic_cast<TH2F*>(iHistos[iF][i]->Clone());
      
#ifdef _AVERAGE_
      if(iF ==0) iHistos_average[i] = dynamic_cast<TH2F*>(iHistos[iF][i]);
      if(iF>0)iHistos_average[i]->Add(iHistos_clone[iF][i]);
#endif
      if(iF>0){
#ifndef _AVERAGE_
#ifdef _PREVIOUS_
	iHistos_clone[iF][i]->Divide(iHistos[iF-1][i]);
#else
	iHistos_clone[iF][i]->Divide(refHistos[i]);
#endif
#endif
	iHistos_clone[iF][i]->SetMinimum(0.8);
	iHistos_clone[iF][i]->SetMaximum(1.2);
	iHistos_clone[iF][i]->GetZaxis()->SetLabelSize(0.025);  // increase font size
	//iHistos_clone[iF][i]->GetZaxis()->SetTitleSize(0.04);  // increase title size
	iHistos_clone[iF][i]->GetZaxis()->SetTitleOffset(0.8); // move title away from axis

      
	gStyle->SetOptStat(0);
	//gStyle->SetPalette(kRainBow);  // or another palette
	gStyle->SetNumberContours(99); // smoother gradient
      
	// Optional: expand margins
	gPad->SetRightMargin(0.1);  // more space for palette

	if(i==0){
	  //TLatex latex;
	  //latex.SetNDC(); // Use Normalized Device Coordinates (0–1)
	  //latex.SetTextSize(0.04);
	  //latex.DrawLatex(0.99, 0.99, cname);
	  TPaveText* pt = new TPaveText(0.5, 0.5, 0.5, 0.5, "NDC NB");
#ifndef _AVERAGE_
#ifdef _PREVIOUS_
	  cname.Form("Ratio(%d/%d)",RunNumber[iF],RunNumber[iF-1]);
#else
	  cname.Form("Ratio(%d/2012)",RunNumber[iF]);//RunNumber[0]);
#endif
#endif	  
	  pt->AddText(cname);
	  pt->SetFillColor(0);  // Transparent
	  pt->SetTextFont(42);  // Helvetica
	  pt->SetTextSize(0.02);
	  pt->SetLineWidth(0);
	  pt->Draw();

	}
      }
      c[iF]->cd(i+1); iHistos_clone[iF][i]->Draw("colz");
      c[iF]->SetTitle(cname);

      double m=0.0, s=0.0;
      int pnt=0;
      int wx = iHistos_clone[iF][i]->GetXaxis()->GetBinWidth(1);
      int wy = iHistos_clone[iF][i]->GetYaxis()->GetBinWidth(1);
      //printf("Bins: %d %d\n",pmtbins[i][0]/wx,pmtbins[i][1]/wx);
      for(int ix=(pmtbins[i][0]/wx)+1; ix<=(pmtbins[i][1]/wx); ix++){
	for(int iy=(pmtbins[i][2]/wy)+1; iy<=(pmtbins[i][3]/wy); iy++){
	  double val = iHistos_clone[iF][i]->GetBinContent(ix,iy);
	  //if(iF>14 && i==2) printf("%f\n",val);
	  if(val ==0) continue;
	  m+=val;
	  //printf("%0.2f",m);
	  
	  pnt ++;
	}
      }
      m/=pnt;
      for(int ix=(pmtbins[i][0]/wx)+1; ix<=(pmtbins[i][1]/wx); ix++){
	for(int iy=(pmtbins[i][2]/wy)+1; iy<=(pmtbins[i][3]/wy); iy++){
	  double val = iHistos_clone[iF][i]->GetBinContent(ix,iy);
	  if(val ==0) continue;
	  s+=(val-m)*(val-m);
	}
      }
      s =sqrt(s/pnt);
      printf("-------->%0.2f %0.2f\n",m,s);
      Mean[iF][i] = m ;
      Sigma[iF][i]= s;
      int pmtpt =pnt;
      m=0.0, s=0.0, pnt =0;

      for(int ix=1; ix<=74; ix++){
	for(int iy=1; iy<=37; iy++){
	  double val = iHistos_clone[iF][i]->GetBinContent(ix,iy);
	  //if(iF>14 && i==2) printf("%f\n",val);
	  if(val ==0) continue;
	  m+=val;
	  //printf("%0.2f",m);
	  
	  pnt ++;
	}
      }
      m/=pnt;
      for(int ix=1; ix<=74; ix++){
	for(int iy=1; iy<=37; iy++){
	  double val = iHistos_clone[iF][i]->GetBinContent(ix,iy);
	  if(val ==0) continue;
	  s+=(val-m)*(val-m);
	}
      }
      s =sqrt(s/pnt);
      printf("-------->%0.2f %0.2f\n",m,s);
      MeanT[iF][i] = m ;
      SigmaT[iF][i]= s;

      MeanG[iF][i] = abs(Mean[iF][i]*pmtpt - MeanT[iF][i]*pnt)/abs(pmtpt-pnt) ;
      SigmaG[iF][i]= sqrt(SigmaT[iF][i] - Sigma[iF][i]);
      
      m=0.0, s=0.0, pnt =0;
    }//regions
    //myFile->Close();
  }//files

#ifdef _AVERAGE_
  TCanvas * c_av= new TCanvas("c_av","c_av",900,900);
  c_av->Divide(2,2);
  TFile *outfile = new TFile("bkg_ratio_average.root","RECREATE");
  outfile->cd();
  TCanvas * c_ra[nFiles];
  for(int i=0; i<nRegions; i++){
    iHistos_average[i]->Scale(1/(nFiles*1.0));
    c_av->cd(i+1);
    iHistos_average[i]->Draw("colz");
  }//get average
  for(int iF=0; iF<nFiles; iF++){
    cname.Form("ratio_%d_av",RunNumber[iF]);
    c_ra[iF] = new TCanvas(cname,cname,900,900);
    gStyle->SetOptStat(0);
    //gStyle->SetPalette(kRainBow);  // or another palette
    gStyle->SetNumberContours(99); // smoother gradient
    // Optional: expand margins
    gPad->SetRightMargin(0.1);  // more space for palette
    TPaveText* pt = new TPaveText(0.5, 0.5, 0.5, 0.5, "NDC NB");
    cname.Form("Ratio %d/Av",RunNumber[iF]);
    pt->AddText(cname);
    pt->SetFillColor(0);  // Transparent
    pt->SetTextFont(42);  // Helvetica
    pt->SetTextSize(0.02);
    pt->SetLineWidth(0);
    pt->Draw();
    c_ra[iF]->Divide(2,2);
    if(iF==0) c_ra[0]->Print("bkgratio_2012.pdf[");
    for(int i=0; i<nRegions; i++){
      iHistos_clone[iF][i]->Divide(iHistos_average[i]);
      c_ra[iF]->cd(i+1);
      iHistos_clone[iF][i]->Draw("colz");
      iHistos_clone[iF][i]->SetMinimum(0.8);
      iHistos_clone[iF][i]->SetMaximum(1.2);
      iHistos_clone[iF][i]->GetZaxis()->SetLabelSize(0.025);  // increase font size
      //iHistos_clone[iF][i]->GetZaxis()->SetTitleSize(0.04);  // increase title size
      iHistos_clone[iF][i]->GetZaxis()->SetTitleOffset(0.8); // move title away from axis

      // Mean and sigma
      double m=0.0, s=0.0;
      int pnt=0;
      int wx = iHistos_clone[iF][i]->GetXaxis()->GetBinWidth(1);
      int wy = iHistos_clone[iF][i]->GetYaxis()->GetBinWidth(1);
      //printf("Bins: %d %d\n",pmtbins[i][0]/wx,pmtbins[i][1]/wx);
      for(int ix=(pmtbins[i][0]/wx)+1; ix<=(pmtbins[i][1]/wx); ix++){
	for(int iy=(pmtbins[i][2]/wy)+1; iy<=(pmtbins[i][3]/wy); iy++){
	  double val = iHistos_clone[iF][i]->GetBinContent(ix,iy);
	  //if(iF>14 && i==2) printf("%f\n",val);
	  if(val ==0) continue;
	  m+=val;
	  //printf("%0.2f",m);
	  
	  pnt ++;
	}
      }
      m/=pnt;
      for(int ix=(pmtbins[i][0]/wx)+1; ix<=(pmtbins[i][1]/wx); ix++){
	for(int iy=(pmtbins[i][2]/wy)+1; iy<=(pmtbins[i][3]/wy); iy++){
	  double val = iHistos_clone[iF][i]->GetBinContent(ix,iy);
	  if(val ==0) continue;
	  s+=(val-m)*(val-m);
	}
      }
      s =sqrt(s/pnt);
      printf("-------->%0.2f %0.2f\n",m,s);
      Mean[iF][i] = m ;
      Sigma[iF][i]= s;
      int pmtpt = pnt;
      m=0.0, s=0.0, pnt =0;

      for(int ix=1; ix<=74; ix++){
	for(int iy=1; iy<=37; iy++){
	  double val = iHistos_clone[iF][i]->GetBinContent(ix,iy);
	  //if(iF>14 && i==2) printf("%f\n",val);
	  if(val ==0) continue;
	  m+=val;
	  //printf("%0.2f",m);
	  
	  pnt ++;
	}
      }
      m/=pnt;
      for(int ix=1; ix<=74; ix++){
	for(int iy=1; iy<=37; iy++){
	  double val = iHistos_clone[iF][i]->GetBinContent(ix,iy);
	  if(val ==0) continue;
	  s+=(val-m)*(val-m);
	}
      }
      s =sqrt(s/pnt);
      printf("-------->%0.2f %0.2f\n",m,s);
      MeanT[iF][i] = m ;
      SigmaT[iF][i]= s;

      MeanG[iF][i] = abs(Mean[iF][i]*pmtpt - MeanT[iF][i]*pnt)/abs(pmtpt-pnt) ;
      SigmaG[iF][i]= sqrt(SigmaT[iF][i] - Sigma[iF][i]);
      
      m=0.0, s=0.0, pnt =0;      
    }//nRegions
    c_ra[iF]->Print("bkgratio_2012.pdf");
    c_ra[iF]->Write();
  }//nFiles
  /////////////////////////////////////
  TGraphErrors *g[nRegions][3];
  TCanvas *cgraph = new TCanvas("cgraph","cgraph",1200,900); 
  cgraph->Divide(2,2);
  int counter = 0;
  for(int i=0; i<nRegions; i++){
    counter ++;
    double mean[nFiles]={0};
    double sig[nFiles]={0};
    double runNumber[nFiles]={0.0};

    double meanT[nFiles]={0};
    double sigT[nFiles]={0};

    double meanG[nFiles]={0};
    double sigG[nFiles]={0};
    
    for(int j=0; j<nFiles; j++){
      mean[j] = Mean[j][i];
      sig[j]= Sigma[j][i];
      meanT[j] = MeanT[j][i];
      sigT[j]= SigmaT[j][i];
      meanG[j] = MeanG[j][i];
      sigG[j]= SigmaG[j][i];
      runNumber[j]=RunNumber[j];
    }

    g[i][0]= new TGraphErrors(nFiles-1, runNumber, mean, erRun, sig);
    g[i][1]= new TGraphErrors(nFiles-1, runNumber, meanT, erRun, sigT);
    g[i][2]= new TGraphErrors(nFiles-1, runNumber, meanG, erRun, sigG);
    
    g[i][0]->SetMarkerStyle(20);
    g[i][0]->SetMarkerColor(31);
    g[i][0]->SetLineColor(31);
    
    g[i][1]->SetMarkerStyle(21);
    g[i][1]->SetMarkerColor(41);
    g[i][1]->SetLineColor(41);
    
    g[i][2]->SetMarkerStyle(22);
    g[i][2]->SetMarkerColor(44);
    g[i][2]->SetLineColor(44);
    
    g[i][0]->SetLineWidth(2);
    g[i][1]->SetLineWidth(2);
    g[i][2]->SetLineWidth(2);
    g[i][2]->SetLineStyle(3);
    
    cgraph->cd();
    TLegend* leg = new TLegend(0.65, 0.75, 0.88, 0.88);
    leg->AddEntry(g[i][0],"PMT" ,"lep");
    leg->AddEntry(g[i][1],"Total" ,"lep");
    leg->AddEntry(g[i][2],"Gas" ,"lep");
    
    g[i][0]->SetTitle(hNames[i]);
    g[i][0]->GetXaxis()->SetTitle("Run#");
    g[i][0]->GetXaxis()->SetNoExponent(kTRUE);
    g[i][0]->GetYaxis()->SetTitle("Ratio (#mu #pm #sigma)");
    g[i][0]->GetYaxis()->SetRangeUser(0.50,1.50);
    
    cgraph->cd(i+1);
    g[i][0]->Draw("AP");
    g[i][0]->Write();
    g[i][1]->Draw("P SAMES");
    g[i][2]->Draw("P SAMES");
    leg->Draw();
  }
  cgraph->Print("bkgratio_2012.pdf");
  /////////////////////////////////////
  c_ra[nFiles-1]->Print("bkgratio_2012.pdf]");
  outfile->Write();
  outfile->Close();
#endif

  
#ifndef _AVERAGE_
#ifdef _PREVIOUS_
  TFile *outFile = new TFile("bkg_ratio_Previous.root","RECREATE");
  outFile->cd();
  c[0]->Print("bkgratio_Previous.pdf[");
#else
  TFile *outFile = new TFile("bkg_ratio_2012.root","RECREATE");
  outFile->cd();
  c[0]->Print("bkgratio_2012.pdf[");
#endif
#endif

#ifndef _AVERAGE_  
  for(int i=1; i<nFiles; i++){
    gStyle->SetOptStat(0);
    c[i]->Write();
#ifdef _PREVIOUS_
    c[i]->Print("bkgratio_Previous.pdf");
#else
    c[i]->Print("bkgratio_2012.pdf");
#endif
  }
  //c[nFiles-1]->Print("bkgratio.pdf]");
  //outFile->Write();
  //outFile->Close();
  TGraphErrors *g[nRegions][3];
  TCanvas *cgraph = new TCanvas("cgraph","cgraph",1200,900);
   
  cgraph->Divide(2,2);
  int counter = 0;
  for(int i=0; i<nRegions; i++){
    counter ++;
    double mean[nFiles]={0};
    double sig[nFiles]={0};
    double runNumber[nFiles]={0.0};

    double meanT[nFiles]={0};
    double sigT[nFiles]={0};

    double meanG[nFiles]={0};
    double sigG[nFiles]={0};
    
    for(int j=0; j<nFiles; j++){
      mean[j] = Mean[j][i];
      sig[j]= Sigma[j][i];
      meanT[j] = MeanT[j][i];
      sigT[j]= SigmaT[j][i];
      meanG[j] = MeanG[j][i];
      sigG[j]= SigmaG[j][i];
      runNumber[j]=RunNumber[j];
    }

    g[i][0]= new TGraphErrors(nFiles-1, runNumber, mean, erRun, sig);
    g[i][1]= new TGraphErrors(nFiles-1, runNumber, meanT, erRun, sigT);
    g[i][2]= new TGraphErrors(nFiles-1, runNumber, meanG, erRun, sigG);
    
    g[i][0]->SetMarkerStyle(20);
    g[i][0]->SetMarkerColor(31);
    g[i][0]->SetLineColor(31);
    
    g[i][1]->SetMarkerStyle(21);
    g[i][1]->SetMarkerColor(41);
    g[i][1]->SetLineColor(41);
    
    g[i][2]->SetMarkerStyle(22);
    g[i][2]->SetMarkerColor(44);
    g[i][2]->SetLineColor(44);
    
    g[i][0]->SetLineWidth(2);
    g[i][1]->SetLineWidth(2);
    g[i][2]->SetLineWidth(2);
    g[i][2]->SetLineStyle(3);
    
    cgraph->cd();
    TLegend* leg = new TLegend(0.65, 0.75, 0.88, 0.88);
    leg->AddEntry(g[i][0],"PMT" ,"lep");
    leg->AddEntry(g[i][1],"Total" ,"lep");
    leg->AddEntry(g[i][2],"Gas" ,"lep");
    
    g[i][0]->SetTitle(hNames[i]);
    g[i][0]->GetXaxis()->SetTitle("Run#");
    g[i][0]->GetXaxis()->SetNoExponent(kTRUE);
    g[i][0]->GetYaxis()->SetTitle("Ratio (#mu #pm #sigma)");
    g[i][0]->GetYaxis()->SetRangeUser(0.50,5.50);
    
    cgraph->cd(i+1);
    g[i][0]->Draw("AP");
    g[i][0]->Write();
    g[i][1]->Draw("P SAMES");
    g[i][2]->Draw("P SAMES");
    leg->Draw();
  }
  //cgraph->SaveAs("ratio.root");
#ifdef _PREVIOUS_
  cgraph->Print("bkgratio_Previous.pdf");
  c[nFiles-1]->Print("bkgratio_Previous.pdf]");
#else
  cgraph->Print("bkgratio_2012.pdf");
  c[nFiles-1]->Print("bkgratio_2012.pdf]");
#endif
  outFile->Write();
  outFile->Close();
#endif
#endif
  
#ifdef _BKGMAPS_  
  TCanvas *cbkg[nFiles];
  int l =0;
  for(int i=0; i<histCollection.size(); i++){
    if(i%nRegions == 0){
      l = i/nRegions;
      cname.Form("Bmap_%d",RunNumber[i/nRegions]);
      cbkg[l]= new TCanvas(cname,cname,900,900);
      cbkg[l]->Divide(2,2);
      TPaveText* pt = new TPaveText(0.5, 0.5, 0.5, 0.5, "NDC NB");
      cname.Form("Run %d",RunNumber[i/nRegions]);
      pt->AddText(cname);
      pt->SetFillColor(0);  // Transparent
      pt->SetTextFont(42);  // Helvetica
      pt->SetTextSize(0.02);
      pt->SetLineWidth(0);
      pt->Draw();
    }
    if(i==0)cbkg[0]->Print("BackGroundMaps_W9.pdf[");
    gPad->SetLogz();
    cbkg[l]->cd(i%nRegions+1);
    //printf("--->%d %d\n",l, i%nRegions+1);
    histCollection[i]->Draw("colz");
    histCollection[i]->GetZaxis()->SetLabelSize(0.025);  // increase font size
    histCollection[i]->GetZaxis()->SetTitleOffset(0.8); // move title away from axis
    
    
    gStyle->SetOptStat(0);
    //gStyle->SetPalette(kRainBow);  // or another palette
    gStyle->SetNumberContours(99); // smoother gradient    
    // Optional: expand margins
    gPad->SetRightMargin(0.1);  // more space for palette
    //cbkg->Update();
    if(i%nRegions == 3){
      gPad->SetLogz();
      cbkg[l]->Print("BackGroundMaps_W9.pdf");
    }
  }//for
  cbkg[nFiles-1]->Print("BackGroundMaps_W9.pdf]");
  //printf("%d",sizeof(cbkg)/sizeof(cbkg[0]));
#endif
}
