return
{
    -- POSITIONNABLE
    canTakePosition = function( self )
        return integration.isInEffectArea( self.proxy ) or self:isReached()
    end,
    -- BYPASSING
    bypassingEfficiency = function( self, objective )
        return self.proxy == objective and 1 or 0
    end,
    predicate "isBypassingFor"
    {
        dependencies = "none",
        method = function( self, objective )
            return self.proxy == objective
        end
    },
    --  REMOVING
    removalEfficiency = function( self, objective )
        return self.proxy == objective and 1 or 0
    end,
    predicate "isRemovingFor"
    {
        dependencies = "none",
        method = function( self, objective )
           return self.proxy == objective
        end
    },
    destroyMoveToIt = function( self )
        return meKnowledge:destroyMoveToItIntegration( self )
    end
}