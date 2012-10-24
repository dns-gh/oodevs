queryImplementation "getMissionDirection"
{ 
    ["execute"] = function ( params )
        local mission = integration.getRawMission( meKnowledge.source )
        if integration.hasMission( meKnowledge.source ) then
            local dir = integration.getDangerousDirection( mission )
            res =  CreateKnowledge( world.Direction, dir )
        else
            res = nil
        end
        return { res }
    end
}
