#ifndef ME0DIGISIMLINK_CLASSES_H
#define ME0DIGISIMLINK_CLASSES_H

#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include <vector>
#include <map>

#include "SimDataFormats/ME0DigiSimLink/interface/ME0DigiSimLink.h"

namespace SimDataFormats_ME0DigiSimLink
{
  struct dictionary
  {
    edm::Wrapper<ME0DigiSimLink> ME0DigiSimLinkWrapper;
    edm::Wrapper<std::vector<ME0DigiSimLink> > ME0DigiSimLinkVector;

    edm::Wrapper<edm::DetSet<ME0DigiSimLink> > ME0DigiSimLinkDetSetWrapper;
    edm::Wrapper<std::vector<edm::DetSet<ME0DigiSimLink> > > ME0DigiSimLinkVectorDetSet;
    edm::Wrapper<edm::DetSetVector<ME0DigiSimLink> > ME0DigiSimLinkDetSetVectorWrapper;
  };
}

#endif

