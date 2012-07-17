queryImplementation "getAutomatPCUnit" { ["execute"] = function ( params )
    return { CreateKnowledge( world_elements.PlatoonAlly, DEC_Automate_PionPC() ) }
end}