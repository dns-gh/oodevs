queryImplementation "getEntitiesInMyKnowledgeGroupCanIlluminate" { ["execute"] = function ( params )
    local allies  = DEC_Pion_PionsSansPC( ) --no allies given, find one in automat
    for _, ally in pairs( allies ) do
        if ( DEC_Agent_PeutIllumine( ally ) ) then
            alliesKn[ #alliesKn + 1 ] = CreateKnowledge( world.PlatoonAlly, ally )
            return alliesKn --stop when find the first ally
        end
    end
    return alliesKn
end}

