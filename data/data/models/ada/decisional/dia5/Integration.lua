include "integration/ToolsFunctions.lua"
include "integration/KnowledgeManager.lua"
include "integration/Types.lua"
include "integration/BaseTaskKnowledge.lua"


integration = {}

include "integration/GeometryIntegration.lua"
include "integration/GetDestructionState.lua"
include "integration/GetIdentificationState.lua"
include "integration/GetOperationalCapacity.lua"
include "integration/GetPerception.lua"
include "integration/Relievableintegration.lua"
include "integration/Element.lua"
include "integration/KeyPoint.lua"
include "integration/Neutralizable.lua"
include "integration/Observable.lua"
include "integration/Reconnoitrable.lua"
include "integration/Unit.lua"
include "integration/UndergroundNetworkIntegration.lua"

include "integration/CRTaskListener.lua"

include "integration/ActivableIntegration.lua"
include "integration/AnimatableIntegration.lua"
include "integration/AskableIntegration.lua"
include "integration/BuildableIntegration.lua"
include "integration/BypassableIntegration.lua"
include "integration/ObservableIntegration.lua"
include "integration/IdentifiableIntegration.lua"
include "integration/ReconnoitrableIntegration.lua"
include "integration/ReconnoitringIntegration.lua"
include "integration/ReachableIntegration.lua"
include "integration/RelievableIntegration.lua"
include "integration/DestroyableIntegration.lua"
include "integration/NeutralizableIntegration.lua"
include "integration/IlluminableIntegration.lua"
include "integration/IndirectFireableIntegration.lua"
include "integration/ImprovableIntegration.lua"
include "integration/RemovableIntegration.lua"
include "integration/QueriesIntegration.lua"
include "integration/IdentifyingIntegration.lua"
include "integration/LoadableIntegration.lua"
include "integration/DamageableIntegration.lua"

include "integration/PositionableIntegration.lua"
include "integration/SafeguardableIntegration.lua"
include "integration/SafeguardingIntegration.lua"
include "integration/SupportingIntegration.lua"

include "integration/DestroyingIntegration.lua"
include "integration/ReachingIntegration.lua"
include "integration/IlluminatingIntegration.lua"

include "integration/ControlableIntegration.lua"
include "integration/SealableIntegration.lua"
include "integration/CommunicationIntegration.lua"

include "integration/AsymetricIntegration.lua"

include "integration/ReinforcableIntegration.lua"

include "integration/FragOrderIntegration.lua"
include "integration/EquipmentIntegration.lua"

include "integration/CrowdIntegration.lua"
include "integration/LogisticIntegration.lua"

include "integration/CommandingFilter.lua"
include "integration/ToolsQueries.lua"

masalife.brain.core.options.feedbacks.checkParamDeclaration = false