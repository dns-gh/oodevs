queryImplementation "getAutomatPCUnit" { ["execute"] = function ( params )
    return { CreateKnowledge( sword.military.world.PlatoonAlly, DEC_Automate_PionPC() ) }
end}