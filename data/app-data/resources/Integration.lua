include "integration/ToolsFunctions.lua"
include "integration/KnowledgeManager.lua"
include "integration/Types_CR.lua"
include "integration/Types.lua"
include "integration/BaseTaskKnowledge.lua"


integration = {}

include "integration/DefaultGetDestructionState.lua"
include "integration/DefaultGetIdentificationState.lua"
include "integration/DefaultGetOperationalCapacity.lua"
include "integration/DefaultGetPerception.lua"
include "integration/DefaultRelievableintegration.lua"
include "integration/Element.lua"
include "integration/KeyPoint.lua"
include "integration/Neutralizable.lua"
include "integration/Observable.lua"
include "integration/Reconnoitrable.lua"
include "integration/Unit.lua"
include "integration/UndergroundNetworkIntegration.lua"

include "integration/CRTaskListener.lua"

include "integration/DefaultActivableIntegration.lua"
include "integration/DefaultAnimatableIntegration.lua"
include "integration/DefaultAskableIntegration.lua"
include "integration/DefaultBuildableIntegration.lua"
include "integration/DefaultBypassableIntegration.lua"
include "integration/DefaultObservableIntegration.lua"
include "integration/DefaultIdentifiableIntegration.lua"
include "integration/DefaultReconnoitrableIntegration.lua"
include "integration/DefaultReconnoitringIntegration.lua"
include "integration/DefaultReachableIntegration.lua"
include "integration/DefaultRelievableIntegration.lua"
include "integration/DefaultDestroyableIntegration.lua"
include "integration/DefaultNeutralizableIntegration.lua"
include "integration/DefaultIlluminableIntegration.lua"
include "integration/DefaultIndirectFireableIntegration.lua"
include "integration/DefaultImprovableIntegration.lua"
include "integration/DefaultRemovableIntegration.lua"
include "integration/DefaultQueriesIntegration.lua"
include "integration/DefaultIdentifyingIntegration.lua"
include "integration/DefaultLoadableIntegration.lua"
include "integration/DefaultDamageableIntegration.lua"

include "integration/DefaultPositionableIntegration.lua"
include "integration/DefaultSafeguardableIntegration.lua"
include "integration/DefaultSafeguardingIntegration.lua"
include "integration/DefaultSupportingIntegration.lua"

include "integration/DefaultDestroyingIntegration.lua"
include "integration/DefaultReachingIntegration.lua"
include "integration/DefaultIlluminatingIntegration.lua"

include "integration/DefaultControlableIntegration.lua"
include "integration/DefaultSealableIntegration.lua"
include "integration/CommunicationIntegration.lua"

include "integration/AsymetricIntegration.lua"

include "integration/DefaultReinforcableIntegration.lua"

include "integration/FragOrderIntegration.lua"
include "integration/DefaultEquipmentIntegration.lua"

include "integration/CrowdIntegration.lua"
include "integration/LogisticIntegration.lua"

include "integration/CommandingFilter.lua"
include "integration/ToolsQueries.lua"

-- for Security
include "integration/UrbanBlock.lua"
include "integration/Teammate.lua"
include "integration/Area.lua"
include "integration/Fire.lua"
include "integration/Object.lua"
include "integration/Commander.lua"


masalife.brain.core.options.feedbacks.checkParamDeclaration = false
