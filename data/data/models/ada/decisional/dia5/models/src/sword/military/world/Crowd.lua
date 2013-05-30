-- -------------------------------------------------------------------------------- 
-- Predicates
-- --------------------------------------------------------------------------------
predicate "isDangerous"
{
    dependencies = {},
    method = function( self )
        return self:dangerosityLevel() > 50 
    end
}
predicate "isDominated"
{
    dependencies = {},
    method = function( self )
        return self:getDomination() == 0
    end
}
predicate "isAgressive"
{   
    dependencies = {},
    method = function( self )
        return integration.getAttitudeCrowd( self ) == eAttitudeAgressive
    end
}

return
{
    -- -------------------------------------------------------------------------------- 
    -- Decisional methods 
    -- --------------------------------------------------------------------------------
    approachPriority = integration.approachPriority, -- $$$ To be removed
    canApproachIt = function( self )
       return meKnowledge:isSupported()
    end,

    -- -------------------------------------------------------------------------------- 
    -- Integration
    -- --------------------------------------------------------------------------------
    moveToIt = function( self, pathType )
        --DDA à voir=> return meKnowledge:moveToItIntegration( self, pathType )
        return true --$$$ not used
    end,
    destroyMoveToIt = function( self )
        return true --$$$ not used
    end,
    observeIt = function( self )
      return meKnowledge:orientateSensor( self )
    end,
    attackIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startAttackIt, 
        started = integration.updateAttackIt, 
        stop = integration.stopAttackIt
    } ),
    getPosition = function( self )
        return integration.getCrowdPosition( self )
    end,
    getHeadPosition = function( self )
        return CreateKnowledge( sword.military.world.Point, integration.getHeadPosition( self ) )
    end,
    getMyPosition = function( self )
      -- $$$ MIA TODO renvoyer une reaching Area (trackingPosition) pour pouvoir suivre la pop
      return CreateKnowledge( sword.military.world.Point, self:getPosition() )
    end,
    getPositions = function( self )
      return DEC_Geometrie_CalculerTrafficablePointPourPoint( self:getPosition() )
    end,
    getProximity = function( self, reachable )
        if masalife.brain.core.class.isOfType( reachable, sword.military.world.Direction ) then
            return 100 -- $$$$ PSN: TMP pour prendre en compte la direction, qui n'a pas de notion de position
        else
            return integration.normalizedInversedDistance( self, reachable )
        end
    end,
    computeRelation = function( self )
        return integration.computeRelationAgent( self )
    end,
    getPerception = function( self )
        return 100
    end,
    getRole = function( self )
        return self.source:GetType()
    end,
    interrogateIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startInterrogateCrowd,
        started = integration.updateInterrogateCrowd,
        stop = integration.stopInterrogateCrowd
    } ),
    getTasks = function( self )
        return self.tasks
    end,
    getInformations = function( self )
        integration.getCrowdInformations( self )
        return true
    end,
    controlIt = function( self )
        if self:isDominated() then 
             integration.getCrowdInformations( self )
        end
        integration.controlItCrowd( self )
    end,
    getDomination = function( self )
        return integration.getDomination( self )
    end,
    respondToIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startShootingOnCrowd, 
        started = function( self, ... ) end, 
        stop = integration.stopShootingOnCrowd 
     } ),
    dangerosityLevel = function( self ) -- entre 0 et 100
        return integration.getDangerosityLevel( self )
    end,
    affinityLevel = function( self )-- entre 0 et 100
        return integration.affinityLevel( self )
    end,
    isInAttackRange = function( self, suicide, dotation )
        if suicide then return self:isReached() end
        return integration.distance( meKnowledge, self ) < integration.porteePourAttentat( dotation )
    end,
    loadIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startLoadCrowd,
        started = integration.startedLoadCrowd, 
        stop = integration.stopLoadCrowd
    } ),
    unloadIt = masalife.brain.integration.startStopAction( 
    { 
        start = integration.startUnloadCrowd,
        started = integration.startedUnloadCrowd, 
        stop = integration.stopUnloadCrowd
    } ),
    isMoving = function( self )
        return integration.crowdHasFlow( self )
    end,
    canBeFollowed = function( self )
        return true
    end,
    getName = function( self )
        return integration.getName( self )
    end,
    hideInto = masalife.brain.integration.startStopAction( 
    { 
        start = function( self )
            meKnowledge:RC( eRC_startHiddingInTheCrowd )
            integration.lockCrowdKnowledge( self )
            integration.startHidingInCrowd( self )
        end,
        started = function( self )
            return true -- this action cannot fail...
        end,
        stop = function( self )
            meKnowledge:RC( eRC_stopHiddingInTheCrowd )
            integration.stopHidingInCrowd( self )
            integration.unlockCrowdKnowledge( self )
        end
    } ),
    isValid = function( self )
        return integration.isKnowledgeCrowdValid( self.source )
    end,
    getHiddenUnits = function( self )
        return integration.getAgentsHiddenInCrowd( meKnowledge.transportedCrowd )
    end,
    -- -------------------------------------------------------------------------------- 
    -- Attributes
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
    }
}
