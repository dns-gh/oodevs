
predicate "isAgressive"
{
    dependencies = {},
    method = function( self )
        return integration.getAttitude() == eAttitudeAgressive
    end 
}
predicate "isDominated"
{
    dependencies = {},
    method = function( self )
        return integration.getMyDomination( self ) == 0
    end 
}

return
{
    -- -------------------------------------------------------------------------------- 
    -- Methods
    -- --------------------------------------------------------------------------------
    getPosition = function( self )
        return integration.getCrowdPosition( self )
    end,
    getMyPosition = function( self )
      return CreateKnowledge( sword.military.world.Point, self:getPosition() )
    end,
    adoptAgressiveAttitude = function( self )
        integration.setAttitude( eAttitudeAgressive )
    end,
    moveToItIntegration = function( self, entity, pathType )
        return integration.moveToItCrowd( entity, pathType )
    end,
    destroyMoveToItIntegration = function( self, position )
        return integration.stopMoveToItCrowd( position )
    end,
    orientateSensor = function( self, entity )
        return false -- A crowd don't have sensors. NOT USED
    end,
    RC = function( self, ... )
        integration.crowdRC( ... )
    end,
    getTasks = function( self )
        return self.tasks
    end,
    computeMovementCapability = function( self, position )
        return true
    end,
    getDomination = function( self )
        return integration.getDomination( self )
    end,
    setDomination = function( self, domination )
        integration.setDomination( domination )
    end,
    isContaminated = function( self )
        return integration.crowdIsContamined( self )
    end,
    demonstrate = masalife.brain.integration.startStopAction(
    { 
        start = integration.startDemonstrate,
        started = integration.uptateDemonstrate,
        stop = integration.stopDemonstrate 
    } ),
    -- -------------------------------------------------------------------------------- 
    -- Attributes
    -- $$$ MIA TODO A revoir, inutile pour les crowd
    -- --------------------------------------------------------------------------------
    tasks =
    {
        adder = function( self, task ) error( "unused method", 2 ) end,
        accepter = function( self, visitor )
            local function roleVisitor( role )
                if role.body == self then return role.tasks:accept( visitor ) end
            end
            for group in masalife.brain.knowledge.executeQuery( { knowledgeType = { directia.core.model.Group } } ) do
                return group.roles:accept( roleVisitor )
            end
        end,
        remover = function( self, task )
            error( "unused method", 2 )
        end
     },
}
