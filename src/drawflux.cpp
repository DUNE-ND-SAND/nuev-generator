#include "TFile.h"
#include "TDirectoryFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TROOT.h"
#include "TPad.h"


void drawflux(const char* fIn)
{
	TFile* f= new TFile(fIn,"UPDATE");
	
	////fluxes////
	
	TH1D * hnumu_flux = new TH1D("hnumu_flux","hnumu_flux" , 100, 0, 80);
    hnumu_flux = (TH1D*)f->Get("numu_flux");
	
	TH1D * hnue_flux = new TH1D("hnue_flux","hnue_flux" , 100, 0, 80);
    hnue_flux = (TH1D*)f->Get("nue_flux");
	
	TH1D * hnumubar_flux = new TH1D("hnumubar_flux","hnumubar_flux" , 100, 0, 80);
    hnumubar_flux = (TH1D*)f->Get("numubar_flux");
	
	TH1D * hnuebar_flux = new TH1D("hnuebar_flux","hnuebar_flux" , 100, 0, 80);
    hnuebar_flux = (TH1D*)f->Get("nuebar_flux");
	
	////CCeventrates////
	
	TH1D * hnumu_cceventrate = new TH1D("hnumu_cceventrate","hnumu_cceventrate" , 100, 0, 80);
    hnumu_cceventrate = (TH1D*)f->Get("numu_cceventrate");
	
	TH1D * hnue_cceventrate = new TH1D("hnue_cceventrate","hnue_cceventrate" , 100, 0, 80);
    hnue_cceventrate = (TH1D*)f->Get("nue_cceventrate");
	
	TH1D * hnumubar_cceventrate = new TH1D("hnumubar_cceventrate","hnumubar_cceventrate" , 100, 0, 80);
    hnumubar_cceventrate = (TH1D*)f->Get("numubar_cceventrate");
	
	TH1D * hnuebar_cceventrate = new TH1D("hnuebar_cceventrate","hnuebar_cceventrate" , 100, 0, 80);
    hnuebar_cceventrate = (TH1D*)f->Get("nuebar_cceventrate");
	
	////NCeventrates////
	
	TH1D * hnumu_nceventrate = new TH1D("hnumu_nceventrate","hnumu_nceventrate" , 100, 0, 80);
    hnumu_nceventrate = (TH1D*)f->Get("numu_nceventrate");
	
	TH1D * hnue_nceventrate = new TH1D("hnue_nceventrate","hnue_nceventrate" , 100, 0, 80);
    hnue_nceventrate = (TH1D*)f->Get("nue_nceventrate");
	
	TH1D * hnumubar_nceventrate = new TH1D("hnumubar_nceventrate","hnumubar_nceventrate" , 100, 0, 80);
    hnumubar_nceventrate = (TH1D*)f->Get("numubar_nceventrate");
	
	TH1D * hnuebar_nceventrate = new TH1D("hnuebar_nceventrate","hnuebar_nceventrate" , 100, 0, 80);
    hnuebar_nceventrate = (TH1D*)f->Get("nuebar_nceventrate");
	
	//////draw neutrino fluxes///////
	
	TCanvas *c1 = new TCanvas("c1","c1",200,10,900,800);
	gStyle->SetOptStat(0);
	gPad->SetLogy();
	
	hnumu_flux->SetTitle("");
	hnumu_flux->GetXaxis()->SetTitle("Energy (GeV)");
	hnumu_flux->GetYaxis()->SetTitle("log (#nus/m^{2}/POT)");
	hnumu_flux->SetLineColor(kRed);
	//hnumu_flux->SetLineWidth(2);
	hnumu_flux->SetFillColor(kRed-10);
	hnumu_flux->GetXaxis()->SetRangeUser(0., 80.);
	hnumu_flux->SetFillStyle(3001);
	
	hnue_flux->SetLineColor(kBlue);
	//hnue_flux->SetLineWidth(2);
	hnue_flux->SetFillColor(kBlue-10);
	hnue_flux->GetXaxis()->SetRangeUser(0., 80.);
	hnue_flux->SetFillStyle(3001);
	
	hnumu_flux->Draw("hist E1");
	hnue_flux->Draw("hist E1 same");
	
	
	TLegend* legend = new TLegend(0.62,0.8,0.894,0.9);
	legend->AddEntry( hnumu_flux, "#nu_{#mu} flux", "f");
    legend->AddEntry( hnue_flux, "#nu_{e} flux", "f");
	legend->SetFillColor(0);
	legend->SetBorderSize(1);
	legend->Draw("");
	
	c1->Modified();
	c1->Update();
	gSystem->ProcessEvents();
	
	//////draw anti-neutrino fluxes///////
	
	TCanvas *c2 = new TCanvas("c2","c2",200,10,900,800);
	gStyle->SetOptStat(0);
	gPad->SetLogy();
	
	hnumubar_flux->SetTitle("");
	hnumubar_flux->GetXaxis()->SetTitle("Energy (GeV)");
	hnumubar_flux->GetYaxis()->SetTitle("log (#nus/m^{2}/POT)");
	hnumubar_flux->SetLineColor(kRed);
	//hnumubar_flux->SetLineWidth(2);
	hnumubar_flux->SetFillColor(kRed-10);
	hnumubar_flux->GetXaxis()->SetRangeUser(0., 80.);
	hnumubar_flux->SetFillStyle(3001);
	
	hnuebar_flux->SetLineColor(kBlue);
	//hnuebar_flux->SetLineWidth(2);
	hnuebar_flux->SetFillColor(kBlue-10);
	hnuebar_flux->GetXaxis()->SetRangeUser(0., 80.);
	hnuebar_flux->SetFillStyle(3001);
	
	hnumubar_flux->Draw("E1 hist");
	hnuebar_flux->Draw("E1 hist  same");
	
	
	TLegend* legend2 = new TLegend(0.62,0.8,0.894,0.9);
	legend2->AddEntry( hnumu_flux, "#bar{#nu}_{#mu} flux", "f");
    legend2->AddEntry( hnue_flux, "#bar{#nu}_{e} flux", "f");
	legend2->SetFillColor(0);
	legend2->SetBorderSize(1);
	legend2->Draw("");
	
	c2->Modified();
	c2->Update();
	gSystem->ProcessEvents();
	
	
	//////draw neutrino cceventrates and nceventrates///////
	
	TCanvas *c3 = new TCanvas("c3","c3",200,10,900,800);
	gStyle->SetOptStat(0);
	gPad->SetLogy();
	
	
	hnumu_cceventrate->SetTitle("");
	hnumu_cceventrate->GetXaxis()->SetTitle("Energy (GeV)");
	hnumu_cceventrate->GetYaxis()->SetTitle("log (events/kton/POT)");
	hnumu_cceventrate->SetLineColor(kRed);
	//hnumu_cceventrate->SetLineWidth(2);
	hnumu_cceventrate->SetFillColor(kRed-10);
	hnumu_cceventrate->GetXaxis()->SetRangeUser(0., 80.);
	hnumu_cceventrate->SetFillStyle(3001);
	
	hnue_cceventrate->SetLineColor(kBlue);
	//hnue_cceventrate->SetLineWidth(2);
	hnue_cceventrate->SetFillColor(kBlue-10);
	hnue_cceventrate->GetXaxis()->SetRangeUser(0., 80.);
	hnue_cceventrate->SetFillStyle(3001);
	
	hnumu_nceventrate->SetLineColor(kYellow);
	//hnumu_nceventrate->SetLineWidth(2);
	hnumu_nceventrate->SetFillColor(kYellow-10);
	hnumu_nceventrate->GetXaxis()->SetRangeUser(0., 80.);
	hnumu_nceventrate->SetFillStyle(3001);
	
	hnue_nceventrate->SetLineColor(kGreen);
	//hnue_nceventrate->SetLineWidth(2);
	hnue_nceventrate->SetFillColor(kGreen-10);
	hnue_nceventrate->GetXaxis()->SetRangeUser(0., 80.);
	hnue_nceventrate->SetFillStyle(3001);
	
	hnumu_cceventrate->Draw("E1 hist");
	hnue_cceventrate->Draw("E1 hist  same");
	hnumu_nceventrate->Draw("E1 hist  same");
	hnue_nceventrate->Draw("E1 hist  same");
	
	
	TLegend* legend3 = new TLegend(0.62,0.8,0.894,0.9);
	legend3->AddEntry( hnumu_cceventrate, "#nu_{#mu} CC", "f");
    legend3->AddEntry( hnue_cceventrate, "#nu_{e} CC", "f");
	legend3->AddEntry( hnumu_nceventrate, "#nu_{#mu} NC", "f");
    legend3->AddEntry( hnue_nceventrate, "#nu_{e} NC", "f");
	legend3->SetFillColor(0);
	legend3->SetBorderSize(1);
	legend3->Draw("");
	
	c3->Modified();
	c3->Update();
	gSystem->ProcessEvents();
	
	//////draw anti-neutrino cceventrates and nceventrates///////
	
	TCanvas *c4 = new TCanvas("c4","c4",200,10,900,800);
	gStyle->SetOptStat(0);
	gPad->SetLogy();
	
	hnumubar_cceventrate->SetTitle("");
	hnumubar_cceventrate->GetXaxis()->SetTitle("Energy (GeV)");
	hnumubar_cceventrate->GetYaxis()->SetTitle("log (events/kton/POT)");
	hnumubar_cceventrate->SetLineColor(kRed);
	//hnumubar_cceventrate->SetLineWidth(2);
	hnumubar_cceventrate->SetFillColor(kRed-10);
	hnumubar_cceventrate->GetXaxis()->SetRangeUser(0., 80.);
	hnumubar_cceventrate->SetFillStyle(3001);
	
	hnuebar_cceventrate->SetLineColor(kBlue);
	//hnuebar_cceventrate->SetLineWidth(2);
	hnuebar_cceventrate->SetFillColor(kBlue-10);
	hnuebar_cceventrate->GetXaxis()->SetRangeUser(0., 80.);
	hnuebar_cceventrate->SetFillStyle(3001);
	
	hnumubar_nceventrate->SetLineColor(kYellow);
	//hnumubar_nceventrate->SetLineWidth(2);
	hnumubar_nceventrate->SetFillColor(kYellow-10);
	hnumubar_nceventrate->GetXaxis()->SetRangeUser(0., 80.);
	hnumubar_nceventrate->SetFillStyle(3001);
	
	hnuebar_nceventrate->SetLineColor(kGreen);
	//hnuebar_nceventrate->SetLineWidth(2);
	hnuebar_nceventrate->SetFillColor(kGreen-10);
	hnuebar_nceventrate->GetXaxis()->SetRangeUser(0., 80.);
	hnuebar_nceventrate->SetFillStyle(3001);
	
	hnumubar_cceventrate->Draw("E1 hist");
	hnuebar_cceventrate->Draw("E1 hist  same");
	hnumubar_nceventrate->Draw("E1 hist  same");
	hnuebar_nceventrate->Draw("E1 hist  same");
	
	
	TLegend* legend4 = new TLegend(0.62,0.8,0.894,0.9);
	legend4->AddEntry( hnumubar_cceventrate, "#bar{#nu}_{#mu} CC", "f");
    legend4->AddEntry( hnuebar_cceventrate, "#bar{#nu}_{e} CC", "f");
	legend4->AddEntry( hnumubar_nceventrate, "#bar{#nu}_{#mu} NC", "f");
    legend4->AddEntry( hnuebar_nceventrate, "#bar{#nu}_{e} NC", "f");
	legend4->SetFillColor(0);
	legend4->SetBorderSize(1);
	legend4->Draw("");
	
	c4->Modified();
	c4->Update();
	gSystem->ProcessEvents();
	
	/////save plots /////
	
	c1->SaveAs("/mnt/c/Linux/Dune/nuev-generator/src/nuFlux.png");
	c2->SaveAs("/mnt/c/Linux/Dune/nuev-generator/src/nuBarFlux.png");
	c3->SaveAs("/mnt/c/Linux/Dune/nuev-generator/src/nuEventRates.png");
	c4->SaveAs("/mnt/c/Linux/Dune/nuev-generator/src/nuBarEventRates.png");
	

     
}