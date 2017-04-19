#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TList.h"
#include "TGaxis.h"
#include "TMath.h"
#include <stdio.h>
#include <TROOT.h>
#include "TSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>


void savePlot(TCanvas*, TString);
void prepareHisto (TH1F* hist, Int_t rebin, Double_t weight, Color_t color);

Double_t myfunction(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Double_t f = par[0]+par[1]*xx+par[2]*pow(xx,2)+par[3]*pow(xx,3)+par[4]*pow(xx,4)+par[5]*pow(xx,5)+par[6]*pow(xx,6);
  return f;
}

void TestME0Background()
{

  TH1::SetDefaultSumw2();
  cout<<"ciao"<<endl;
  TFile* file = TFile::Open("ME0Digis_out.root");
  TString tDir = "DQMData/MuonME0DigisV/ME0DigisTask";
  TString total     = tDir+"/me0_strip_dg_bkg_radius_tot";
  TString electrons = tDir+"/me0_strip_dg_bkgElePos_radius";
  TString neutrons  = tDir+"/me0_strip_dg_bkgNeutral_radius";

  cout<<"ciao"<<endl;
  TH1F* totalBkg     = (TH1F*) file->Get(total);
  TH1F* electronsBkg = (TH1F*) file->Get(electrons);
  TH1F* neutronsBkg  = (TH1F*) file->Get(neutrons);

  cout<<"ciao  "<<electronsBkg<<"   "<<neutronsBkg << endl;
  Double_t weight    = 12000*9*25e-09;

  prepareHisto(totalBkg,5,weight,kBlack);
  prepareHisto(electronsBkg,5,weight,kBlue+1);
  prepareHisto(neutronsBkg,5,weight,kRed+1);


  cout<<"ciao"<<endl;
 

  TLegend *leg_hits = new TLegend(0.60,0.60,0.85,0.85);
  leg_hits->SetFillColor(0);
  leg_hits->SetHeader("ME0 Neutron Bkg Hits");
  leg_hits->AddEntry(totalBkg,"total Bkgs","l");
  leg_hits->AddEntry(electronsBkg,"e^{#pm} Bkg ","l");
  leg_hits->AddEntry(neutronsBkg,"n Bkg","l");

  TCanvas *c_noPrompt = new TCanvas("NoPrompt_Rate","NoPrompt_Rate",51,52,600,600);
  c_noPrompt->Range(41.54844,-992.0508,165.6428,7722.734);
  c_noPrompt->SetLeftMargin(0.1672241);
  c_noPrompt->cd();
  c_noPrompt->SetTitle("ME0 Neutron Background Hits");
  //neutronsBkg->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
  //neutronsBkg->GetXaxis()->SetTitle("R (cm)");
  //neutronsBkg->GetYaxis()->SetTitleOffset(1.8);
  totalBkg->Draw("9p0e1");
  //gammasBkg->Draw("9samep0e1");
  electronsBkg->Draw("9samep0e1");
  neutronsBkg->Draw("9samep0e1");
  //leg_hits->Draw();

  c_noPrompt->Update();
  //savePlot(c_noPrompt,"NoPromptHitsRate");

  TCanvas *c = new TCanvas("BkgHitRate", "BkgHitRate", 600, 600);
  c->cd();
  c->SetTicks(1,1);
  c->SetGrid(1,1);
  c->SetLogy();
  c->SetTitle("ME0 Neutron Background Parameterization");
  // c->SetTitle("");

  TF1 *f1 = new TF1("myfunc1",myfunction,60,150,7);
  f1->SetParameters(899644.0, -30841.0, 441.28, -3.3405, 0.0140588, -3.11473e-05, 2.83736e-08);
  // f1->SetParNames("constant","coefficient");
  f1->Draw();

  TF1 *f2 = new TF1("myfunc2",myfunction,60,150,7);
  f2->SetParameters(4.68590e+05, -1.63834e+04, 2.35700e+02, -1.77706e+00, 7.39960e-03, -1.61448e-05, 1.44368e-08);
  f2->SetLineColor(kBlue);
  f2->Draw("same");

  TF1 *f3 = new TF1("myfunc3",myfunction,60,150,7);
  f3->SetParameters(5.69e+06, -293334, 6279.6, -71.2928, 0.452244, -0.0015191, 2.1106e-06);
  f3->SetLineColor(kRed);
  f3->SetLineStyle(2);
  f3->Draw("same");

  TF1 *f4 = new TF1("myfunc4",myfunction,60,150,7);
  f4->SetParameters(3.77712e+06, -199280, 4340.69, -49.922, 0.319699, -0.00108113, 1.50889e-06);
  f4->SetLineColor(kBlue);
  f4->SetLineStyle(2);
  f4->Draw("same");

  TLegend *leg = new TLegend(0.60,0.60,0.85,0.85);
  leg->SetFillColor(0);
  leg->SetHeader("ME0 Neutron Bkg Param");
  leg->AddEntry("myfunc1","n/#gamma Bkg SLHC","l");
  leg->AddEntry("myfunc2","e^{#pm} Bkg SLHC","l");
  leg->AddEntry("myfunc3","n/#gamma Bkg 76X","l");
  leg->AddEntry("myfunc4","e^{#pm} Bkg 76X","l");
  leg->Draw();


  /*f1->GetYaxis()->SetRangeUser(300.01,1.2E5);
  f1->GetYaxis()->SetTitleOffset(1.35);
  f1->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
  f1->GetXaxis()->SetTitle("R (cm)");
  c->Update();*/

  electronsBkg->GetYaxis()->SetRangeUser(60.01,1.2E5);
  electronsBkg->GetYaxis()->SetTitleOffset(1.35);
  electronsBkg->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
  electronsBkg->GetXaxis()->SetTitle("R (cm)");
  c->Update();
  //savePlot(c,"BkgParamSLHC26p3");

  TCanvas* c_all = new TCanvas("Superposition","Superposition",600,600);
  c_all->cd();
  c_all->SetTicks(1,1);
  c_all->SetGrid(1,1);
  c_all->SetLogy();
  c_all->SetTitle("ME0 Neutron Background Parameterization");

   TF1 *myFit = new TF1("*myFit",60,145,4);
    //The original function : func had originally been created by:
    //TF1 *myFit = new TF1("myFit",func,60,145,4);
   myFit->SetRange(60,145);
   myFit->SetName("myFit");
   //myFit->SetTitle("func");
   myFit->SetSavedPoint(0,219297.5);
   myFit->SetSavedPoint(1,171953.2);
   myFit->SetSavedPoint(2,136817.2);
   myFit->SetSavedPoint(3,110456.2);
   myFit->SetSavedPoint(4,90467.68);
   myFit->SetSavedPoint(5,75152.89);
   myFit->SetSavedPoint(6,63298.56);
   myFit->SetSavedPoint(7,54029.89);
   myFit->SetSavedPoint(8,46710.31);
   myFit->SetSavedPoint(9,40872.52);
   myFit->SetSavedPoint(10,36170.58);
   myFit->SetSavedPoint(11,32346.28);
   myFit->SetSavedPoint(12,29205.43);
   myFit->SetSavedPoint(13,26600.84);
   myFit->SetSavedPoint(14,24420.16);
   myFit->SetSavedPoint(15,22577.03);
   myFit->SetSavedPoint(16,21004.61);
   myFit->SetSavedPoint(17,19650.81);
   myFit->SetSavedPoint(18,18474.82);
   myFit->SetSavedPoint(19,17444.43);
   myFit->SetSavedPoint(20,16534.08);
   myFit->SetSavedPoint(21,15723.36);
   myFit->SetSavedPoint(22,14995.9);
   myFit->SetSavedPoint(23,14338.47);
   myFit->SetSavedPoint(24,13740.33);
   myFit->SetSavedPoint(25,13192.72);
   myFit->SetSavedPoint(26,12688.44);
   myFit->SetSavedPoint(27,12221.58);
   myFit->SetSavedPoint(28,11787.2);
   myFit->SetSavedPoint(29,11381.22);
   myFit->SetSavedPoint(30,11000.19);
   myFit->SetSavedPoint(31,10641.25);
   myFit->SetSavedPoint(32,10301.93);
   myFit->SetSavedPoint(33,9980.172);
   myFit->SetSavedPoint(34,9674.196);
   myFit->SetSavedPoint(35,9382.483);
   myFit->SetSavedPoint(36,9103.721);
   myFit->SetSavedPoint(37,8836.774);
   myFit->SetSavedPoint(38,8580.655);
   myFit->SetSavedPoint(39,8334.498);
   myFit->SetSavedPoint(40,8097.546);
   myFit->SetSavedPoint(41,7869.129);
   myFit->SetSavedPoint(42,7648.656);
   myFit->SetSavedPoint(43,7435.598);
   myFit->SetSavedPoint(44,7229.484);
   myFit->SetSavedPoint(45,7029.894);
   myFit->SetSavedPoint(46,6836.445);
   myFit->SetSavedPoint(47,6648.795);
   myFit->SetSavedPoint(48,6466.631);
   myFit->SetSavedPoint(49,6289.67);
   myFit->SetSavedPoint(50,6117.652);
   myFit->SetSavedPoint(51,5950.339);
   myFit->SetSavedPoint(52,5787.513);
   myFit->SetSavedPoint(53,5628.972);
   myFit->SetSavedPoint(54,5474.529);
   myFit->SetSavedPoint(55,5324.011);
   myFit->SetSavedPoint(56,5177.257);
   myFit->SetSavedPoint(57,5034.116);
   myFit->SetSavedPoint(58,4894.45);
   myFit->SetSavedPoint(59,4758.125);
   myFit->SetSavedPoint(60,4625.018);
   myFit->SetSavedPoint(61,4495.014);
   myFit->SetSavedPoint(62,4368.003);
   myFit->SetSavedPoint(63,4243.881);
   myFit->SetSavedPoint(64,4122.551);
   myFit->SetSavedPoint(65,4003.92);
   myFit->SetSavedPoint(66,3887.899);
   myFit->SetSavedPoint(67,3774.407);
   myFit->SetSavedPoint(68,3663.362);
   myFit->SetSavedPoint(69,3554.69);
   myFit->SetSavedPoint(70,3448.318);
   myFit->SetSavedPoint(71,3344.177);
   myFit->SetSavedPoint(72,3242.201);
   myFit->SetSavedPoint(73,3142.328);
   myFit->SetSavedPoint(74,3044.496);
   myFit->SetSavedPoint(75,2948.648);
   myFit->SetSavedPoint(76,2854.729);
   myFit->SetSavedPoint(77,2762.685);
   myFit->SetSavedPoint(78,2672.465);
   myFit->SetSavedPoint(79,2584.019);
   myFit->SetSavedPoint(80,2497.3);
   myFit->SetSavedPoint(81,2412.262);
   myFit->SetSavedPoint(82,2328.862);
   myFit->SetSavedPoint(83,2247.057);
   myFit->SetSavedPoint(84,2166.807);
   myFit->SetSavedPoint(85,2088.07);
   myFit->SetSavedPoint(86,2010.811);
   myFit->SetSavedPoint(87,1934.991);
   myFit->SetSavedPoint(88,1860.575);
   myFit->SetSavedPoint(89,1787.529);
   myFit->SetSavedPoint(90,1715.819);
   myFit->SetSavedPoint(91,1645.413);
   myFit->SetSavedPoint(92,1576.281);
   myFit->SetSavedPoint(93,1508.391);
   myFit->SetSavedPoint(94,1441.715);
   myFit->SetSavedPoint(95,1376.225);
   myFit->SetSavedPoint(96,1311.892);
   myFit->SetSavedPoint(97,1248.69);
   myFit->SetSavedPoint(98,1186.594);
   myFit->SetSavedPoint(99,1125.578);
   myFit->SetSavedPoint(100,0);
   myFit->SetSavedPoint(101,60);
   myFit->SetSavedPoint(102,145);
   myFit->SetFillColor(19);
   myFit->SetFillStyle(0);
   myFit->SetLineColor(2);
   myFit->SetLineWidth(2);
   myFit->SetChisquare(44.11221);
   myFit->SetNDF(15);
   myFit->GetXaxis()->SetLabelFont(42);
   myFit->GetXaxis()->SetLabelSize(0.035);
   myFit->GetXaxis()->SetTitleSize(0.035);
   myFit->GetXaxis()->SetTitleFont(42);
   myFit->GetYaxis()->SetLabelFont(42);
   myFit->GetYaxis()->SetLabelSize(0.035);
   myFit->GetYaxis()->SetTitleSize(0.035);
   myFit->GetYaxis()->SetTitleFont(42);
   myFit->SetParameter(0,0.003862567);
   myFit->SetParError(0,0.003937138);
   myFit->SetParLimits(0,0,0);
   myFit->SetParameter(1,6344.653);
   myFit->SetParError(1,511.64);
   myFit->SetParLimits(1,0,0);
   myFit->SetParameter(2,1.662775e+07);
   myFit->SetParError(2,1275819);
   myFit->SetParLimits(2,0,0);
   myFit->SetParameter(3,-102097.7);
   myFit->SetParError(3,9994.619);
   myFit->SetParLimits(3,0,0);
   myFit->SetLineColor(kRed+1);
   myFit->Draw();


   TF1 *myFit1 = new TF1("*myFit1",60,145,4);
    //The original function : func had originally been created by:
    //TF1 *myFit1 = new TF1("myFit1",func,60,145,4);
   myFit1->SetRange(60,145);
   myFit1->SetName("myFit1");
   //myFit1->SetTitle("func");
   myFit1->SetSavedPoint(0,31212.54);
   myFit1->SetSavedPoint(1,25725.87);
   myFit1->SetSavedPoint(2,21373.57);
   myFit1->SetSavedPoint(3,17894.27);
   myFit1->SetSavedPoint(4,15092.17);
   myFit1->SetSavedPoint(5,12819.4);
   myFit1->SetSavedPoint(6,10963.35);
   myFit1->SetSavedPoint(7,9437.694);
   myFit1->SetSavedPoint(8,8175.705);
   myFit1->SetSavedPoint(9,7125.489);
   myFit1->SetSavedPoint(10,6246.409);
   myFit1->SetSavedPoint(11,5506.439);
   myFit1->SetSavedPoint(12,4880.186);
   myFit1->SetSavedPoint(13,4347.398);
   myFit1->SetSavedPoint(14,3891.833);
   myFit1->SetSavedPoint(15,3500.392);
   myFit1->SetSavedPoint(16,3162.458);
   myFit1->SetSavedPoint(17,2869.381);
   myFit1->SetSavedPoint(18,2614.078);
   myFit1->SetSavedPoint(19,2390.726);
   myFit1->SetSavedPoint(20,2194.512);
   myFit1->SetSavedPoint(21,2021.443);
   myFit1->SetSavedPoint(22,1868.192);
   myFit1->SetSavedPoint(23,1731.975);
   myFit1->SetSavedPoint(24,1610.453);
   myFit1->SetSavedPoint(25,1501.653);
   myFit1->SetSavedPoint(26,1403.909);
   myFit1->SetSavedPoint(27,1315.802);
   myFit1->SetSavedPoint(28,1236.124);
   myFit1->SetSavedPoint(29,1163.843);
   myFit1->SetSavedPoint(30,1098.072);
   myFit1->SetSavedPoint(31,1038.05);
   myFit1->SetSavedPoint(32,983.1171);
   myFit1->SetSavedPoint(33,932.7039);
   myFit1->SetSavedPoint(34,886.3157);
   myFit1->SetSavedPoint(35,843.5213);
   myFit1->SetSavedPoint(36,803.9444);
   myFit1->SetSavedPoint(37,767.2556);
   myFit1->SetSavedPoint(38,733.1657);
   myFit1->SetSavedPoint(39,701.4202);
   myFit1->SetSavedPoint(40,671.7945);
   myFit1->SetSavedPoint(41,644.0901);
   myFit1->SetSavedPoint(42,618.131);
   myFit1->SetSavedPoint(43,593.7608);
   myFit1->SetSavedPoint(44,570.8403);
   myFit1->SetSavedPoint(45,549.2453);
   myFit1->SetSavedPoint(46,528.8646);
   myFit1->SetSavedPoint(47,509.5987);
   myFit1->SetSavedPoint(48,491.3581);
   myFit1->SetSavedPoint(49,474.0624);
   myFit1->SetSavedPoint(50,457.639);
   myFit1->SetSavedPoint(51,442.0225);
   myFit1->SetSavedPoint(52,427.1536);
   myFit1->SetSavedPoint(53,412.9784);
   myFit1->SetSavedPoint(54,399.4483);
   myFit1->SetSavedPoint(55,386.5187);
   myFit1->SetSavedPoint(56,374.1492);
   myFit1->SetSavedPoint(57,362.3029);
   myFit1->SetSavedPoint(58,350.9459);
   myFit1->SetSavedPoint(59,340.0474);
   myFit1->SetSavedPoint(60,329.5789);
   myFit1->SetSavedPoint(61,319.5144);
   myFit1->SetSavedPoint(62,309.8299);
   myFit1->SetSavedPoint(63,300.5033);
   myFit1->SetSavedPoint(64,291.5141);
   myFit1->SetSavedPoint(65,282.8436);
   myFit1->SetSavedPoint(66,274.4742);
   myFit1->SetSavedPoint(67,266.3899);
   myFit1->SetSavedPoint(68,258.5757);
   myFit1->SetSavedPoint(69,251.0175);
   myFit1->SetSavedPoint(70,243.7025);
   myFit1->SetSavedPoint(71,236.6187);
   myFit1->SetSavedPoint(72,229.7546);
   myFit1->SetSavedPoint(73,223.0999);
   myFit1->SetSavedPoint(74,216.6447);
   myFit1->SetSavedPoint(75,210.3798);
   myFit1->SetSavedPoint(76,204.2967);
   myFit1->SetSavedPoint(77,198.3872);
   myFit1->SetSavedPoint(78,192.6438);
   myFit1->SetSavedPoint(79,187.0593);
   myFit1->SetSavedPoint(80,181.6271);
   myFit1->SetSavedPoint(81,176.3409);
   myFit1->SetSavedPoint(82,171.1947);
   myFit1->SetSavedPoint(83,166.1829);
   myFit1->SetSavedPoint(84,161.3002);
   myFit1->SetSavedPoint(85,156.5416);
   myFit1->SetSavedPoint(86,151.9024);
   myFit1->SetSavedPoint(87,147.3781);
   myFit1->SetSavedPoint(88,142.9644);
   myFit1->SetSavedPoint(89,138.6573);
   myFit1->SetSavedPoint(90,134.4531);
   myFit1->SetSavedPoint(91,130.3479);
   myFit1->SetSavedPoint(92,126.3385);
   myFit1->SetSavedPoint(93,122.4215);
   myFit1->SetSavedPoint(94,118.5939);
   myFit1->SetSavedPoint(95,114.8525);
   myFit1->SetSavedPoint(96,111.1946);
   myFit1->SetSavedPoint(97,107.6174);
   myFit1->SetSavedPoint(98,104.1185);
   myFit1->SetSavedPoint(99,100.6952);
   myFit1->SetSavedPoint(100,0);
   myFit1->SetSavedPoint(101,60);
   myFit1->SetSavedPoint(102,145);
   myFit1->SetFillColor(19);
   myFit1->SetFillStyle(0);
   myFit1->SetLineColor(2);
   myFit1->SetLineWidth(2);
   myFit1->SetChisquare(0.8430339);
   myFit1->SetNDF(15);
   myFit1->GetXaxis()->SetLabelFont(42);
   myFit1->GetXaxis()->SetLabelSize(0.035);
   myFit1->GetXaxis()->SetTitleSize(0.035);
   myFit1->GetXaxis()->SetTitleFont(42);
   myFit1->GetYaxis()->SetLabelFont(42);
   myFit1->GetYaxis()->SetLabelSize(0.035);
   myFit1->GetYaxis()->SetTitleSize(0.035);
   myFit1->GetYaxis()->SetTitleFont(42);
   myFit1->SetParameter(0,0.00171409);
   myFit1->SetParError(0,0.003484349);
   myFit1->SetParLimits(0,0,0);
   myFit1->SetParameter(1,4900.561);
   myFit1->SetParError(1,1017.924);
   myFit1->SetParLimits(1,0,0);
   myFit1->SetParameter(2,710909);
   myFit1->SetParError(2,588102.8);
   myFit1->SetParLimits(2,0,0);
   myFit1->SetParameter(3,-4327.248);
   myFit1->SetParError(3,3728.886);
   myFit1->SetParLimits(3,0,0);
   myFit1->SetLineColor(kBlue+1);
   myFit1->Draw("Same");


  //f1->Draw("FC"); f2->Draw("FCsame");
  //neutralBkg->Draw("9p1e0same");
  //gammasBkg->Draw("9p1e0same");
  electronsBkg->Draw("9p1e0same");
  neutronsBkg->Draw("9p1e0same");
  totalBkg->Draw("9p1e0same");

  TLegend *leg2 = new TLegend(0.60,0.60,0.8,0.85);
  leg2->SetFillColor(0);
  leg2->SetHeader("ME0 Neutron Bkg Param and Hits");
  leg2->AddEntry("myFit","neutral Bkg Function","l");
  leg2->AddEntry("myFit1","charged Bkg Function","l");
  //leg2->AddEntry(neutralBkg,"n/#gamma Bkg","pl");
  //leg2->AddEntry(gammasBkg,"#gamma Bkg","pl");
  leg2->AddEntry(totalBkg,"total Bkg","pl");
  leg2->AddEntry(electronsBkg,"e^{#pm} Bkg","pl");
  leg2->AddEntry(neutronsBkg,"n Bkg","pl");
  leg2->Draw();
  c_all->Update();


}

void savePlot(TCanvas * c, TString name) {
  // c->SaveAs(name+".root");
  c->SaveAs(name+".png");
  c->SaveAs(name+".eps");
  gSystem->Exec("epstopdf "+name+".eps");
  std::cout<<"Plots saved"<<std::endl;
}



void prepareHisto (TH1F* hist, Int_t rebin, Double_t weight, Color_t color){
  //std::cout<<" BinWidth : "<<hist->GetBinWidth(2)<<std::endl;
  //Double_t bottomLenght = 21.9762;
  //Double_t topLenght = 52.7218;
  //Double_t minBase = 0.;
  //Double_t maxBase = 0.;


  //hist->Rebin(rebin);
  //std::cout<<" BinWidth dopo Rebin: "<<hist->GetBinContent(30)<<std::endl;
  //hist->Scale(1/weight);
  hist->SetStats(kFALSE);
  hist->SetLineColor(color);
  hist->SetMarkerColor(color);
  hist->SetMarkerStyle(kFullTriangleDown);
  hist->SetMarkerSize(0.7);
  //hist->GetXaxis()->SetRangeUser(65,150);
  std::cout << " Histogram :  "<<hist->GetName() << " -------------  ";
  for (Int_t bin = 1; bin<=hist->GetNbinsX();++bin){
    //  if (bin == 1) minBase = bottomLenght;
    //  else minBase = maxBase;
    //  if (bin==hist->GetNbinsX()) maxBase = topLenght;

    Double_t R_min = hist->GetBinCenter(bin) - 0.5 * hist->GetBinWidth(bin);
    Double_t R_max = hist->GetBinCenter(bin) + 0.5 * hist->GetBinWidth(bin);
    //  Double_t DeltaR = R_max - R_min;
    //  maxBase = 2*DeltaR*0.1763269 + minBase ;

    //  Double_t trapArea = (0.5*DeltaR*(minBase + maxBase))*18*6*2; //18*6*2 fattori geometrici per tenere conto dei 6 layers, 18 camere e le 2 regioni

    Double_t Area     = (TMath::Pi() * ( TMath::Power(R_max,2) - TMath::Power(R_min,2) ))*12;
    std::cout << "\n #bin = "<<bin
      //  <<"\n\tArea = "<<Area<<"  Area Trap= "<<trapArea<<"  Weight = "<< Area*weight
	      <<"\n\tBinContent = "<<hist->GetBinContent(bin)<<"  SetBinContent = "<<(hist->GetBinContent(bin))/(Area*weight)<<std::endl;
    hist->SetBinContent(bin,(hist->GetBinContent(bin))/(1.5*Area*weight));
    //hist->SetBinContent(bin,(hist->GetBinContent(bin))/(weight));
  }

}
