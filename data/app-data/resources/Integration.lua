integration = {}
utilities = {}

-- Load pre-requisite Lua modules
require "debug"
require "math"
require "string"
require "table"

include "integration/Reports.lua"
include "integration/SimulationIntegration.lua"
include "integration/KnowledgeManager.lua"
include "integration/Types.lua"
include "integration/BaseTaskKnowledge.lua"

include "integration/ToolsFunctions.lua"
include "integration/TableIntegration.lua"
include "integration/GetPerception.lua"
include "integration/Relievableintegration.lua"
include "integration/Element.lua"
include "integration/KeyPoint.lua"
include "integration/Neutralizable.lua"
include "integration/Observable.lua"
include "integration/Reconnoitrable.lua"
include "integration/Unit.lua"
include "integration/UndergroundNetworkIntegration.lua"
include "integration/LeadIntegration.lua"

include "integration/CRTaskListener.lua"

include "integration/ActivableIntegration.lua"
include "integration/AnimatableIntegration.lua"
include "integration/AskableIntegration.lua"
include "integration/BuildableIntegration.lua"
include "integration/BypassableIntegration.lua"
include "integration/ObservableIntegration.lua"
include "integration/IdentifiableIntegration.lua"
include "integration/ReconnoitrableIntegration.lua"
include "integration/ReachableIntegration.lua"
include "integration/RelievableIntegration.lua"
include "integration/DestroyableIntegration.lua"
include "integration/NeutralizableIntegration.lua"
include "integration/IlluminableIntegration.lua"
include "integration/IndirectFireableIntegration.lua"
include "integration/ImprovableIntegration.lua"
include "integration/RemovableIntegration.lua"
include "integration/LoadableIntegration.lua"

include "integration/PositionableIntegration.lua"
include "integration/SafeguardableIntegration.lua"
include "integration/SafeguardingIntegration.lua"
include "integration/SupportingIntegration.lua"

include "integration/DestroyingIntegration.lua"
include "integration/ReachingIntegration.lua"

include "integration/ControlableIntegration.lua"
include "integration/SealableIntegration.lua"
include "integration/CommunicationIntegration.lua"

include "integration/AsymetricIntegration.lua"

include "integration/ReinforcableIntegration.lua"

include "integration/FragOrderIntegration.lua"
include "integration/EquipmentIntegration.lua"

include "integration/CrowdBodyIntegration.lua"
include "integration/CrowdIntegration.lua"
include "integration/LogisticIntegration.lua"

include "integration/CommandingFilter.lua"
include "integration/ToolsQueries.lua"

include "integration/GeometryIntegration.lua"
include "integration/LimaIntegration.lua"

-- for Security
include "integration/UrbanBlock.lua"
include "integration/Area.lua"
include "integration/Fire.lua"
include "integration/Object.lua"
include "integration/Commander.lua"


masalife.brain.core.options.feedbacks.checkParamDeclaration = false

-- Allow devtools plugin to output traces:
TraceGraph = function( trace ) DEC_Trace( trace ) end

-- Allow masalife_brain plugin to output messages
LogMessage = function ( tag, message )
    -- Make sure we do nothing if this function is called while in a restricted environment
    if DEC_Trace then
        DEC_Trace( "[" .. tag .. "] " .. message ) 
    end
end
