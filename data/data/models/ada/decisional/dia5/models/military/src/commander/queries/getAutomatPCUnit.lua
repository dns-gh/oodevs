queryImplementation "getAutomatPCUnit" { ["execute"] = function ( params )
    return { CreateKnowledge( world.PlatoonAlly, DEC_Automate_PionPC() ) }
end}