return
{
    init = function( self, params )
        myself.leadData.subAreas = {} 
    end,

    getReachable = function( self, params )
        if params.position ~= NIL then
            if masalife.brain.core.class.isOfType( params.position, sword.military.world.Area ) then
                if not myself.leadData.currentPosition then
                    myself.leadData.currentPosition = 0 
                    integration.splitArea( params.position, math.min( self:getNbrFront(), params.maxNbrFront ) )
                end
                myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
                local nextObjective = myself.leadData.subAreas[ myself.leadData.currentPosition ]
                return nextObjective:getMyPosition()
            elseif masalife.brain.core.class.isOfType( params.position, sword.military.world.UrbanBlock ) then
                return params.position:getMyPosition()
            else
                return integration.randomPositionOnCircle( params.position, 100 )
            end
        end
        return nil
    end
}