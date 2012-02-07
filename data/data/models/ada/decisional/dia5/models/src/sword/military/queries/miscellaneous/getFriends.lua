queryImplementation "getFriends"
{
    ["execute"] = function ( params )
        local friends = {}
        local simAgents  = DEC_Pion_PionsAvecPC()
        for _, simAgent in pairs( simAgents ) do
            local friend = CreateKnowledge( sword.military.world.PlatoonAlly, simAgent )
            friends[ #friends + 1 ] = friend
        end
        return friends
    end
}