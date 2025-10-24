#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

#include <stdlib.h>
#include <cstdio>
#include <cmath>

//------------------------------
#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TF2.h"
#include "TF12.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TMinuit.h"
#include "TMath.h"
#include "TPostScript.h"
#include "TLine.h"
#include "TArc.h"
#include "TStyle.h"
#include "TLatex.h"

using namespace std;

void generate_background_map(){
	//#####################################################
	//
	//DST file with RICH histograms
	//
	//#####################################################
	TString histo_file_name =
	"/gpfs/compass/chchatte/2016/MAPMT_det_mir_updated_mom80/coral_output/backup/hist_cdr14048-274527.root";
	
	TString map_file_name="./Test_back-para-14048-274527-2016.new-vector74-root";

	//######################################################
	//######################################################







	TFile *histo_file=new TFile(histo_file_name);
	TDirectory *rich_dir=(TDirectory*)histo_file->Get("RICH");
	cout<<"now dealing with.........."<<histo_file_name<<endl;
	//two critical histograms: 
	//1D: 3521
	//2D: 3525
	
	//---get 3521
	TH1F *h1_3521=(TH1F*)rich_dir->Get("3521");
	int N_events=h1_3521->Integral();
	//h1_3521->Draw();
	cout<<"number of events: "<<N_events<<endl;


	//---get 3525, 2D histogram
	TH2F* h2_3525 = (TH2F*) rich_dir->Get("3525");
	//h2_3525->Draw("LEGO FB BB");
	h2_3525->Draw("scat");
	

	//---detector area is divided into 4 parts, top SALEVE, top Jura, bottom SALEVE, bottom Jura
		
	//------------------------
	//---first do top SALEVE
	//to save top saleve info (4 detectors/cathodes) into a new TH2F tank
	//------------------------
	int top_saleve_x_min=23;
	int top_saleve_x_max=98; // 76...

	int top_saleve_bin_x= top_saleve_x_max - top_saleve_x_min + 1;

	int top_saleve_y_min=118;
	int top_saleve_y_max=191; // 74...

	int top_saleve_bin_y = top_saleve_y_max - top_saleve_y_min +1;

	TH2F* tank_top_saleve_3525=new TH2F("tank_top_saleve_3525","top saleve", top_saleve_bin_x, 0, float(top_saleve_bin_x), top_saleve_bin_y, 0, float(top_saleve_bin_y));
	int jx, jy;
	jx = 1;
	for( int kx=top_saleve_x_min; kx<=top_saleve_x_max; kx++ ) {
		jy = 1;
		for( int ky=top_saleve_y_min; ky<=top_saleve_y_max; ky++ ) {
			int kb = h2_3525->GetBin( kx, ky );
			
			double coo = h2_3525->GetBinContent( kb );
			
			int jb = tank_top_saleve_3525->GetBin( jx, jy );
			
			tank_top_saleve_3525->SetBinContent( jb, coo );
			
			jy++;
		}
		jx++;
	} //now the tank only save the top saleve part
	tank_top_saleve_3525->Scale(1.0/N_events);
	//tank_top_saleve_3525->Draw("COLZ");

	//------------------------
	//---do top JURA
	//to save top jura info (4 detectors/cathodes) into a new TH2F tank
	//------------------------
	int top_jura_x_min=103;
	int top_jura_x_max=178; // 76...

	int top_jura_bin_x= top_jura_x_max - top_jura_x_min + 1;

	int top_jura_y_min=118;
	int top_jura_y_max=191; // 74...

	int top_jura_bin_y = top_jura_y_max - top_jura_y_min +1;

	TH2F* tank_top_jura_3525=new TH2F("tank_top_jura_3525","top jura", top_jura_bin_x, 0, float(top_jura_bin_x), top_jura_bin_y, 0, float(top_jura_bin_y));
	//int jx, jy;
	jx = 1;
	for( int kx=top_jura_x_min; kx<=top_jura_x_max; kx++ ) {
		jy = 1;
		for( int ky=top_jura_y_min; ky<=top_jura_y_max; ky++ ) {
			int kb = h2_3525->GetBin( kx, ky );
			
			double coo = h2_3525->GetBinContent( kb );
			
			int jb = tank_top_jura_3525->GetBin( jx, jy );
			
			tank_top_jura_3525->SetBinContent( jb, coo );
			
			jy++;
		}
		jx++;
	} //now the tank only save the top jura part
	tank_top_jura_3525->Scale(1.0/N_events);
	//tank_top_jura_3525->Draw("COLZ");
	
	
	//------------------------
	//---do bottom SALEVE
	//to save bottom saleve info (4 detectors/cathodes) into a new TH2F tank
	//------------------------
	int bottom_saleve_x_min=23;
	int bottom_saleve_x_max=98; // 76...

	int bottom_saleve_bin_x= bottom_saleve_x_max - bottom_saleve_x_min + 1;

	int bottom_saleve_y_min=10;
	int bottom_saleve_y_max=83; // 74...

	int bottom_saleve_bin_y = bottom_saleve_y_max - bottom_saleve_y_min +1;

	TH2F* tank_bottom_saleve_3525=new TH2F("tank_bottom_saleve_3525","bottom saleve", bottom_saleve_bin_x, 0, float(bottom_saleve_bin_x), bottom_saleve_bin_y, 0, float(bottom_saleve_bin_y));
	//int jx, jy;
	jx = 1;
	for( int kx=bottom_saleve_x_min; kx<=bottom_saleve_x_max; kx++ ) {
		jy = 1;
		for( int ky=bottom_saleve_y_min; ky<=bottom_saleve_y_max; ky++ ) {
			int kb = h2_3525->GetBin( kx, ky );
			
			double coo = h2_3525->GetBinContent( kb );
			
			int jb = tank_bottom_saleve_3525->GetBin( jx, jy );
			
			tank_bottom_saleve_3525->SetBinContent( jb, coo );
			
			jy++;
		}
		jx++;
	} //now the tank only save the bottom saleve part
	tank_bottom_saleve_3525->Scale(1.0/N_events);
	//tank_bottom_saleve_3525->Draw("COLZ");

	//------------------------
	//---do bottom jura
	//to save bottom jura info (4 detectors/cathodes) into a new TH2F tank
	//------------------------
	int bottom_jura_x_min=103;
	int bottom_jura_x_max=178; // 76...

	int bottom_jura_bin_x= bottom_jura_x_max - bottom_jura_x_min + 1;

	int bottom_jura_y_min=10;
	int bottom_jura_y_max=83; // 74...

	int bottom_jura_bin_y = bottom_jura_y_max - bottom_jura_y_min +1;

	TH2F* tank_bottom_jura_3525=new TH2F("tank_bottom_jura_3525","bottom jura", bottom_jura_bin_x, 0, float(bottom_jura_bin_x), bottom_jura_bin_y, 0, float(bottom_jura_bin_y));
	//int jx, jy;
	jx = 1;
	for( int kx=bottom_jura_x_min; kx<=bottom_jura_x_max; kx++ ) {
		jy = 1;
		for( int ky=bottom_jura_y_min; ky<=bottom_jura_y_max; ky++ ) {
			int kb = h2_3525->GetBin( kx, ky );
			
			double coo = h2_3525->GetBinContent( kb );
			
			int jb = tank_bottom_jura_3525->GetBin( jx, jy );
			
			tank_bottom_jura_3525->SetBinContent( jb, coo );
			
			jy++;
		}
		jx++;
	} //now the tank only save the bottom jura part
	tank_bottom_jura_3525->Scale(1.0/N_events);
	//tank_bottom_jura_3525->Draw("COLZ");
	









	//###################################################
	//do output to background map to a file
	//###################################################
	ofstream OUTPUT_map;
	OUTPUT_map.open(map_file_name);
	if(!OUTPUT_map){
		cout<<"can't open the file to write background map!"<<endl;
		cout<<"No map output..."<<endl;
	}
	double x_width=8.0*2;  //2 pads, 8mm per pad
	double y_width=8.0*2.0*2.0;  //2 addional aveage in y direction
	double dcdxdy=x_width*y_width;


	int nSliX = 37;
	int kco = 0;
	
	//top saleve
	std::vector<double> hCont1;
	for( int ks=1; ks<=nSliX; ks++ ) {
		int first_bin = 2*ks - 1;
		int last_bin = 2*ks;    //for one x bin, average 2 y bins
		
		TH1D* pSliX = tank_top_saleve_3525->ProjectionX( "XX", first_bin, last_bin, "" );
		//TH1D* pSliX_ = tank_top_saleve_3525->ProjectionX( "XX", first_bin, last_bin, "" );
		//pSliX->Draw();	
		for( Int_t kc=1; kc<=top_saleve_bin_x; kc++ ) {
			double coo = pSliX->GetBinContent( kc );
			if( coo < 0. ) coo = 0.;
			pSliX->SetBinContent( kc, coo );  //deal with unphysical thing
			//pSliX_->SetBinContent( kc, coo );  //deal with unphysical thing
		}
		
		pSliX->Smooth( 3 );
		pSliX->Scale( 1./dcdxdy ); //don't remember to scale by event number in the beginning

		for( Int_t kc=2; kc<=top_saleve_bin_x-1; kc++ ) { //74 numbers
			double coo = pSliX->GetBinContent( kc );
			OUTPUT_map << coo << " ";
			hCont1.push_back( coo );
			kco++;
		}
	}//done with top saleve
	//pSliX->Draw();
	TCanvas *c = new TCanvas();
	pSliX->Draw();

	//top jura
	std::vector<double> hCont2;
	kco = 0;
	for( int ks=1; ks<=nSliX; ks++ ) {
		int first_bin = 2*ks - 1;
		int last_bin = 2*ks;    //for one x bin, average 2 y bins
		
		TH1D* pSliX = tank_top_jura_3525->ProjectionX( "XX", first_bin, last_bin, "" );
		
		for( Int_t kc=1; kc<=top_jura_bin_x; kc++ ) {
			double coo = pSliX->GetBinContent( kc );
			if( coo < 0. ) coo = 0.;
			pSliX->SetBinContent( kc, coo );  //deal with unphysical thing
		}
		
		pSliX->Smooth( 3 );
		pSliX->Scale( 1./dcdxdy ); //don't remember to scale by event number in the beginning

		for( Int_t kc=2; kc<=top_jura_bin_x-1; kc++ ) { //74 numbers
			double coo = pSliX->GetBinContent( kc );
			OUTPUT_map << coo << " ";
			hCont2.push_back( coo );
			kco++;
		}
	}//done with top jura

	
	//bottom saleve
	std::vector<double> hCont3;
	kco = 0;
	for( int ks=1; ks<=nSliX; ks++ ) {
		int first_bin = 2*ks - 1;
		int last_bin = 2*ks;    //for one x bin, average 2 y bins
		
		TH1D* pSliX = tank_bottom_saleve_3525->ProjectionX( "XX", first_bin, last_bin, "" );
		
		for( Int_t kc=1; kc<=bottom_saleve_bin_x; kc++ ) {
			double coo = pSliX->GetBinContent( kc );
			if( coo < 0. ) coo = 0.;
			pSliX->SetBinContent( kc, coo );  //deal with unphysical thing
		}
		
		pSliX->Smooth( 3 );
		pSliX->Scale( 1./dcdxdy ); //don't remember to scale by event number in the beginning

		for( Int_t kc=2; kc<=bottom_saleve_bin_x-1; kc++ ) { //74 numbers
			double coo = pSliX->GetBinContent( kc );
			OUTPUT_map << coo << " ";
			hCont3.push_back( coo );
			kco++;
		}
	}//done with bottom saleve


	//bottom jura
	std::vector<double> hCont4;
	kco = 0;
	for( int ks=1; ks<=nSliX; ks++ ) {
		int first_bin = 2*ks - 1;
		int last_bin = 2*ks;    //for one x bin, average 2 y bins
		
		TH1D* pSliX = tank_bottom_jura_3525->ProjectionX( "XX", first_bin, last_bin, "" );
		
		for( Int_t kc=1; kc<=bottom_jura_bin_x; kc++ ) {
			double coo = pSliX->GetBinContent( kc );
			if( coo < 0. ) coo = 0.;
			pSliX->SetBinContent( kc, coo );  //deal with unphysical thing
		}
		
		pSliX->Smooth( 3 );
		pSliX->Scale( 1./dcdxdy ); //don't remember to scale by event number in the beginning

		for( Int_t kc=2; kc<=bottom_jura_bin_x-1; kc++ ) { //74 numbers
			double coo = pSliX->GetBinContent( kc );
			OUTPUT_map << coo << " ";
			hCont4.push_back( coo );
			kco++;
		}
	}//done with bottom jura

	OUTPUT_map.close();



}

