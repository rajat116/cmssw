import FWCore.ParameterSet.Config as cms
from Validation.MuonME0Validation.simTrackMatching_cfi import SimTrackMatching

me0DigiValidation = cms.EDAnalyzer('ME0DigisValidation',
    #verboseSimHit = cms.untracked.int32(1),
    input = cms.InputTag('g4SimHits'),
    siminput = cms.InputTag('g4SimHits','MuonME0Hits'),
    digiinput = cms.InputTag('simMuonME0Digis'),
    debug = cms.untracked.bool(False),
    folderPath = cms.untracked.string('MuonME0DigisV/ME0DigiTask'),
    #stripDigiLabel = cms.InputTag("simMuonME0Digis"),
    #simInputLabel = cms.InputTag('g4SimHits',"MuonME0Hits"),
    #digiInputLabel = cms.InputTag("simMuonME0Digis"),
    # st1, st2_short, st2_long of xbin, st1,st2_short,st2_long of ybin
    #nBinGlobalZR = cms.untracked.vdouble(80,120),
    # st1 xmin, xmax, st2_short xmin, xmax, st2_long xmin, xmax, st1 ymin, ymax...
    #RangeGlobalZR = cms.untracked.vdouble(515,555,20,160),
    #nBinGlobalXY = cms.untracked.int32(360),
    EffSaveRootFile = cms.untracked.bool(True),
    EffRootFileName = cms.untracked.string('ME0Digis_ME.root'),
    simTrackMatching = SimTrackMatching
)
