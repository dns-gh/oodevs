include "integration/ToolsFunctions.lua"

integration = {}

include "integration/DefaultObservableIntegration.lua"
include "integration/DefaultIdentifiableIntegration.lua"
include "integration/DefaultReconnoitrableIntegration.lua"
include "integration/DefaultReachableIntegration.lua"
include "integration/DefaultDestroyableIntegration.lua"
include "integration/DefaultNeutralizableIntegration.lua"
include "integration/DefaultPositionableIntegration.lua"

include "integration/DefaultReachingIntegration.lua"
include "integration/DefaultNeutralizingIntegration.lua"

include "integration/CommunicationIntegration.lua"

-- Frag orders
include "integration/FragOrderIntegration.lua"