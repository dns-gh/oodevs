queryImplementation "getMissionDirection"
{ 
    ["execute"] = function ( params )
        local mission = DEC_GetRawMission( meKnowledge.source )
        if DEC_HasMission( meKnowledge.source ) then
            local dir = DEC_GetDirectionDanger( mission )
            res =  CreateKnowledge( sword.military.world.Direction, dir )
        else
            res = nil
        end
        return { res }
    end
}
