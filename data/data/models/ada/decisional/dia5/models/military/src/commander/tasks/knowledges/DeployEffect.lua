return
{
    init = function( self, params )
        myself.leadData.subAreas = {} 
    end,

    getObjective = function( self, params )
        if params.objective ~= NIL then
            if masalife.brain.core.class.isOfType( params.objective, world.Area ) then
                if not myself.leadData.currentPosition then
                    myself.leadData.currentPosition = 0 
                    integration.splitArea( params.objective, math.min( self:getNbrFront(), params.maxNbrFront ) )
                end
                myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
                local nextObjective = myself.leadData.subAreas[ myself.leadData.currentPosition ]
                return nextObjective:getMyPosition()
            elseif masalife.brain.core.class.isOfType( params.objective, world.UrbanBlock ) then
                return params.objective:getMyPosition()
            else
                return integration.randomPositionInCircle( params.objective, 100 )
            end
        end
        return nil
    end
}