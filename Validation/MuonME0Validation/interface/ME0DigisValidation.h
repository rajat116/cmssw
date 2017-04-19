#ifndef ME0DigisValidation_H
#define ME0DigisValidation_H

#include "Validation/MuonME0Validation/interface/ME0BaseValidation.h"
//Data Formats
#include "DataFormats/GEMDigi/interface/ME0DigiPreReco.h"
#include "DataFormats/GEMDigi/interface/ME0DigiPreRecoCollection.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//Log messages
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

using namespace std;
using namespace edm;
class DQMStore;
class MonitorElement;

class ME0DigisValidation : public ME0BaseValidation
{
public:
    explicit ME0DigisValidation( const edm::ParameterSet& );
    ~ME0DigisValidation();
    void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
    virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    void analyze(const edm::Event& e, const edm::EventSetup&) override;
    bool isMatched(const int, const int, const int, const int, const int, const int);
private:
    
    MonitorElement* num_evts;
    MonitorElement* me0_strip_dg_xy[2][6];
    MonitorElement* me0_strip_dg_xy_Muon[2][6];
    MonitorElement* me0_strip_dg_zr[2][6];
    MonitorElement* me0_strip_dg_zr_tot[2];
    MonitorElement* me0_strip_dg_zr_tot_Muon[2];
    
    MonitorElement* me0_strip_dg_dx_local_Muon[2][6];
    MonitorElement* me0_strip_dg_dy_local_Muon[2][6];
    MonitorElement* me0_strip_dg_dphi_global_Muon[2][6];
    
    MonitorElement* me0_strip_dg_dx_local_tot_Muon;
    MonitorElement* me0_strip_dg_dy_local_tot_Muon;
    MonitorElement* me0_strip_dg_x_local_tot;
    MonitorElement* me0_strip_dg_y_local_tot;
    MonitorElement* me0_strip_dg_dphi_global_tot_Muon;
    MonitorElement* me0_strip_dg_dphi_vs_phi_global_tot_Muon;
    MonitorElement* me0_strip_dg_dtime_tot_Muon;
    MonitorElement* me0_strip_dg_time_tot;
    
    MonitorElement* me0_strip_dg_den_eta[2][6];
    MonitorElement* me0_strip_dg_num_eta[2][6];
    
    MonitorElement* me0_strip_dg_den_eta_tot;
    MonitorElement* me0_strip_dg_num_eta_tot;
    
    MonitorElement* me0_strip_dg_bkg_rad_tot;
    MonitorElement* me0_strip_dg_bkgElePos_rad;
    MonitorElement* me0_strip_dg_bkgNeutral_rad;
    
    edm::EDGetToken InputTagToken_Digi;
    bool EffSaveRootFile_;
    std::string EffRootFileName_;
    double sigma_x_, sigma_y_;
     DQMStore * dbe_;
    int npart;
};

#endif
