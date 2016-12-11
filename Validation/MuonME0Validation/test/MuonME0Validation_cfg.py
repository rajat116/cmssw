import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2015MuonGEMDevReco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load("DQMServices.Components.MEtoEDMConverter_cfi")
process.load("DQMServices.Core.DQM_cfg")

## global tag for 2019 upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_design', '')

process.load("Validation.MuonME0Validation.MuonME0Hits_cfi")
process.me0HitsValidation.EffSaveRootFile = True
process.load("Validation.MuonME0Validation.MuonME0Digis_cfi")
process.me0DigiValidation.EffSaveRootFile = True
process.load("Validation.MuonME0Validation.MuonME0RecHits_cfi")
process.me0RecHitsValidation.EffSaveRootFile = True

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",

	fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/r/rgupta/CMSSW_9_0_0_pre1/src/step2.root',
) 
)
process.p = cms.Path(#process.me0HitsValidation *
		     process.me0DigiValidation
		     #process.me0RecHitsValidation
)

'''# Automatic addition of the customisation function from Validation.MuonME0Hits.me0Custom
from Validation.MuonME0Hits.me0Custom import customise2023

#call to customisation function customise2023 imported from Validation.MuonME0Hits.me0Custom
process = customise2023(process)

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.fixMissingUpgradeGTPayloads
from SLHCUpgradeSimulations.Configuration.fixMissingUpgradeGTPayloads import fixCSCAlignmentConditions

#call to customisation function fixCSCAlignmentConditions imported from SLHCUpgradeSimulations.Configuration.fixMissingUpgradeGTPayloads
process = fixCSCAlignmentConditions(process)

# Automatic addition of the customisation function from SimGeneral.MixingModule.fullMixCustomize_cff
from SimGeneral.MixingModule.fullMixCustomize_cff import setCrossingFrameOn

#call to customisation function setCrossingFrameOn imported from SimGeneral.MixingModule.fullMixCustomize_cff
process = setCrossingFrameOn(process)

# End of customisation functions'''

