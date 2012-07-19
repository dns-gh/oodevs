queryImplementation "getCommanding" { ["execute"] = function ( params )
    return integration.getEntitiesFromAutomatCommunication( meKnowledge, params.role, params.withPC)
end}
