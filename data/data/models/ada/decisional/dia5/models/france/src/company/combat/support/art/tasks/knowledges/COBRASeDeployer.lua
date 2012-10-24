return
{    
    init = function( self, params )
        myself.leadData.subAreas = {} 
    end,
    
    getReachable = function( self, params )
        if params.position ~= NIL then
            local nbUnits = #( integration.getAgentsFromCommander( meKnowledge ) )
            -- division de la zone en sous-zones
            if masalife.brain.core.class.isOfType( params.position, world.Area) then
                if not myself.leadData.currentPosition then
                    myself.leadData.currentPosition = 0 
                    integration.splitArea( params.position, nbUnits )
                end
                myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
                local nextObjective = myself.leadData.subAreas[ myself.leadData.currentPosition ]
                return nextObjective:getMyPosition()
            elseif masalife.brain.core.class.isOfType( params.position, world.Point ) then
                return integration.randomPositionInCircle( params.position, 100 )
            else
                return params.position:getMyPosition()
            end
        end
        local AutomatPosition =  meKnowledge:getMyPosition()
        return integration.randomPositionInCircle( AutomatPosition, 100 )
    end,
}
