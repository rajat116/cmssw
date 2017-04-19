#include <DataFormats/GEMDigi/interface/GEMDigi.h>
#include <DataFormats/GEMDigi/interface/GEMDigiCollection.h>

#include <DataFormats/GEMDigi/interface/GEMPadDigi.h>
#include <DataFormats/GEMDigi/interface/GEMPadDigiCollection.h>

#include <DataFormats/GEMDigi/interface/GEMPadDigiCluster.h>
#include <DataFormats/GEMDigi/interface/GEMPadDigiClusterCollection.h>

#include <DataFormats/GEMDigi/interface/GEMCoPadDigi.h>
#include <DataFormats/GEMDigi/interface/GEMCoPadDigiCollection.h>

#include <DataFormats/GEMDigi/interface/ME0DigiPreReco.h>
#include <DataFormats/GEMDigi/interface/ME0DigiPreRecoCollection.h>

#include <DataFormats/GEMDigi/interface/ME0Digi.h>
#include <DataFormats/GEMDigi/interface/ME0DigiCollection.h>

#include <DataFormats/Common/interface/Wrapper.h>
#include <vector>
#include <map>

namespace DataFormats_GEMDigi {
  struct dictionary {
    
    GEMDigi g;
    std::vector<GEMDigi>  vg;
    std::vector<std::vector<GEMDigi> >  vvg;
    GEMDigiCollection gcol;
    edm::Wrapper<GEMDigiCollection> wg;

    GEMPadDigi gc;
    std::vector<GEMPadDigi>  vgc;
    std::vector<std::vector<GEMPadDigi> >  vvgc;
    GEMPadDigiCollection gccol;
    edm::Wrapper<GEMPadDigiCollection> wgc;

    GEMPadDigiCluster gcc;
    std::vector<GEMPadDigiCluster>  vgcc;
    std::vector<std::vector<GEMPadDigiCluster> >  vvgcc;
    GEMPadDigiClusterCollection gcccol;
    edm::Wrapper<GEMPadDigiClusterCollection> wgcc;

    GEMCoPadDigi gcp;
    std::vector<GEMCoPadDigi>  vgcp;
    std::vector<std::vector<GEMCoPadDigi> >  vvgcp;
    GEMCoPadDigiCollection gcpcol;
    edm::Wrapper<GEMCoPadDigiCollection> wgcp;

    ME0DigiPreReco m;
    std::vector<ME0DigiPreReco>  vm;
    std::vector<std::vector<ME0DigiPreReco> >  vvm;
    ME0DigiPreRecoCollection mcol;
    edm::Wrapper<ME0DigiPreRecoCollection> wm;
    ME0DigiPreRecoMap mmap;
    edm::Wrapper<ME0DigiPreRecoMap> wmmap;

    ME0Digi me;
    std::vector<ME0Digi>  vme;
    std::vector<std::vector<ME0Digi> >  vvme;
    ME0DigiCollection mecol;
    edm::Wrapper<ME0DigiCollection> wme;

    edm::Wrapper<std::map< std::pair<int,int>, int > > a2;
  };
}
