//#ifndef ME0DigisValidation_H
//#define ME0DigisValidation_H

#include "Validation/MuonME0Validation/interface/ME0BaseValidation.h"

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//Data Format
//#include "DataFormats/GEMDigi/interface/ME0DigiCollection.h"
#include "DataFormats/GEMDigi/interface/ME0DigiPreReco.h"
#include "DataFormats/GEMDigi/interface/ME0DigiPreRecoCollection.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartition.h"
#include "Geometry/GEMGeometry/interface/GEMEtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"

//Log messages
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
using namespace std;

struct MyME0SimHit
{
    Int_t eventNumber;
    Int_t detUnitId, particleType;
    Float_t x, y, energyLoss, pabs, timeOfFlight;
    Int_t region, ring, station, layer, chamber, roll;
    Float_t globalR, globalEta, globalPhi, globalX, globalY, globalZ;
    Int_t strip;
    Float_t Phi_0, DeltaPhi, R_0;
    Int_t countMatching;
};

struct MyME0Digi
{
    Int_t detId, particleType;
    Short_t region, ring, station, layer, chamber, roll;
    Short_t strip, bx;
    Float_t x, y;
    Float_t g_r, g_eta, g_phi, g_x, g_y, g_z;
    Float_t x_sim, y_sim;
    Float_t g_eta_sim, g_phi_sim, g_x_sim, g_y_sim, g_z_sim;
};

struct MySimTrack
{
    Float_t pt, eta, phi;
    Char_t charge;
    Char_t endcap;
    Char_t gem_sh_layer1, gem_sh_layer2;
    Char_t gem_dg_layer1, gem_dg_layer2;
    Char_t gem_pad_layer1, gem_pad_layer2;
    Float_t gem_sh_eta, gem_sh_phi;
    Float_t gem_sh_x, gem_sh_y;
    Float_t gem_dg_eta, gem_dg_phi;
    Float_t gem_pad_eta, gem_pad_phi;
    Float_t gem_lx_even, gem_ly_even;
    Float_t gem_lx_odd, gem_ly_odd;
    Char_t  has_gem_sh_l1, has_gem_sh_l2;
Char_t  has_gem_dg_l1, has_gem_dg_l2;
    Char_t  has_gem_pad_l1, has_gem_pad_l2;
    Float_t gem_trk_eta, gem_trk_phi, gem_trk_rho;
};


class ME0DigisValidation : public edm::EDAnalyzer {
   public:
      explicit ME0DigisValidation(const edm::ParameterSet&);
      ~ME0DigisValidation();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      bool isSimTrackGood(const SimTrack &);
      bool isME0DigiMatched(MyME0Digi me0_dg, MyME0SimHit me0_sh);

      virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      edm::EDGetToken simTrackInput_;
      edm::EDGetToken simTrackInput1_;
      edm::EDGetToken me0SimHitInput_;
      edm::EDGetToken me0DigiInput_;

    bool debug_;
    std::string folderPath_;
    bool EffSaveRootFile_;
    std::string EffRootFileName_;

    DQMStore * dbe;
    std::map<std::string, MonitorElement*> meCollection;

    //edm::Handle<edm::PSimHitContainer> ME0Hits;
    //edm::Handle<ME0DigiPreRecoCollection> me0_digis;
    //edm::Handle<edm::SimTrackContainer> sim_tracks;
    //edm::Handle<edm::SimVertexContainer> sim_vertices;
    edm::ESHandle<ME0Geometry> me0_geom;

    edm::ParameterSet cfg_;

    //edm::InputTag me0SimHitInput_;
    //edm::InputTag me0DigiInput_;
    //edm::InputTag simTrackInput_;

    double simTrackMinPt_;
    double simTrackMaxPt_;
    double simTrackMinEta_;
    double simTrackMaxEta_;
    double simTrackOnlyMuon_;

    const ME0Geometry* me0_geometry_;

    MyME0Digi me0_digi_;
    MyME0SimHit me0_sh;
    MySimTrack track_;

    bool hasME0Geometry_;

};

