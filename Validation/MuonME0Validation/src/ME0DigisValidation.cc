#include "Validation/MuonME0Validation/interface/ME0DigisValidation.h"
#include <TMath.h>

ME0DigisValidation::ME0DigisValidation(const edm::ParameterSet& iConfig):
simTrackInput_(consumes<edm::SimTrackContainer>(iConfig.getParameter<edm::InputTag>("input"))),
simTrackInput1_(consumes<edm::SimVertexContainer>(iConfig.getParameter<edm::InputTag>("input"))),
me0SimHitInput_(consumes<edm::PSimHitContainer>(iConfig.getParameter<edm::InputTag>("siminput"))),
me0DigiInput_(consumes<ME0DigiPreRecoCollection>(iConfig.getParameter<edm::InputTag>("digiinput"))),
debug_(iConfig.getUntrackedParameter<bool>("debug")),
folderPath_(iConfig.getUntrackedParameter<std::string>("folderPath")),
EffSaveRootFile_(iConfig.getUntrackedParameter<bool>("EffSaveRootFile")),
EffRootFileName_(iConfig.getUntrackedParameter<std::string>("EffRootFileName"))
{
   //now do what ever initialization is needed
    //hello
    //now do what ever initialization is needed
    dbe = edm::Service<DQMStore>().operator->();

    cfg_ = iConfig.getParameter<edm::ParameterSet>("simTrackMatching");
    auto simTrack = cfg_.getParameter<edm::ParameterSet>("simTrack");
    //simTrackInput_ = simTrack.getParameter<edm::InputTag>("input");
    simTrackMinPt_ = simTrack.getParameter<double>("minPt");
    simTrackMaxPt_ = simTrack.getParameter<double>("maxPt");
    simTrackMinEta_ = simTrack.getParameter<double>("minEta");
    simTrackMaxEta_ = simTrack.getParameter<double>("maxEta");
    simTrackOnlyMuon_ = simTrack.getParameter<bool>("onlyMuon");

    auto me0SimHit = cfg_.getParameter<edm::ParameterSet>("me0SimHit");
    //me0SimHitInput_ = me0SimHit.getParameter<edm::InputTag>("input");

    auto me0Digi = cfg_.getParameter<edm::ParameterSet>("me0StripDigi");
    //me0DigiInput_ = me0Digi.getParameter<edm::InputTag>("input");

    hasME0Geometry_=false;

}


ME0DigisValidation::~ME0DigisValidation()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ME0DigisValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    std::cout<<"hello1"<<std::endl;
    using namespace edm;
    //std::cout<<"hello1"<<std::endl;
    edm::Handle<edm::SimTrackContainer> sim_tracks;
    iEvent.getByToken(simTrackInput_, sim_tracks);
    edm::Handle<edm::SimVertexContainer> sim_vertices;
    iEvent.getByToken(simTrackInput1_, sim_vertices);
    edm::Handle<edm::PSimHitContainer> ME0Hits;
    iEvent.getByToken(me0SimHitInput_, ME0Hits);
    edm::Handle<ME0DigiPreRecoCollection> me0_digis;
    iEvent.getByToken(me0DigiInput_, me0_digis);

    std::vector<int> trackIds;
    std::vector<int> trackType;
    const edm::SimTrackContainer & sim_trks = *sim_tracks.product();

    meCollection["num_events"]->Fill(1);
    for (auto& t: sim_trks)
    {
        if (!isSimTrackGood(t)) continue;
        trackType.push_back(t.type());
        trackIds.push_back(t.trackId());
    }

    for (edm::PSimHitContainer::const_iterator itHit = ME0Hits->begin(); itHit != ME0Hits->end(); ++itHit)
    {
        if(abs(itHit->particleType()) != 13) continue;
        if(std::find(trackIds.begin(), trackIds.end(), itHit->trackId()) == trackIds.end()) continue;

        //std::cout<<"Size "<<trackIds.size()<<" id1 "<<trackIds[0]<<" type1 "<<trackType[0]<<" id2 "<<trackIds[1]<<" type2 "<<trackType[1]<<std::endl;

        me0_sh.eventNumber = iEvent.id().event();
        me0_sh.detUnitId = itHit->detUnitId();
        me0_sh.particleType = itHit->particleType();
        me0_sh.x = itHit->localPosition().x();
        me0_sh.y = itHit->localPosition().y();
        me0_sh.energyLoss = itHit->energyLoss();
        me0_sh.pabs = itHit->pabs();
        me0_sh.timeOfFlight = itHit->timeOfFlight();

        const ME0DetId id(itHit->detUnitId());

        me0_sh.region = id.region();
        me0_sh.ring = 0;
        me0_sh.station = 0;
        me0_sh.layer = id.layer();
        me0_sh.chamber = id.chamber();
        me0_sh.roll = id.roll();

        const LocalPoint p0(0., 0., 0.);
        const GlobalPoint Gp0(me0_geometry_->idToDet(itHit->detUnitId())->surface().toGlobal(p0));

        me0_sh.Phi_0 = Gp0.phi();
        me0_sh.R_0 = Gp0.perp();
        me0_sh.DeltaPhi = atan(-1*id.region()*pow(-1,id.chamber())*itHit->localPosition().x()/(Gp0.perp() + itHit->localPosition().y()));

        const LocalPoint hitLP(itHit->localPosition());
        const GlobalPoint hitGP(me0_geometry_->idToDet(itHit->detUnitId())->surface().toGlobal(hitLP));
        me0_sh.globalR = hitGP.perp();
        me0_sh.globalEta = hitGP.eta();
        me0_sh.globalPhi = hitGP.phi();
        me0_sh.globalX = hitGP.x();
        me0_sh.globalY = hitGP.y();
        me0_sh.globalZ = hitGP.z();

        //  Now filling strip info using entry point rather than local position to be
        //  consistent with digi strips. To change back, just switch the comments - WHF
        //  me0_sh.strip=gem_geometry_->etaPartition(itHit->detUnitId())->strip(hitLP);
        const LocalPoint hitEP(itHit->entryPoint());
        me0_sh.strip = me0_geometry_->etaPartition(itHit->detUnitId())->strip(hitEP);

        int count = 0;

        for(ME0DigiPreRecoCollection::DigiRangeIterator cItr = me0_digis->begin(); cItr != me0_digis->end(); ++cItr){

            ME0DetId id = (*cItr).first;

            const GeomDet* gdet = me0_geom->idToDet(id);
            const BoundPlane & surface = gdet->surface();

            me0_digi_.detId = id();
            me0_digi_.region = (Short_t) id.region();
            me0_digi_.ring = 0;
            me0_digi_.station = 0;
            me0_digi_.layer = (Short_t) id.layer();
            me0_digi_.chamber = (Short_t) id.chamber();
            me0_digi_.roll = (Short_t) id.roll();

            ME0DigiPreRecoCollection::const_iterator digiItr;
            //loop over digis of given roll
            for (digiItr = (*cItr ).second.first; digiItr != (*cItr ).second.second; ++digiItr)
            {
		if(abs(digiItr->pdgid()) != 13) continue;
		me0_digi_.particleType = digiItr->pdgid();
                me0_digi_.strip = 0;
                me0_digi_.bx = 0;

                me0_digi_.x = (Float_t) digiItr->x();
                me0_digi_.y = (Float_t) digiItr->y();

                LocalPoint lp(digiItr->x(), digiItr->y(), 0);

                GlobalPoint gp = surface.toGlobal(lp);
                me0_digi_.g_r = (Float_t) gp.perp();
                me0_digi_.g_eta = (Float_t) gp.eta();
                me0_digi_.g_phi = (Float_t) gp.phi();
                me0_digi_.g_x = (Float_t) gp.x();
                me0_digi_.g_y = (Float_t) gp.y();
                me0_digi_.g_z = (Float_t) gp.z();

                me0_digi_.x_sim = me0_sh.x;
                me0_digi_.y_sim = me0_sh.y;
                me0_digi_.g_eta_sim = me0_sh.globalEta;
                me0_digi_.g_phi_sim = me0_sh.globalPhi;
                me0_digi_.g_x_sim = me0_sh.globalX;
                me0_digi_.g_y_sim = me0_sh.globalY;
                me0_digi_.g_z_sim = me0_sh.globalZ;

                // abbreviations
                int re(me0_digi_.region);
                int la(me0_digi_.layer);

                if(me0_digi_.bx != 0) continue;
                if(isME0DigiMatched(me0_digi_, me0_sh)){
                    count++;

                    /*------------zR Occupancy--------------*/
                    const double glb_R(sqrt(me0_digi_.g_x*me0_digi_.g_x+me0_digi_.g_y*me0_digi_.g_y));
                    if(re==-1) meCollection["strip_dg_zr_rm1"]->Fill(me0_digi_.g_z,glb_R);
                    if(re==1) meCollection["strip_dg_zr_rp1"]->Fill(me0_digi_.g_z,glb_R);

                    /*-------------XY Occupancy---------------*/
                    if(re==-1 && la==1) meCollection["strip_dg_xy_rm1_l1"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==-1 && la==2) meCollection["strip_dg_xy_rm1_l2"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==-1 && la==3) meCollection["strip_dg_xy_rm1_l3"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==-1 && la==4) meCollection["strip_dg_xy_rm1_l4"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==-1 && la==5) meCollection["strip_dg_xy_rm1_l5"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==-1 && la==6) meCollection["strip_dg_xy_rm1_l6"]->Fill(me0_digi_.g_x,me0_digi_.g_y);

                    if(re==1 && la==1) meCollection["strip_dg_xy_rp1_l1"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==1 && la==2) meCollection["strip_dg_xy_rp1_l2"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==1 && la==3) meCollection["strip_dg_xy_rp1_l3"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==1 && la==4) meCollection["strip_dg_xy_rp1_l4"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==1 && la==5) meCollection["strip_dg_xy_rp1_l5"]->Fill(me0_digi_.g_x,me0_digi_.g_y);
                    if(re==1 && la==6) meCollection["strip_dg_xy_rp1_l6"]->Fill(me0_digi_.g_x,me0_digi_.g_y);

                    /*------------ (x_digi_sim - x_digi_rec) ------------*/
                    meCollection["digiDX"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);

                    if(re==-1 && la==1) meCollection["digiDX_rm1_l1"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==-1 && la==2) meCollection["digiDX_rm1_l2"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==-1 && la==3) meCollection["digiDX_rm1_l3"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==-1 && la==4) meCollection["digiDX_rm1_l4"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==-1 && la==5) meCollection["digiDX_rm1_l5"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==-1 && la==6) meCollection["digiDX_rm1_l6"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);

                    if(re==1 && la==1) meCollection["digiDX_rp1_l1"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==1 && la==2) meCollection["digiDX_rp1_l2"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==1 && la==3) meCollection["digiDX_rp1_l3"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==1 && la==4) meCollection["digiDX_rp1_l4"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==1 && la==5) meCollection["digiDX_rp1_l5"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                    if(re==1 && la==6) meCollection["digiDX_rp1_l6"]->Fill(me0_digi_.g_x_sim-me0_digi_.g_x);
                }
            }

        }
        me0_sh.countMatching = count;
    }

    for(ME0DigiPreRecoCollection::DigiRangeIterator cItr = me0_digis->begin(); cItr != me0_digis->end(); ++cItr){

        ME0DetId id = (*cItr).first;

        const GeomDet* gdet = me0_geom->idToDet(id);
        const BoundPlane & surface = gdet->surface();

        //Int_t detId = id();
        Short_t region = (Short_t) id.region();
        //Int_t ring = 0;
        //Int_t station = 0;
        Short_t layer = (Short_t) id.layer();
        //Short_t chamber = (Short_t) id.chamber();
        //Short_t roll = (Short_t) id.roll();

        ME0DigiPreRecoCollection::const_iterator digiItr;
        //loop over digis of given roll
        for (digiItr = (*cItr ).second.first; digiItr != (*cItr ).second.second; ++digiItr)
        {

            Int_t particleType = digiItr->pdgid();
            Float_t timing = digiItr->tof();
          //  Int_t strip = 0;
          //  Int_t bx = 0;

          //  Float_t x = (Float_t) digiItr->x();
          //  Float_t y = (Float_t) digiItr->y();

            LocalPoint lp(digiItr->x(), digiItr->y(), 0);

            GlobalPoint gp = surface.toGlobal(lp);
            Float_t g_r = (Float_t) gp.perp();
            Float_t g_eta = (Float_t) gp.eta();
          //  Float_t g_phi = (Float_t) gp.phi();
            Float_t g_x = (Float_t) gp.x();
            Float_t g_y = (Float_t) gp.y();
            Float_t g_z = (Float_t) gp.z();

            // abbreviations
            int re(region);
            int la(layer);

			//std::cout<<" Global Eta:  "<<g_eta<<std::endl;

            if(digiItr->prompt()){
		  //std::cout<<"Prompt"<<std::endl;
                  meCollection["strip_dg_R_prompt"]->Fill(g_r);
                  meCollection["strip_dg_eta_prompt"]->Fill(std::fabs(g_eta));
                  meCollection["strip_dg_prompt_timing"]->Fill(timing);
                  if ( fabs(particleType) == 11 ) {
                    meCollection["strip_dg_R_prompt_e"]->Fill(g_r);
                    meCollection["strip_dg_eta_prompt_e"]->Fill(std::fabs(g_eta));
                    meCollection["strip_dg_prompt_timing_e"]->Fill(timing);
                  }
                  if ( fabs(particleType) == 13 ) {
                    meCollection["strip_dg_R_prompt_m"]->Fill(g_r);
                    meCollection["strip_dg_eta_prompt_m"]->Fill(std::fabs(g_eta));
                    meCollection["strip_dg_prompt_timing_m"]->Fill(timing);
                  }
                  if ( !(fabs(particleType) == 11 && fabs(particleType) == 13) )  {
                    meCollection["strip_dg_R_prompt_h"]->Fill(g_r);
                    meCollection["strip_dg_eta_prompt_h"]->Fill(std::fabs(g_eta));
                    meCollection["strip_dg_prompt_timing_h"]->Fill(timing);
                  }
                  meCollection["strip_dg_zr_prompt"]->Fill(std::fabs(g_z),g_r);
                  if(re==-1 && la==1) meCollection["strip_dg_xy_rm1_l1_prompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==2) meCollection["strip_dg_xy_rm1_l2_prompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==3) meCollection["strip_dg_xy_rm1_l3_prompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==4) meCollection["strip_dg_xy_rm1_l4_prompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==5) meCollection["strip_dg_xy_rm1_l5_prompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==6) meCollection["strip_dg_xy_rm1_l6_prompt"]->Fill(g_x,g_y);

                  if(re==1 && la==1) meCollection["strip_dg_xy_rp1_l1_prompt"]->Fill(g_x,g_y);
                  if(re==1 && la==2) meCollection["strip_dg_xy_rp1_l2_prompt"]->Fill(g_x,g_y);
                  if(re==1 && la==3) meCollection["strip_dg_xy_rp1_l3_prompt"]->Fill(g_x,g_y);
                  if(re==1 && la==4) meCollection["strip_dg_xy_rp1_l4_prompt"]->Fill(g_x,g_y);
                  if(re==1 && la==5) meCollection["strip_dg_xy_rp1_l5_prompt"]->Fill(g_x,g_y);
                  if(re==1 && la==6) meCollection["strip_dg_xy_rp1_l6_prompt"]->Fill(g_x,g_y);
                }

            if(!(digiItr->prompt())){
                  meCollection["strip_dg_R_NoPrompt"]->Fill(g_r);
                  meCollection["strip_dg_eta_NoPrompt"]->Fill(std::fabs(g_eta));
                  meCollection["strip_dg_timing_NoPrompt"]->Fill(timing);
                  if ( fabs(particleType) == 11 ){
                     meCollection["strip_dg_R_NoPrompt_e"]->Fill(g_r);
                     meCollection["strip_dg_eta_NoPrompt_e"]->Fill(std::fabs(g_eta));
                     meCollection["strip_dg_timing_NoPrompt_e"]->Fill(timing);
                   }
                  if ( particleType == 2112 ){
                    meCollection["strip_dg_R_NoPrompt_n"]->Fill(g_r);
                    meCollection["strip_dg_eta_NoPrompt_n"]->Fill(std::fabs(g_eta));
                    meCollection["strip_dg_timing_NoPrompt_n"]->Fill(timing);
                  }
                  if ( particleType == 22 ) {
                    meCollection["strip_dg_R_NoPrompt_g"]->Fill(g_r);
                    meCollection["strip_dg_eta_NoPrompt_g"]->Fill(std::fabs(g_eta));
                    meCollection["strip_dg_timing_NoPrompt_g"]->Fill(timing);
                  }
                  meCollection["strip_dg_zr_NoPrompt"]->Fill(std::fabs(g_z),g_r);
                  if(re==-1 && la==1) meCollection["strip_dg_xy_rm1_l1_NoPrompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==2) meCollection["strip_dg_xy_rm1_l2_NoPrompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==3) meCollection["strip_dg_xy_rm1_l3_NoPrompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==4) meCollection["strip_dg_xy_rm1_l4_NoPrompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==5) meCollection["strip_dg_xy_rm1_l5_NoPrompt"]->Fill(g_x,g_y);
                  if(re==-1 && la==6) meCollection["strip_dg_xy_rm1_l6_NoPrompt"]->Fill(g_x,g_y);

                  if(re==1 && la==1) meCollection["strip_dg_xy_rp1_l1_NoPrompt"]->Fill(g_x,g_y);
                  if(re==1 && la==2) meCollection["strip_dg_xy_rp1_l2_NoPrompt"]->Fill(g_x,g_y);
                  if(re==1 && la==3) meCollection["strip_dg_xy_rp1_l3_NoPrompt"]->Fill(g_x,g_y);
                  if(re==1 && la==4) meCollection["strip_dg_xy_rp1_l4_NoPrompt"]->Fill(g_x,g_y);
                  if(re==1 && la==5) meCollection["strip_dg_xy_rp1_l5_NoPrompt"]->Fill(g_x,g_y);
                  if(re==1 && la==6) meCollection["strip_dg_xy_rp1_l6_NoPrompt"]->Fill(g_x,g_y);
                    }

        }

    }

}


// ------------ method called once each job just before starting event loop  ------------
void
ME0DigisValidation::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
ME0DigisValidation::endJob()
{
}

// ------------ method called when starting to processes a run  ------------

void ME0DigisValidation::beginRun(edm::Run const&, edm::EventSetup const& iSetup)
{
    try {
        iSetup.get<MuonGeometryRecord>().get(me0_geom);
        me0_geometry_ = &*me0_geom;
        hasME0Geometry_ = true;
    } catch (edm::eventsetup::NoProxyException<ME0Geometry>& e) {
        hasME0Geometry_ = false;
        LogDebug("MuonSimHitAnalyzer") << "+++ Info: ME0 geometry is unavailable. +++\n";
    }

    if(debug_) std::cout<<"booking Global histograms with "<<folderPath_<<std::endl;
    std::string main_folder;
    main_folder = folderPath_;
    std::string prompt_folder = main_folder + "/Prompt";
    std::string noPrompt_folder = main_folder + "/NoPrompt";
    dbe->setCurrentFolder(main_folder);

    if(hasME0Geometry_){

        int num_region=2;
	      int num_layers=6;

        std::string region[2] ={"m1", "p1"};
	      std::string layers[6] = {"l1","l2","l3","l4","l5","l6"};

        meCollection["num_events"]=dbe->book1D("num_events","Number Total Events",3,0,2);
        meCollection["strip_dg_zr_rm1"]=dbe->book2D("strip_dg_zr_rm1","Digi occupancy: region m1;globalZ [cm];globalR [cm]",80,-555,-515,120,20,160);
        meCollection["strip_dg_zr_rp1"]=dbe->book2D("strip_dg_zr_rp1","Digi occupancy: region p1;globalZ [cm];globalR [cm]",80,515,555,120,20,160);
        meCollection["digiDX"]=dbe->book1D("digiDX","x^{digi}_{sim} - x^{digi}_{rec}; x^{digi}_{sim} - x^{digi}_{rec} [cm]; entries",100,-10,+10);

        for(int k=0;k<num_region;k++){
          for(int j=0; j<num_layers;j++){
            //std::cout<<"REGION!!!!!!   "<<region[k]<<std::endl;

            meCollection["strip_dg_xy_r"+region[k]+"_"+layers[j]+""]=dbe->book2D("strip_dg_xy_r"+region[k]+"_"+layers[j]+"","Digi occupancy All: region "+region[k]+", layer "+layers[j]+";globalX [cm];globalY [cm]",120,-280,280,120,-280,280);
            meCollection["strip_dg_xy_r"+region[k]+"_"+layers[j]+"_prompt"]=dbe->book2D("strip_dg_xy_r"+region[k]+"_"+layers[j]+"_prompt","Digi occupancy Prompt: region "+region[k]+", layer "+layers[j]+";globalX [cm];globalY [cm]",120,-280,280,120,-280,280);
            meCollection["strip_dg_xy_r"+region[k]+"_"+layers[j]+"_NoPrompt"]=dbe->book2D("strip_dg_xy_r"+region[k]+"_"+layers[j]+"_NoPrompt","Digi occupancy NoPrompt: region "+region[k]+", layer "+layers[j]+";globalX [cm];globalY [cm]",120,-280,280,120,-280,280);

            meCollection["digiDX_r"+region[k]+"_"+layers[j]+""]=dbe->book1D("digiDX_r"+region[k]+"_"+layers[j]+"","x^{digi}_{sim} - x^{digi}_{rec} region "+region[k]+", layer "+layers[j]+"; x^{digi}_{sim} - x^{digi}_{rec} [cm]; entries",100,-10,+10);
            }


          }


        dbe->setCurrentFolder(prompt_folder);
        meCollection["strip_dg_zr_prompt"]=dbe->book2D("strip_dg_zr_prompt","Digi occupancy Prompt: region m1;globalZ [cm];globalR [cm]",80,515,555,120,20,160);
        // Prompt particles spectrum Vs R
        meCollection["strip_dg_R_prompt"]   =dbe->book1D("strip_dg_R_prompt","Digi R distribution Prompt; globalR [cm]; Events ",90,60,150);
        meCollection["strip_dg_R_prompt_e"]   =dbe->book1D("strip_dg_R_prompt_e","Digi R distribution Prompt electrons; globalR [cm]; Events ",90,60,150);
        meCollection["strip_dg_R_prompt_m"]   =dbe->book1D("strip_dg_R_prompt_m","Digi R distribution Prompt muons; globalR [cm]; Events ",90,60,150);
        meCollection["strip_dg_R_prompt_h"]   =dbe->book1D("strip_dg_R_prompt_h","Digi R distribution Prompt hadrons; globalR [cm]; Events ",90,60,150);
        // Prompt Timing
        meCollection["strip_dg_prompt_timing"]    =  dbe->book1D("strip_dg_prompt_timing","Digi Timing Distribution Prompt; time [ns]; Events",250,-150,100);
        meCollection["strip_dg_prompt_timing_e"]  =  dbe->book1D("strip_dg_prompt_timing_e","Digi Timing Distribution Prompt electrons; time [ns]; Events",250,-150,100);
        meCollection["strip_dg_prompt_timing_m"]  =  dbe->book1D("strip_dg_prompt_timing_m","Digi Timing Distribution Prompt muons; time [ns]; Events",250,-150,100);
        meCollection["strip_dg_prompt_timing_h"]  =  dbe->book1D("strip_dg_prompt_timing_h","Digi Timing Distribution Prompt hadrons; time [ns]; Events",250,-150,100);
		// Prompt particles spectrum Vs Eta
        meCollection["strip_dg_eta_prompt"]   =  dbe->book1D("strip_dg_eta_prompt","Digi |#eta| distribution Prompt; |#eta|; Events ",240,2.0,2.8);
        meCollection["strip_dg_eta_prompt_e"] =  dbe->book1D("strip_dg_eta_prompt_e","Digi |#eta| distribution Prompt electrons; |#eta|; Events ",240,2.0,2.8);
        meCollection["strip_dg_eta_prompt_m"] =  dbe->book1D("strip_dg_eta_prompt_m","Digi |#eta| distribution Prompt muons; |#eta|; Events ",240,2.0,2.8);
        meCollection["strip_dg_eta_prompt_h"] =  dbe->book1D("strip_dg_eta_prompt_h","Digi |#eta| distribution Prompt hadrons; |#eta|; Events ",240,2.0,2.8);


        dbe->setCurrentFolder(noPrompt_folder);
        meCollection["strip_dg_zr_NoPrompt"]=dbe->book2D("strip_dg_zr_NoPrompt","Digi occupancy NoPrompt: region m1;global|Z| [cm];globalR [cm]",80,515,555,120,20,160);
        // NoPrompt particles spectrum Vs R
        meCollection["strip_dg_R_NoPrompt"]   =dbe->book1D("strip_dg_R_NoPrompt","Digi R distribution NoPrompt; globalR [cm]; Events ",90,60,150);
        meCollection["strip_dg_R_NoPrompt_e"]   =dbe->book1D("strip_dg_R_NoPrompt_e","Digi R distribution NoPrompt electrons; globalR [cm]; Events ",90,60,150);
        meCollection["strip_dg_R_NoPrompt_n"]   =dbe->book1D("strip_dg_R_NoPrompt_n","Digi R distribution NoPrompt neutrons; globalR [cm]; Events ",90,60,150);
        meCollection["strip_dg_R_NoPrompt_g"]   =dbe->book1D("strip_dg_R_NoPrompt_g","Digi R distribution NoPrompt gammas; globalR [cm]; Events ",90,60,150);
        // NoPrompt Timing
        meCollection["strip_dg_timing_NoPrompt"]    =  dbe->book1D("strip_dg_timing","Digi Timing Distribution Prompt; time [ns]; Events",250,-150,100);
        meCollection["strip_dg_timing_NoPrompt_e"]  =  dbe->book1D("strip_dg_timing_NoPrompt_e","Digi Timing Distribution Prompt electrons; time [ns]; Events",250,-150,100);
        meCollection["strip_dg_timing_NoPrompt_n"]  =  dbe->book1D("strip_dg_timing_NoPrompt_m","Digi Timing Distribution Prompt neutrons; time [ns]; Events",250,-150,100);
        meCollection["strip_dg_timing_NoPrompt_g"]  =  dbe->book1D("strip_dg_timing_NoPrompt_h","Digi Timing Distribution Prompt gammas; time [ns]; Events",250,-150,100);
		// NoPrompt particles spectrum Vs Eta
        meCollection["strip_dg_eta_NoPrompt"]   =  dbe->book1D("strip_dg_eta_NoPrompt","Digi |#eta| distribution NoPrompt; |#eta|; Events ",240,2.0,2.8);
        meCollection["strip_dg_eta_NoPrompt_e"] =  dbe->book1D("strip_dg_eta_NoPrompt_e","Digi |#eta| distribution NoPrompt electrons; |#eta|; Events ",240,2.0,2.8);
        meCollection["strip_dg_eta_NoPrompt_n"] =  dbe->book1D("strip_dg_eta_NoPrompt_n","Digi |#eta| distribution NoPrompt neutrons; |#eta|; Events ",240,2.0,2.8);
        meCollection["strip_dg_eta_NoPrompt_g"] =  dbe->book1D("strip_dg_eta_NoPrompt_g","Digi |#eta| distribution NoPrompt gammas; |#eta|; Events ",240,2.0,2.8);


    }
}


// ------------ method called when ending the processing of a run  ------------

void ME0DigisValidation::endRun(edm::Run const&, edm::EventSetup const&)
{
    if (EffSaveRootFile_) dbe->save(EffRootFileName_);
}


// ------------ method called when starting to processes a luminosity block  ------------
/*
void
ME0DigisValidation::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
ME0DigisValidation::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

bool ME0DigisValidation::isME0DigiMatched(MyME0Digi me0_dg, MyME0SimHit me0_sh)
{

    Int_t me0_region = me0_dg.region;
    Int_t me0_layer = me0_dg.layer;
    Int_t me0_station = me0_dg.station;
    Int_t me0_chamber = me0_dg.chamber;
    Int_t me0_roll = me0_dg.roll;
    //Int_t me0_strip = me0_dg.strip;

    Int_t me0_sh_region = me0_sh.region;
    Int_t me0_sh_layer = me0_sh.layer;
    Int_t me0_sh_station = me0_sh.station;
    Int_t me0_sh_chamber = me0_sh.chamber;
    Int_t me0_sh_roll = me0_sh.roll;
    //Int_t me0_sh_strip = me0_sh.strip;

    bool cond1, cond2;

    if(me0_sh_region == me0_region && me0_sh_layer == me0_layer && me0_sh_station == me0_station) cond1 = true;
    else cond1 = false;
    if(me0_sh_chamber == me0_chamber && me0_sh_roll == me0_roll) cond2 = true;
    else cond2 = false;

    return (cond1 & cond2);

}

bool ME0DigisValidation::isSimTrackGood(const SimTrack &t)
{
    // SimTrack selection
    if (t.noVertex()) return false;
    if (t.noGenpart()) return false;
    // only muons
    if (std::abs(t.type()) != 13 and simTrackOnlyMuon_) return false;
    // pt selection
    if (t.momentum().pt() < simTrackMinPt_) return false;
    // eta selection
    const float eta(std::abs(t.momentum().eta()));
    if (eta > simTrackMaxEta_ || eta < simTrackMinEta_) return false;
    return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ME0DigisValidation::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
//DEFINE_FWK_MODULE(ME0DigisValidation);
