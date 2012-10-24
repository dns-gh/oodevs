return
{
    getEntities = function( self, params )
        local res = {}
        local pions = integration.getAgentsFromCommander( meKnowledge )
        for _, pion in pairs( pions ) do
            res[ #res + 1 ] = CreateKnowledge( world.PlatoonAlly, pion )
        end
        return res
    end,

    getPositions = function( self, params )

      -- Couverture à partir de points : attribution de 1 point par pion
       if not myself.leadData.positionIndex then
            myself.leadData.positionIndex = 0 
       end

       myself.leadData.positionIndex = myself.leadData.positionIndex % #params.positions + 1

        return { params.positions[myself.leadData.positionIndex] }
    end,

    getDirectionPoints = function( self, params )
        return params.directionPoints
    end,
}
