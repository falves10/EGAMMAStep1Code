//#include "atlas/CommonHead.h"
#include <TTreeFormula.h>

//#include "atlas/AtlasStyle.C"
//#include "atlas/AtlasStyle.h"
//#include "atlas/AtlasUtils.C"
//#include "atlas/AtlasUtils.h"
#include "TMath.h"
#include "TStopwatch.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TChain.h"
#include "TROOT.h"
#include "THStack.h"
#include "TLegend.h"
#include "TF1.h"
#include "TLorentzVector.h"

#include "TMath.h"
#include "TStopwatch.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TChain.h"
#include "TROOT.h"
#include "THStack.h"
#include "TLegend.h"
#include "TF1.h"
#include "TSystem.h"

#include <math.h> // for “fabs”

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>


//RooFit include
#include "RooDataHist.h"
#include "RooFit.h"
#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "RooAddPdf.h"
#include "RooChebychev.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include <stdio.h>
#include <sys/stat.h>

#include <chrono>

using namespace RooFit ;
using namespace std;


int m_nEtaBin(13);
std::vector<double> m_etaBinning({0.0, 0.2, 0.4, 0.6, 0.8, 1., 1.2, 1.37, 1.52,
      1.8, 1.9, 2.1, 2.3, 2.47});

int m_nEtaBin_eOverp(13);
std::vector<double> m_etaBinning_eOverp({0.0, 0.2, 0.4, 0.6, 0.8, 1., 1.2, 1.37, 1.52,
      1.8, 1.9, 2.1, 2.3, 2.47}); //comment from meeting



int m_nEnergyBin(10);
std::vector<double> m_energyBinning({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.,2.0});
//int m_nEnergyBin(1);
//std::vector<double> m_energyBinning({0.,2.0});



bool calibrated(true);//Adele true

void PrintProgressBar( int index, int total )
{
  //  if( index%10000 == 0 )
  if( index%200000 == 0 )
    {
      TString print_bar = " [";
      for( int bar = 0; bar < 20; bar++ )
	{
	  double current_fraction = double(bar) / 20.0;
	  if( double(index)/double(total) > current_fraction )
	    print_bar.Append("/");
	  else
	    print_bar.Append(".");
	}
      print_bar.Append("] ");
      std::cout << print_bar << 100.*(double(index)/double(total)) << "%\r" << std::flush;
    }
}



void infile2chain(TString _infilelist, TChain *&_fchain, TString chainname)
{
  _fchain = new TChain(chainname);
  ifstream infile(_infilelist, ios::in);
  string line;
  while (getline(infile, line)){
    _fchain->Add(line.c_str());
  }
  infile.close();
}


void mySave(TH1F* map_histog_mc, TH1F* map_histog_data, int iBias, int iEtaBin, int iEnergyBin,TString vartagname, TString varname,TString path, TString mc);


vector<double> E1layer(vector<double> *vec);
vector<double> E2layer(vector<double> *vec);


/// Get the bin number of a given eta value
unsigned int binNbOfEta(double eta) 
{
  double localEta = eta;
  localEta = fabs(eta);
  unsigned int bin = 0;
  while (bin < m_nEtaBin && localEta > m_etaBinning[bin+1])
    bin++;
  return bin;
} // end binNbOfEta


/// Get the bin number of a given eta value eoverp
unsigned int binNbOfEta_eOverp(double eta) 
{
  double localEta = eta;
  localEta = fabs(eta);
  unsigned int bin = 0;
  while (bin < m_nEtaBin_eOverp && localEta > m_etaBinning_eOverp[bin+1])
    bin++;
  return bin;
} // end binNbOfEta eOverp


/// Get the bin number of a given E1/E2 value
unsigned int binNbOfEnergy(double E1E2) 
{
  
  double localEnergy = E1E2;
  localEnergy = fabs(E1E2);
  unsigned int bin = 0;
  while (bin < m_nEnergyBin && localEnergy > m_energyBinning[bin+1])
    bin++;
  return bin;
} // end binNbOfE1E2




//MAIN
//int egamma_test_condor(string geoName){ 
int egamma_test_condor(TString mc, string geoName){ 
 
  
  cout<< Form("/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/%s/",geoName.c_str() ) << endl;
  //TString path = "/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/";
  TString path = Form("/publicfs/atlas/atlasnew/higgs/hgg/fabiolucio/EgammCalibration/Codes_ntuples/%s/",geoName.c_str() );
  
    
   
  vector<TString> varname;
  varname.push_back("m_mass2el");
  //varname.push_back("m_eOverp");
  //varname.push_back("m_E1_E2");
  
  vector<TString> vartagname;
  vartagname.push_back("m_{ee} [GeV]");
  //vartagname.push_back("E/p");
  //vartagname.push_back("E1/E2");
    
  for(int ivar = 0; ivar < varname.size(); ivar++){

      
    
    map<int, TH1F*> map_histog;
    map<int, TH1F*> vhisto2D_data;
    map<int, TH1F*> vhisto2DMean_data;
    map<int, TH1F*> vhisto2DSigma_data;

    std::map<pair<int, int>, map<int,TH1F*>> m_histo; 

    int biasSize = 0;

    int binning; float lowedge; float highedge; float lowcut; float highcut;// mass
    if(varname[ivar].Contains("mass")) {binning = 50; lowedge = 80; highedge = 100; lowcut = 80; highcut = 100;}// mass
    if(varname[ivar].Contains("m_eOverp")) {binning = 50; lowedge = 0.6; highedge = 2; lowcut = 0.6; highcut = 2;}// E/p
    if(varname[ivar].Contains("el_E1")) {binning = 50; lowedge = 0; highedge = 200; lowcut = 0; highcut = 200;}// E1
    if(varname[ivar].Contains("el_E2")) {binning = 50; lowedge = 0; highedge = 200; lowcut = 0; highcut = 200;}// E2
    if(varname[ivar].Contains("E1_E2")) {binning = 20; lowedge = 0; highedge = 2.; lowcut = 0; highcut = 2.;}// E1/E2
    
 
    unsigned int maxEta = 0;
    
    maxEta = m_nEtaBin;
    //maxEta 
    
    //booking histogram
      
    for (unsigned int iEtaBin = 0; iEtaBin < maxEta; iEtaBin++)
    {      
      for (unsigned int iEnergyBin = 0; iEnergyBin < m_nEnergyBin; iEnergyBin++) 
      {
	       m_histo[std::make_pair(iEtaBin,iEnergyBin)][0] = new TH1F(Form("Data_histo_binEta%d_binEnergy%d_%s",iEtaBin,iEnergyBin,varname[ivar].Data()), "",    binning,  lowedge, highedge);
	       m_histo[std::make_pair(iEtaBin,iEnergyBin)] [1]= new TH1F(Form("MC_histo_binEta%d_binEnergy%d_%s",iEtaBin,iEnergyBin,varname[ivar].Data()), "",    binning,  lowedge, highedge);
	   
            for (unsigned int i = 0; i < 16; i++)
            {
	           m_histo[std::make_pair(iEtaBin,iEnergyBin)][i+2] = new TH1F(Form("Data_histo_binEta%d_binEnergy%d_bias%d_%s",iEtaBin,iEnergyBin,i,varname[ivar].Data()), "",    binning,  lowedge, highedge);
            }
        }
    }   
    
    
    
    vector<TString> recolist;
    TString position;

    
    // for running on HT CONDOR
    
    if(mc.Contains("var")) 
    {
	   recolist.push_back( Form("./%s.list",mc.Data()) ); //data/MC with bias
       //cout<< " teste sample " <<  Form("./%s.list",mc.Data())  << endl;
    }
    if(mc.Contains("nom")) 
    {
	   recolist.push_back("var");
	   recolist.push_back( Form("./%s.list",mc.Data())); //MCnominal
    }
    
    map<int, TChain*> chain;          
    
    for(int iperiod = 0 ; iperiod <recolist.size() ; iperiod ++)
    {
      infile2chain(recolist[iperiod], chain[iperiod], "CollectionTree");
    }
    
        
    int evtsMC = 0;
    int evtsData = 0;     

    int count_data = 0;	
    int count_mc = 0;	  
    
      
    for(int iperiod = 0 ; iperiod <recolist.size() ; iperiod ++){
      
      chain[iperiod]->SetBranchStatus("*",0);
      
      double pt1, eta1, el1_ccl_E1, el1_ccl_E2, m12,el_zver, el1_rawcl_E1, el1_rawcl_E2, phi1, el1_ptTrack, energy1,eta2,pt2,phi2,calenergy1,calenergy2;
      float weight_pileup, mc_weight;
      double weight_norm, weight, lumi;
      int RunNumber;
      bool isMC = false;
      vector<double> *el1_energy = 0;
      vector<double> *el2_energy = 0;
      vector<double> *el1_energy_sysPS = 0;
      vector<double> *el2_energy_sysPS = 0;

    
        
      chain[iperiod]->SetBranchStatus("eta1", 1);
      chain[iperiod]->SetBranchAddress("eta1", &eta1);
      chain[iperiod]->SetBranchStatus("eta2", 1);
      chain[iperiod]->SetBranchAddress("eta2", &eta2);
      chain[iperiod]->SetBranchStatus("m12", 1);
      chain[iperiod]->SetBranchAddress("m12", &m12);
      chain[iperiod]->SetBranchStatus("RunNumber", 1);
      chain[iperiod]->SetBranchAddress("RunNumber", &RunNumber);
      chain[iperiod]->SetBranchStatus("lumi", 1);
      chain[iperiod]->SetBranchAddress("lumi", &lumi);
      chain[iperiod]->SetBranchStatus("pt1", 1);
      chain[iperiod]->SetBranchAddress("pt1", &pt1);
      chain[iperiod]->SetBranchStatus("pt2", 1);
      chain[iperiod]->SetBranchAddress("pt2", &pt2);
      chain[iperiod]->SetBranchStatus("el1_ptTrack",1);
      chain[iperiod]->SetBranchAddress("el1_ptTrack", &el1_ptTrack);
      chain[iperiod]->SetBranchStatus("energy1",1);
      chain[iperiod]->SetBranchAddress("energy1", &energy1);
      chain[iperiod]->SetBranchStatus("phi1", 1);
      chain[iperiod]->SetBranchAddress("phi1", &phi1);
      chain[iperiod]->SetBranchStatus("phi2", 1);
      chain[iperiod]->SetBranchAddress("phi2", &phi2);
      chain[iperiod]->SetBranchAddress("calenergy1", &calenergy1);
      chain[iperiod]->SetBranchAddress("calenergy2", &calenergy2);
     
      
        
      if(iperiod == 0){
            //chain[iperiod]->SetBranchStatus("el1_ccl_E1", 1);
            //chain[iperiod]->SetBranchAddress("el1_ccl_E1", &el1_ccl_E1);
            //chain[iperiod]->SetBranchStatus("el1_ccl_E2", 1);
            //chain[iperiod]->SetBranchAddress("el1_ccl_E2", &el1_ccl_E2);
            chain[iperiod]->SetBranchStatus("el1_rawcl_E1", 1);
            chain[iperiod]->SetBranchAddress("el1_rawcl_E1", &el1_rawcl_E1);
            chain[iperiod]->SetBranchStatus("el1_rawcl_E2", 1);
            chain[iperiod]->SetBranchAddress("el1_rawcl_E2", &el1_rawcl_E2);
            chain[iperiod]->SetBranchStatus("el1_energy", 1);
            chain[iperiod]->SetBranchAddress("el1_energy", &el1_energy);
            chain[iperiod]->SetBranchStatus("el2_energy", 1);
            chain[iperiod]->SetBranchAddress("el2_energy", &el2_energy);
          
            chain[iperiod]->SetBranchStatus("el1_energy_sysPS", 1);
            chain[iperiod]->SetBranchAddress("el1_energy_sysPS", &el1_energy_sysPS);
            chain[iperiod]->SetBranchStatus("el2_energy_sysPS", 1);
            chain[iperiod]->SetBranchAddress("el2_energy_sysPS", &el2_energy_sysPS);
          
            chain[iperiod]->SetBranchStatus("weight", 1);
            chain[iperiod]->SetBranchAddress("weight", &weight);
            chain[iperiod]->SetBranchStatus("weight_pileup", 1);
            chain[iperiod]->SetBranchAddress("weight_pileup", &weight_pileup);
            chain[iperiod]->SetBranchStatus("mc_weight", 1);
            chain[iperiod]->SetBranchAddress("mc_weight", &mc_weight);
            chain[iperiod]->SetBranchStatus("weight_norm", 1);
            chain[iperiod]->SetBranchAddress("weight_norm", &weight_norm);
      }
      
      
      
      if(iperiod == 1){
            //	cout<<" period 1 " << endl;
            chain[iperiod]->SetBranchStatus("el1_rawcl_E1", 1);
            chain[iperiod]->SetBranchAddress("el1_rawcl_E1", &el1_rawcl_E1);
            chain[iperiod]->SetBranchStatus("el1_rawcl_E2", 1);
            chain[iperiod]->SetBranchAddress("el1_rawcl_E2", &el1_rawcl_E2);
            chain[iperiod]->SetBranchStatus("weight", 1);
            chain[iperiod]->SetBranchAddress("weight", &weight);
            chain[iperiod]->SetBranchStatus("weight_pileup", 1);
            chain[iperiod]->SetBranchAddress("weight_pileup", &weight_pileup);
            chain[iperiod]->SetBranchStatus("mc_weight", 1);
            chain[iperiod]->SetBranchAddress("mc_weight", &mc_weight);
            chain[iperiod]->SetBranchStatus("weight_norm", 1);
            chain[iperiod]->SetBranchAddress("weight_norm", &weight_norm);
      }
      
      
      int ntotevtR = chain[iperiod]->GetEntries();
      //int ntotevtR = 5;
      cout<<"total event "<<ntotevtR<< " variable " << varname[ivar] << endl;
      cout<<" reco size "<< recolist.size() << endl;
	
      if(iperiod == 0) std::cout<<"chain[iperiod]->GetEntries() = 0 "<<chain[iperiod]->GetEntries()<<std::endl;
      if(iperiod == 1) std::cout<<"chain[iperiod]->GetEntries() = 1 "<<chain[iperiod]->GetEntries()<<std::endl;
    
        
      for (int j=0; j<ntotevtR;j++)
      {
            PrintProgressBar(j, ntotevtR);
            chain[iperiod]->GetEntry(j);

             
            float weight = 1.;
            if(iperiod==1)
            {
              if(RunNumber<290000)  weight = lumi*weight_norm*weight;//weight_norm contains sum_weight
              else if(RunNumber<310000)  weight = lumi*weight_norm*weight;//weight_norm contains sum_weight
              else  weight = lumi*weight_norm*weight;//weight_norm contains sum_weight
	        }
            else if (iperiod==0)
            {
                if(RunNumber<290000)  weight = lumi*weight_norm*weight;//weight_norm contains sum_weight
                else if(RunNumber<310000)  weight = lumi*weight_norm*weight;//weight_norm contains sum_weight
                else  weight = lumi*weight_norm*weight;//weight_norm contains sum_weight
            }
          
            
     
             
	if(m12/1000.<80.) continue;
	if(m12/1000.>105.) continue;

	if(pt1<0.) continue;
	if(pt2<0.) continue;
	
	float weight_data = 1.;
	float target_var = 0.; // mass	
	
	if(varname[ivar]=="m_mass2el") target_var = m12/1000.; // mass
	if(std::isnan(target_var)) continue;
          
    
	
      
	if(iperiod==0){	 
	  if(calibrated){
          
	    if(el1_rawcl_E1/el1_rawcl_E2>2.) continue;
	    if(el1_rawcl_E1/el1_rawcl_E2<0.) continue;
	    if(varname[ivar]=="m_el_E1") target_var = el1_rawcl_E1/1000.; // E1	   
	    if(varname[ivar]=="m_el_E2") target_var = el1_rawcl_E2/1000.; // E2	 
	    if(varname[ivar]=="m_E1_E2")
        {
	      target_var = el1_rawcl_E1/el1_rawcl_E2; // ratio
	      m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][0]->Fill(target_var,weight);
	    }
	    
        
	  
          
	    if(varname[ivar]=="m_mass2el"){
	      target_var = m12/1000.; // mass
	      //Fill DATA histos
	   
          m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][0]->Fill(target_var,weight);
	     
            //For using electron energies from layer1:
           vector<double> el_mee1 = E1layer(el1_energy);
           vector<double> el_mee2 = E1layer(el2_energy);
          //For using electron energies from layer2:
           //vector<double> el_mee1 = E2layer(el1_energy);
           //vector<double> el_mee2 = E2layer(el2_energy);
            
            
           for(int i = 0; i < el_mee1.size(); i++)
           {
                TLorentzVector P1;
                TLorentzVector P2;
                TLorentzVector Psum;
                double m_electron_mass = 0.5; //MeV
                double momentum_prime1 = std::sqrt(pow(el_mee1[i],2) - pow(m_electron_mass,2));
                double pz_prime1 = std::tanh(eta1)*momentum_prime1;
                double pt_prime1 = pz_prime1/(std::sinh(eta1));
                double momentum_prime2 = std::sqrt(pow(el_mee2[i],2) - pow(m_electron_mass,2));
                double pz_prime2 = std::tanh(eta2)*momentum_prime2;
                double pt_prime2 = pz_prime2/(std::sinh(eta2));
                P1.SetPtEtaPhiE(pt_prime1/1000.,eta1,phi1,el_mee1[i]/1000.);
                P2.SetPtEtaPhiE(pt_prime2/1000.,eta2,phi2,el_mee2[i]/1000.);
                Psum = P1+P2;
                target_var = Psum.M();
                //Fill DATA histos
                //m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_ccl_E1/el1_ccl_E2))][i+2]->Fill(target_var,weight_data);
                m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][i+2]->Fill(target_var,weight);
              }
	    }
        
          
        
	    if(varname[ivar]=="m_eOverp"){
	      //if(fabs(eta1) > 1.35) continue;
          
	      target_var = calenergy1/cosh(eta1)/el1_ptTrack;
          //cout<<"period 0 calenergy1 " << calenergy1 << endl;
	      
	      //Fill DATA histos
          //cout<<" Nominal element in variation ntuple calenergy1: (period0) " << calenergy1 << endl;
	      //m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][0]->Fill(target_var,weight);//MC
          m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][0]->Fill(target_var,weight_data);
          
          //vector<double> elLayer1 = E1layer(el1_energy);
          //if using el_layer2, uncomment the below line
          vector<double> elLayer2 = E2layer(el1_energy);
        
            
          for(int i = 0; i < elLayer2.size(); i++){ 
              
              //cout<<" Variations elLayer1[i]: " << elLayer1[i] << endl;
              target_var = elLayer2[i]/cosh(eta1)/el1_ptTrack;
              //m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][i+2]->Fill(target_var,weight);//MC weight
              m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][i+2]->Fill(target_var,weight_data);//MC weight
	      }
              
            
	    }
         
	  }
	  else
      {
	    if(el1_rawcl_E1/el1_rawcl_E2>2) continue;
	    if(el1_rawcl_E1/el1_rawcl_E2<0) continue;
	    
	    if(varname[ivar]=="m_el_E1") target_var = el1_rawcl_E1/1000.; // E1	   
	    if(varname[ivar]=="m_el_E2") target_var = el1_rawcl_E2/1000.; // E2	 
	    if(varname[ivar]=="m_E1_E2") target_var = el1_rawcl_E1/el1_rawcl_E2; // ratio
	    //Fill DATA histos
	    if(varname[ivar]=="m_mass2el") target_var = m12/1000.; // mass
	    m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_ccl_E1/el1_ccl_E2))][0]->Fill(target_var,weight_data);
	  }
	}
	//Fill MC histos
	
	if(iperiod==1){
	  if(varname[ivar]=="m_el_E1") target_var = el1_rawcl_E1/1000.; // E1	   
	  if(varname[ivar]=="m_el_E2") target_var = el1_rawcl_E2/1000.; // E2	 
	  if(varname[ivar]=="m_E1_E2") target_var = el1_rawcl_E1/el1_rawcl_E2; // ratio
	  
        
	  if(el1_rawcl_E1/el1_rawcl_E2>2) continue;
	  if(el1_rawcl_E1/el1_rawcl_E2<0) continue;
	  
	  if(varname[ivar]=="m_eOverp")
      {
        //cout<<" Nominal element in variation ntuple calenergy1: (period1) " << calenergy1 << endl;
	    target_var = calenergy1/cosh(eta1)/el1_ptTrack;
        //cout<<"target_var "<< target_var << endl;
	    //Fill DATA histos
	    m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][1]->Fill(target_var,weight);
	  }
	  
      if(varname[ivar]=="m_mass2el")
      {
          target_var = m12/1000.; // mass
          m_histo[std::make_pair(binNbOfEta(eta1),binNbOfEnergy(el1_rawcl_E1/el1_rawcl_E2))][1]->Fill(target_var,weight);
    }	 
      
	  
	}
         
          
      }//end loop entries
      
    }//end loop period

    for (unsigned int iEtaBin = 0; iEtaBin < maxEta; iEtaBin++) 
    {
        //if(iEtaBin > 1) continue;
        for (unsigned int iEnergyBin = 0; iEnergyBin < m_nEnergyBin; iEnergyBin++) 
        {
            
            //if(iEnergyBin > 1) continue;
            
            map_histog[1] = new TH1F(Form("data_%d_%d",iEtaBin,iEnergyBin), "",    binning,  lowedge, highedge);
            map_histog[0] = new TH1F(Form("mc_%d_%d",iEtaBin,iEnergyBin), "", binning,  lowedge, highedge);
            
            map_histog[0] = (TH1F*) m_histo[std::make_pair(iEtaBin,iEnergyBin)][1]->Clone(Form("mc_%d_%d",iEtaBin,iEnergyBin));
            map_histog[1] = (TH1F*) m_histo[std::make_pair(iEtaBin,iEnergyBin)][0]->Clone(Form("data_%d_%d",iEtaBin,iEnergyBin));
            
            //map_histog[1]->Sumw2();
            //map_histog[0]->Sumw2();

            
            if(varname[ivar]=="m_E1_E2")
            {
	           mySave(map_histog[0],map_histog[1],-1,iEtaBin,iEnergyBin,vartagname[ivar],varname[ivar],path, mc);
	        }
            //bias part
	        
            else
            {
               
	  
	           if(varname[ivar]=="m_mass2el" || varname[ivar]=="m_eOverp") 
               {
                   
	               for(int i = 0; i < 16 ; i++)
                   {
                       
                        //Adele when using vector with 16 elements only
                        //for(int i = 0; i < biasSize-16 ; i++){ //when using vector with 32 elements only
                       
	                   map_histog[i+2] = new TH1F(Form("data_%d_%d_%d",iEtaBin,iEnergyBin,i+2), "", binning,  lowedge, highedge);
                       
	                   //map_histog[i+2]->Reset();
	                   map_histog[i+2] = (TH1F*) m_histo[std::make_pair(iEtaBin,iEnergyBin)][i+2]->Clone(Form("data_%d_%d_%d",iEtaBin,iEnergyBin,i+2));
                       
	                   map_histog[i+2]->Sumw2();
	               }
	    
                
	           }
                
              
	  
            mySave(map_histog[0],map_histog[1],-1,iEtaBin,iEnergyBin,vartagname[ivar],varname[ivar],path, mc);
	    
            for(int i = 0; i < 16 ; i++)
            {
	           mySave(map_histog[0],map_histog[i+2],i+2,iEtaBin,iEnergyBin,vartagname[ivar],varname[ivar],path,mc);
	        }
    
	       }
            
            
      
        }//end loop energy bins
      
    }//end loop variables
    
  
}
return 0;
}






//////////////////////////////////////////SAVING HISTOS////////////////////////////////////////

void mySave(TH1F* map_histog_mc, TH1F* map_histog_data, int iBias, int iEtaBin, int iEnergyBin,TString vartagname, TString varname,TString path, TString mc)
{	
    
  
 
    
  //cout<<"map_histog_mc "<< map_histog_mc->GetEntries() <<  " map_histog_data " << map_histog_data->GetEntries() << endl;     
    
  
  TString output_folder = path+Form("calibrated_%s",varname.Data());
  struct stat sb;
  if (stat(output_folder, &sb) == 0 && S_ISDIR(sb.st_mode)) {
    gSystem->cd(output_folder);    
  } 
  else {
    gSystem->mkdir(path+Form("calibrated_%s/",varname.Data()), kTRUE);
  }

  if (!(stat(output_folder+"/Histograms/layer1/", &sb) == 0 && S_ISDIR(sb.st_mode))) 
    gSystem->mkdir(output_folder+"/Histograms/layer1/", kTRUE);
  
  TFile *file; 
    
  if(iBias<2){
    //canvas->SaveAs(output_folder+Form("/Histograms/weight_graph_eta%i_energy%i_%s.pdf",iEtaBin, iEnergyBin, varname.Data()));
    //canvas->SaveAs(output_folder+Form("/Histograms/weight_graph_eta%i_energy%i_%s.root",iEtaBin, iEnergyBin, varname.Data()));
    
    file = new TFile(output_folder+Form("/Histograms/layer1/weight_histo_%s_eta%i_energy%i_%s.root", mc.Data(), iEtaBin, iEnergyBin, varname.Data()), "RECREATE"); 
      
    //temporary 
    if(varname=="m_mass2el" || varname=="m_eOverp")
    {
      map_histog_mc->SetName(Form("mc_eta_%d_energy_%d",iEtaBin, iEnergyBin));
      map_histog_mc->Write();
      map_histog_data->SetName(Form("data_eta_%d_energy_%d",iEtaBin, iEnergyBin));
      map_histog_data->Write();
    }
    
    file->Close();
      
  }
  else {
      
    //canvas->SaveAs(output_folder+Form("/Histograms/weight_graph_eta%i_energy%i_%s_bias%i.pdf",iEtaBin, iEnergyBin, varname.Data(),iBias-2));
    //canvas->SaveAs(output_folder+Form("/Histograms/weight_graph_eta%i_energy%i_%s_bias%i.root",iEtaBin, iEnergyBin, varname.Data(),iBias-2));
      
    file = new TFile(output_folder+Form("/Histograms/layer1/weight_histo_%s_eta%i_energy%i_bias%d_%s.root", mc.Data(), iEtaBin, iEnergyBin, iBias-2, varname.Data()), "RECREATE");
      
    if(varname=="m_mass2el" || varname=="m_eOverp")
    {
      map_histog_mc->SetName(Form("mc_eta_%d_energy_%d_bias%d",iEtaBin, iEnergyBin,iBias-2));
      map_histog_mc->Write();
      map_histog_data->SetName(Form("data_eta_%d_energy_%d_bias%d",iEtaBin, iEnergyBin,iBias-2 ));
      map_histog_data->Write();
    }
    file->Close();
    
  }
    
  

  //delete canvas;      
  return 0;
}

////////////////////////////////////////// SPLITTING VECTORS ////////////////////////////////////////
void print(std::vector<double> *v)
{
	for (auto it = v->begin(); it != v->end(); ++it)
		std::cout << *it << ' ';

	std::cout << '\n';
}

vector<double> E1layer(vector<double> *vec)
{

// fill
std::size_t const half_size = vec->size() / 2;
std::vector<double> split_lo(vec->begin(), vec->begin() + half_size);
    
return split_lo;
    
}


vector<double> E2layer(vector<double> *vec)
{

// fill
std::size_t const half_size = vec->size() / 2;
std::vector<double> split_hi(vec->begin() + half_size, vec->end());
    
return split_hi;
    
}
