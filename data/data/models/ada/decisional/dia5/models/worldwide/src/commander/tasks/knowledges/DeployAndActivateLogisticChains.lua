return
{
    init = function( self, params )
        myself.leadData.subAreas = {} 
    end,

    getDeploymentPosition = function( self, params )
        if params.deploymentPosition ~= NIL then
            if masalife.brain.core.class.isOfType( params.deploymentPosition, world.Area ) then
                if not myself.leadData.currentPosition then
                    myself.leadData.currentPosition = 0 
                    integration.splitArea( params.deploymentPosition, math.min( self:getNbrFront(), params.maxNbrFront ) )
                end
                myself.leadData.currentPosition = myself.leadData.currentPosition % #myself.leadData.subAreas + 1
                local nextObjective = myself.leadData.subAreas[ myself.leadData.currentPosition ]
                return nextObjective:getMyPosition()
            elseif masalife.brain.core.class.isOfType( params.deploymentPosition, world.UrbanBlock ) then
                return params.deploymentPosition:getMyPosition()
            else
                return integration.randomPositionInCircle( params.deploymentPosition, 50 )
            end
        end
        return nil
    end
}