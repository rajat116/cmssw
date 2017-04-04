import FWCore.ParameterSet.Config as cms

simMuonME0TrivDigis = cms.EDProducer("ME0DigiProducer",
    signalPropagationSpeed = cms.double(0.66),
    cosmics = cms.bool(False),
    timeResolution = cms.double(5),
    timeJitter = cms.double(1.0),
    averageShapingTime = cms.double(50.0),
    averageEfficiency = cms.double(0.98),
    averageNoiseRate = cms.double(0.001), #intrinsic noise
    bxwidth = cms.int32(25),
    minBunch = cms.int32(-5), ## in terms of 25 ns
    maxBunch = cms.int32(3),
    mixLabel = cms.string('mix'),	# added by A.Sharma
    inputCollection = cms.string('g4SimHitsMuonME0Hits'),
    me0digiModelString = cms.string('Simple'),
    digitizeOnlyMuons = cms.bool(False),
    doBkgNoise = cms.bool(True), #False == No background simulation
    doNoiseCLS = cms.bool(True),
    fixedRollRadius = cms.bool(True), #Uses fixed radius in the center of the roll
    simulateIntrinsicNoise = cms.bool(False),
    simulateElectronBkg = cms.bool(True),	#False=simulate only neutral Bkg
#    simulateLowNeutralRate = cms.bool(False),	#True=neutral_Bkg at L=1x10^{34}, False at L=5x10^{34}cm^{-2}s^{-1}, No further need ot this parameter,
						#since the effect ot inst. lumi now is controled by the instLumi parameter.
    instLumi = cms.double(7.5),                 # in units of 1E34 cm^-2 s^-1. Internally the background is parametrized from FLUKA+GEANT results at 5x10^34 (PU140). We are adding a 1.5 factor for PU200
    rateFact = cms.double(1.0)                 # change the safety factor from 2 to 1, since the effect of the new beam pipe is included in the new background model

)


