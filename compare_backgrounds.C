#include<iostream>
#include<fstream>
using namespace std;

void compare_backgrounds(int runNo){
  //TString name_map1="back-para-histsum-271585-0-8.2016.P02.new-vector74-root";
  TString name_map1;
  name_map1.Form("./back-para-histsum-%d-0-8.new-vector74-root",runNo);
  //string name_map1="/gpfs/compass/fsozzi/transversity/analysis/RICH/testbackmap/back-para-2012.new-vector74-root";


  ifstream INPUT_map1;
  INPUT_map1.open(name_map1);
	
  double tmp;
  int counter1=0;

  /*	while(INPUT_map1>>tmp){
	counter1++;
	}
  */

  TH2F* h2_background_map1_top_saleve=new TH2F("h2_background_map1_top_saleve","top saleve",74,0,74,37,0,37);
  TH2F* h2_background_map1_top_jura=new TH2F("h2_background_map1_top_jura","top jura",74,0,74,37,0,37);
  TH2F* h2_background_map1_bottom_saleve=new TH2F("h2_background_map1_bottom_saleve","bottom saleve",74,0,74,37,0,37);
  TH2F* h2_background_map1_bottom_jura=new TH2F("h2_background_map1_bottom_jura","bottom jura",74,0,74,37,0,37);
	
  //-----------------------------------------
  //first 74 x 37 numbers are for top saleve
  //----------------------------------------
  int bin_x=74;
  int bin_y=37;
  //first 74 numbers belong to 1st y bin... second 74 numbers belong to 2nd y bin
  for(int j=1; j<=bin_y; j++){
    for(int i=1;i<=bin_x;i++){
      INPUT_map1>>tmp;
      int global_bin= h2_background_map1_top_saleve->GetBin(i,j);
      h2_background_map1_top_saleve->SetBinContent(global_bin,tmp);
    }
  }

  //-----------------------------------------
  //second 74 x 37 numbers are for top jura
  //----------------------------------------
  //int bin_x=74;
  //int bin_y=37;
  //first 74 numbers belong to 1st y bin... second 74 numbers belong to 2nd y bin
  for(int j=1; j<=bin_y; j++){
    for(int i=1;i<=bin_x;i++){
      INPUT_map1>>tmp;
      int global_bin= h2_background_map1_top_jura->GetBin(i,j);
      h2_background_map1_top_jura->SetBinContent(global_bin,tmp);
    }
  }

  //-----------------------------------------
  //third 74 x 37 numbers are for bottom saleve
  //----------------------------------------
  //int bin_x=74;
  //int bin_y=37;
  //first 74 numbers belong to 1st y bin... second 74 numbers belong to 2nd y bin
  for(int j=1; j<=bin_y; j++){
    for(int i=1;i<=bin_x;i++){
      INPUT_map1>>tmp;
      int global_bin= h2_background_map1_bottom_saleve->GetBin(i,j);
      h2_background_map1_bottom_saleve->SetBinContent(global_bin,tmp);
    }
  }

  //-----------------------------------------
  //fourth 74 x 37 numbers are for bottom jura
  //----------------------------------------
  //int bin_x=74;
  //int bin_y=37;
  //first 74 numbers belong to 1st y bin... second 74 numbers belong to 2nd y bin
  for(int j=1; j<=bin_y; j++){
    for(int i=1;i<=bin_x;i++){
      INPUT_map1>>tmp;
      int global_bin= h2_background_map1_bottom_jura->GetBin(i,j);
      h2_background_map1_bottom_jura->SetBinContent(global_bin,tmp);
    }
  }

	

  //draw.....
  TCanvas *can=new TCanvas("can","can");
  can->Divide(2,2);
  can->cd(1);
  h2_background_map1_top_saleve->SetMaximum(0.25*0.001);
  h2_background_map1_top_saleve->Draw("LEGO FB BB");
  can->cd(2);
  h2_background_map1_top_jura->SetMaximum(0.25*0.001);
  h2_background_map1_top_jura->Draw("LEGO FB BB");
  can->cd(3);
  h2_background_map1_bottom_saleve->SetMaximum(0.25*0.001);
  h2_background_map1_bottom_saleve->Draw("LEGO FB BB");
  can->cd(4);
  h2_background_map1_bottom_jura->SetMaximum(0.25*0.001);
  h2_background_map1_bottom_jura->Draw("LEGO FB BB");
  TString out_file;
  out_file.Form("BackgroundMap_%d_Display.root",runNo);

  TFile *ofile = new TFile(out_file,"RECREATE");
  ofile->cd();
  h2_background_map1_top_saleve->Write();
  h2_background_map1_top_jura->Write();
  h2_background_map1_bottom_saleve->Write();
  h2_background_map1_bottom_jura->Write();
  ofile->Write();
  ofile->Close();
}

