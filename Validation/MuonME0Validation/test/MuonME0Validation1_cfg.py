import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load('Configuration.Geometry.GeometryExtended2015MuonGEMDevReco_cff')
#process.load('Configuration.Geometry.GeometryExtended2023D9Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2023D4Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2023D4_cff')
process.load('Configuration.Geometry.GeometryExtended2023D12Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D12_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load("DQMServices.Components.MEtoEDMConverter_cfi")
process.load("DQMServices.Core.DQM_cfg")

## global tag for 2019 upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic', '')

process.load("Validation.MuonME0Validation.MuonME0Hits_cfi")
#process.me0HitsValidation.EffSaveRootFile = True
process.load("Validation.MuonME0Validation.MuonME0Digis_cfi")
process.me0DigiValidation.EffRootFileName = cms.untracked.string('ME0Digis_out.root')
#process.me0DigiValidation.EffSaveRootFile = True
process.load("Validation.MuonME0Validation.MuonME0RecHits_cfi")
#process.me0RecHitsValidation.EffSaveRootFile = True

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",

	fileNames = cms.untracked.vstring( *('/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_1.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_10.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_100.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_101.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_102.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_103.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_104.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_105.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_106.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_107.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_108.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_109.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_11.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_110.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_111.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_112.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_113.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_114.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_115.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_116.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_117.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_118.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_119.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_12.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_120.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_13.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_14.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_15.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_16.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_17.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_18.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_19.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_2.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_20.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_21.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_22.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_23.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_24.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_25.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_26.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_27.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_28.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_29.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_3.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_30.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_31.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_32.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_33.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_34.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_35.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_36.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_37.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_38.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_39.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_4.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_40.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_41.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_42.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_43.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_44.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_45.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_46.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_47.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_48.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_49.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_5.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_50.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_51.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_52.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_53.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_54.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_55.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_56.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_57.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_58.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_59.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_6.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_60.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_61.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_62.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_63.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_64.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_65.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_66.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_67.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_68.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_69.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_7.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_70.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_71.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_72.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_73.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_74.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_75.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_76.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_77.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_78.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_79.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_8.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_80.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_81.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_82.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_83.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_84.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_85.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_86.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_87.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_88.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_89.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_9.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_90.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_91.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_92.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_93.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_94.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_95.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_96.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_97.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_98.root',
'/store/user/rgupta/Signal/CMSSW_9_1_X_SingleNuE10_D12_Digi_10Apr/170410_140751/0000/step2_SingleNuE10_99.root',) ) 
)

process.p = cms.Path(#process.me0HitsValidation *
		     process.me0DigiValidation
		     #process.me0RecHitsValidation
)

'''# Automatic addition of the customisation function from Validation.MuonME0Hits.me0Custom
from Validation.MuonME0Validation.me0Custom import customise2023

#call to customisation function customise2023 imported from Validation.MuonME0Hits.me0Custom
process = customise2023(process)'''

'''# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.fixMissingUpgradeGTPayloads
from SLHCUpgradeSimulations.Configuration.fixMissingUpgradeGTPayloads import fixCSCAlignmentConditions

#call to customisation function fixCSCAlignmentConditions imported from SLHCUpgradeSimulations.Configuration.fixMissingUpgradeGTPayloads
process = fixCSCAlignmentConditions(process)'''

'''# Automatic addition of the customisation function from SimGeneral.MixingModule.fullMixCustomize_cff
from SimGeneral.MixingModule.fullMixCustomize_cff import setCrossingFrameOn
#call to customisation function setCrossingFrameOn imported from SimGeneral.MixingModule.fullMixCustomize_cff
process = setCrossingFrameOn(process)
# End of customisation functions'''
