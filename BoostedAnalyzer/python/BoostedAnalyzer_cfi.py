import FWCore.ParameterSet.Config as cms
from BoostedTTH.BoostedAnalyzer.Selection_cff import * # contains addional configurations for the selections classes, like triggers to use
from BoostedTTH.BoostedAnalyzer.Weights_cff import * # defines stuff used for reweighting of the MC to match data PileUp

BoostedAnalyzer = cms.EDAnalyzer( # BoostedAnalyzer is of type EDAnalyzer ( there also exist EDFilters or EDProducers and other stuff )
    'BoostedAnalyzer',
    LeptonSelectionMC, # defined in Selection_cff
    DiLeptonSelectionMC, # defined in Selection_cff
    JetTagSelection, # defined in Selection_cff
    METSelection, # defined in Selection_cff
    checkBasicMCTriggers, # defined in Selection_cff

    era = cms.string("2015_74x"), # has little effect so far, might become important for MiniAODhelper
    analysisType = cms.string("LJ"), # has little effect so far, might become important for MiniAODhelper
    sampleID = cms.int32(9125), # has little effect so far, might become important for MiniAODhelper

    eventWeight = cms.double(1.),
    isData = cms.bool(False),
    
    recorrectMET = cms.bool(True),

    # PU weights, defined in Weights_cff
    nominalPUWeight = cms.PSet(NominalPUWeight),
    additionalPUWeights = cms.VPSet(AdditionalPUWeights),

    makeSystematicsTrees = cms.bool(False),
    doJERsystematic = cms.bool(False),
    generatorName = cms.string("notSpecified"),

    useFatJets = cms.bool(True),
    useForwardJets = cms.bool(False),
    useGenHadronMatch = cms.bool(True),

    dumpSyncExe = cms.bool(False),
    dumpSyncExe2 = cms.bool(False),

    doBoostedMEM = cms.bool(True),

    minJetsForMEM = cms.int32(4),
    minTagsForMEM = cms.int32(3),

    selectionNames = cms.vstring("VertexSelection","LeptonSelection"),
    processorNames = cms.vstring("WeightProcessor","MCMatchVarProcessor","BoostedMCMatchVarProcessor","BasicVarProcessor","MVAVarProcessor","BDTVarProcessor","TriggerVarProcessor","BoostedJetVarProcessor","BoostedTopHiggsVarProcessor"),
    #,"DiJetVarProcessor"), -- conflict
    
    chosenbtaggername=cms.string("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
	selectedCSVworkingpoint=cms.double(0.800),
	selectedCSVLworkingpoint=cms.double(0.460),
	selectedCSVMworkingpoint=cms.double(0.800),
	selectedCSVTworkingpoint=cms.double(0.935),


    outfileName = cms.string("BoostedTTH"),
)
