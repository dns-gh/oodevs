queryImplementation "getAutomatPCUnit" { ["execute"] = function ( params )
    return { CreateKnowledge( world.PlatoonAlly, integration.commanderGetHQUnit() ) }
end}