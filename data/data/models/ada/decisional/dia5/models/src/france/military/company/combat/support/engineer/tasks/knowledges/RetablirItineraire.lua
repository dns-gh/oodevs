return
{
    getObjectivesGen = function( self, params )
        return params.objectives
    end,

    getPositions = function( self, params )
        local whoReinforce = self:getEntitiesToSupport( params )[1]
        local positions = {}
        for _, point in pairs(params.objectives) do
            positions[ #positions + 1 ] = point
        end
        positions[ #positions + 1 ] = CreateKnowledge( world.Point, whoReinforce:getPosition() )
        return positions
    end,
}