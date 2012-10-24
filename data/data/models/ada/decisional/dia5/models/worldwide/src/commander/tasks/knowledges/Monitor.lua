return
{
    getScoutObjectives = function( self, params )
        myself.leadData.currentMonitorPosition = myself.leadData.currentMonitorPosition or 0
        myself.leadData.currentMonitorPosition = myself.leadData.currentMonitorPosition % #params.positions + 1
        return { params.positions[ myself.leadData.currentMonitorPosition ] }
    end,

    getObjectives = function( self, params )
       return params.positions
    end,

    getEntities = function( self, params )
        local res = {}
        local pions = integration.getAgentsFromCommander()
        for _, pion in pairs( pions ) do
            res[ #res + 1 ] = CreateKnowledge( world.PlatoonAlly, pion )
        end
        return res
    end,

    getPositions = function( self, params )
        return params.positions
    end,

    getDirection = function( self, params )
        return params.objective
    end
}
