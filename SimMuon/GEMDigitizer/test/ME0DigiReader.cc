/** \class ME0DigiReader
 *
 *  Dumps ME0 digis 
 *  
 *  \authors: Roumyana Hadjiiska
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/GEMDigi/interface/ME0DigiCollection.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
#include <map>
#include <set>

#include "DataFormats/Common/interface/DetSet.h"
#include <iostream>
/*start MM*/
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1F.h"
#include "TH2F.h"
/*end MM*/
using namespace std;


class ME0DigiReader: public edm::EDAnalyzer
{
public:

  explicit ME0DigiReader(const edm::ParameterSet& pset);
  
  virtual ~ME0DigiReader(){}
  
  void analyze(const edm::Event &, const edm::EventSetup&); 
  
private:

  edm::EDGetTokenT<edm::PSimHitContainer> simhitToken_;
  edm::EDGetTokenT<ME0DigiCollection> me0DigiToken_;
  edm::EDGetTokenT<edm::DetSetVector<StripDigiSimLink> > me0StripDigiSimLinkToken_;
  bool debug_; 

  TH1F *hProces; 
  //TH1F *hParticleTypes;
  //  TH1F *hAllSimHitsType;
  TH1F *hNstripEtaPart1;
  TH1F *hNstripEtaPart2; 
  TH1F *hNstripEtaPart3; 
  TH1F *hNstripEtaPart4; 
  TH1F *hNstripEtaPart5; 
  TH1F *hNstripEtaPart6; 
  TH1F *hNstripEtaPart7; 
  TH1F *hNstripEtaPart8; 
  TH1F *hBx; 
  TH2F *hRadiusEtaPartVsNdigi;
  TH2F *hRadiusEtaPartVsNdigiOvTrArea;
  TH1F *hRadiusEtaPart;

  int numbEvents;

};



ME0DigiReader::ME0DigiReader(const edm::ParameterSet& pset) :
  simhitToken_(consumes<edm::PSimHitContainer>(pset.getParameter<edm::InputTag>("simhitToken"))),
  me0DigiToken_(consumes<ME0DigiCollection>(pset.getParameter<edm::InputTag>("me0DigiToken"))),
  me0StripDigiSimLinkToken_(consumes<edm::DetSetVector<StripDigiSimLink> >(pset.getParameter<edm::InputTag>("me0StripDigiSimLinkToken")))
  , debug_(pset.getParameter<bool>("debugFlag"))
{
  edm::Service < TFileService > fs; 

  hProces = fs->make < TH1F > ("hProces", "Process type for all the simHits", 20, 0, 20);
  //  hAllSimHitsType = fs->make < TH1F > ("hAllSimHitsType", "pdgId for All simHits", 500, 0, 500);
  // hParticleTypes = fs->make < TH1F > ("hParticleTypes", "pdgId for digitized simHits", 500, 0, 500);
  hNstripEtaPart1 = fs->make <TH1F> ("NstripEtaPart1", "Nstrips in EtaPartition 1", 770, 1, 770);
  hNstripEtaPart2 = fs->make <TH1F> ("NstripEtaPart2", "Nstrips in EtaPartition 2", 770, 1, 770); 
  hNstripEtaPart3 = fs->make <TH1F> ("NstripEtaPart3", "Nstrips in EtaPartition 3", 770, 1, 770); 
  hNstripEtaPart4 = fs->make <TH1F> ("NstripEtaPart4", "Nstrips in EtaPartition 4", 770, 1, 770); 
  hNstripEtaPart5 = fs->make <TH1F> ("NstripEtaPart5", "Nstrips in EtaPartition 5", 770, 1, 770); 
  hNstripEtaPart6 = fs->make <TH1F> ("NstripEtaPart6", "Nstrips in EtaPartition 6", 770, 1, 770); 
  hNstripEtaPart7 = fs->make <TH1F> ("NstripEtaPart7", "Nstrips in EtaPartition 7", 770, 1, 770); 
  hNstripEtaPart8 = fs->make <TH1F> ("NstripEtaPart8", "Nstrips in EtaPartition 8", 770, 1, 770); 
  hBx = fs->make <TH1F> ("hBx", "bx from digi - for all #eta partiotions", 9, -5.5, 3.5 );
  hRadiusEtaPartVsNdigi = fs->make <TH2F> ("hRadiusEtaPartVsNdigi", "Radius Eta Partition vs Ndigi", 2500, 0., 250., 200, 0., 20. ); 
  hRadiusEtaPartVsNdigiOvTrArea = fs->make <TH2F> ("hRadiusEtaPartVsNdigiOvTrArea", "Ndigi/TrArea vs Radius Eta Partition", 2500, 0., 250., 1000, 0., 0.1 );
  hRadiusEtaPart = fs->make <TH1F> ("hRadiusEtaPart", "Radius Eta Partition", 200, 0., 200. ); 
  numbEvents = 0;
}


void ME0DigiReader::analyze(const edm::Event & event, const edm::EventSetup& eventSetup)
{
  cout << "--- Run: " << event.id().run() << " Event: " << event.id().event() << endl;

  edm::ESHandle<ME0Geometry> pDD;
  eventSetup.get<MuonGeometryRecord>().get( pDD );

  edm::Handle<edm::PSimHitContainer> simHits; 
  event.getByToken(simhitToken_, simHits);    

  edm::Handle<ME0DigiCollection> digis;
  event.getByToken(me0DigiToken_, digis);
   
  edm::Handle< edm::DetSetVector<StripDigiSimLink> > thelinkDigis;
  event.getByToken(me0StripDigiSimLinkToken_, thelinkDigis);

  ME0DigiCollection::DigiRangeIterator detUnitIt;
  
  for (detUnitIt = digis->begin(); detUnitIt != digis->end(); ++detUnitIt)
  {
    const ME0DetId& id = (*detUnitIt).first;
    const ME0EtaPartition* roll = pDD->etaPartition(id);

    // ME0DetId print-out
    cout<<"--------------"<<endl;
    cout<<"id: "<<id.rawId()<<" etaPartition id.roll() = "<<id.roll()<<endl;
    //cout<<"pDD->etaPartition(detid)"<<roll.id.roll()<<endl;
    
    int ndigi = 0;
    double trArea(0.0);
    double trStripArea(0.0);

    const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*> (&(roll->topology())));//&(roll->topogy())));
    const float rollRadius = top_->radius();
    const float striplength(top_->stripLength());
    const int nstrips = roll->nstrips();
    trStripArea = (roll->pitch()) * striplength;
    trArea = trStripArea * nstrips;

    cout << " roll->nstrips()= "<< roll->nstrips() <<" roll.id() = "<< roll->id() << "\troll area = " << trArea << "\t strip area = " << trStripArea << endl;
    cout << " distance beam pipe to the roll's center = "<< rollRadius<<endl;

    // Loop over the digis of this DetUnit
    const ME0DigiCollection::Range& range = (*detUnitIt).second;
    for (ME0DigiCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt)
    {
      cout<<" digi "<<*digiIt<<endl;
      if(id.roll() == 1) hNstripEtaPart1->Fill(digiIt->strip());
      if(id.roll() == 2) hNstripEtaPart2->Fill(digiIt->strip());   
      if(id.roll() == 3) hNstripEtaPart3->Fill(digiIt->strip()); 
      if(id.roll() == 4) hNstripEtaPart4->Fill(digiIt->strip());    
      if(id.roll() == 5) hNstripEtaPart5->Fill(digiIt->strip());       
      if(id.roll() == 6) hNstripEtaPart6->Fill(digiIt->strip());
      if(id.roll() == 7) hNstripEtaPart7->Fill(digiIt->strip());     
      if(id.roll() == 8) hNstripEtaPart8->Fill(digiIt->strip());    

      //bx
      hBx->Fill(digiIt->bx()); 
      if (digiIt->bx()!= 0) ndigi++;

      cout << " digiIt strip=" << digiIt->strip()<<" digiIt BX = "<< digiIt->bx()<<endl;

      //     if(id.roll() == 1){
	//}
      if (digiIt->strip() < 1 || digiIt->strip() > roll->nstrips() )
      {
        cout <<" XXXXXXXXXXXXX Problemt with "<<id<<"  a digi has strip# = "<<digiIt->strip()<<endl;
      }
 
      for(const auto& simHit: *simHits)
      {
	//std::cout << "particle type\t" << simHit.particleType() << "\tprocess type\t" << simHit.processType() << std::endl;
	
	hProces->Fill(simHit.processType());
	
        ME0DetId rpcId(simHit.detUnitId());//rpc-> me0
	//cout<<" rpcId (simHit.detUnitId()) = "<< rpcId()<<endl;
        if (rpcId == id && abs(simHit.particleType()) == 13)
        {
          cout<<"entry: "<< simHit.entryPoint()<<endl
              <<"exit: "<< simHit.exitPoint()<<endl
              <<"TOF: "<< simHit.timeOfFlight()<<endl;
        }

      }//rumi: (end simhits only muons); either move the for loop outside the digi iterator or make a residuals simhit-digi
    }// for digis in roll

    hRadiusEtaPartVsNdigi->Fill(rollRadius, ndigi);
    hRadiusEtaPartVsNdigiOvTrArea->Fill(rollRadius,ndigi/trArea);
    hRadiusEtaPart->Fill(rollRadius);

  }// for eta partitions (rolls)

/*
  for (edm::DetSetVector<StripDigiSimLink>::const_iterator itlink = thelinkDigis->begin(); itlink != thelinkDigis->end(); itlink++)
  {
    for(edm::DetSet<StripDigiSimLink>::const_iterator link_iter=itlink->data.begin();link_iter != itlink->data.end();++link_iter)
    {
      int detid = itlink->detId();
      int ev = link_iter->eventId().event();
      float frac =  link_iter->fraction();
      int strip = link_iter->channel();
      int trkid = link_iter->SimTrackId();
      int bx = link_iter->eventId().bunchCrossing();
      cout<<"DetUnit: "<<ME0DetId(detid)<<"  Event ID: "<<ev<<"  trkId: "<<trkid<<"  Strip: "<<strip<<"  Bx: "<<bx<<"  frac: "<<frac<<endl;
    }
  }
*/
  numbEvents++;
  std::cout << "number of events = " << numbEvents << std::endl;
  cout<<"--------------"<<endl;
}

#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(ME0DigiReader);

