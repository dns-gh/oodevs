return
{
    -- -------------------------------------------------------------------------------- 
    -- High level methods
    -- --------------------------------------------------------------------------------
    constructionLevel = function( self )
        if self.knowledge then
            return integration.buildLevel( self.knowledge )
        else
            return 0
        end
    end,
    buildPriority = function( self )
        local obstacleType = 0 -- First build preliminary obstacles,at the end build maneuver obstacles
        if integration.isManeuverGenObject( self.source ) == false then
            obstacleType = 1
        end
        return ( ( self:proximityLevel() / 100 + obstacleType ) ) / 2
    end,
    buildEfficiency = function( self, objective )
        return self == objective and 1 or 0
    end,
    isReconnoitred = function( self )
        local position = self:getMyPosition()
        return position:isReconnoitred()
    end,
    reconnoitreIt = function( self )
        local position = self:getMyPosition()
        return position:reconnoitreIt()
    end,

    -- -------------------------------------------------------------------------------- 
    -- Predicates
    -- --------------------------------------------------------------------------------
    predicate "isBuildingFor"
    {
       method = function( self, objective )
          -- $$$ MIA TODO à revoir, ne pas utlisé une efficiency
          return self:buildEfficiency( objective )
       end,
    },
    predicate "isBuilt"
    {
        method = function( self )
          if self[myself] then
            return self[myself].actionBuildState == eActionObjetTerminee 
          elseif self.knowledge then
            return true
          else
            return self:constructionLevel() == 100
          end
        end,
    },

    -- -------------------------------------------------------------------------------- 
    -- Integration
    -- --------------------------------------------------------------------------------
    getProximity = function( self, reachable )
        if masalife.brain.core.class.isOfType( reachable, world.Direction ) then
            return 100 -- $$$$ PSN: TODO TMP pour prendre en compte la direction, 
                       -- qui n'a pas de notion de position
        else
            return integration.normalizedInversedDistance( self, reachable )
        end
    end,
    getPosition = function( self )
        return integration.getEngineerObjectPosition( self )
    end,
    getMyPosition = function( self )
        return CreateKnowledge( world.Point, self:getPosition() )
    end,
    getPositions = function( self )
        return integration.getPointPositions( self:getMyPosition() )
    end,
    canBuildIt = function( self )
        local objectType = self:getType()
        return integration.canBuildObjectType( objectType, self:getLocalisation() ) and integration.hasEnoughtDotationForObjectType( objectType )
    end,
    canBuildItNow = function( self )
        local objectType = self:getType()
        return integration.canBuildNowObjectType( objectType, self:getLocalisation() ) and integration.hasEnoughtDotationForObjectType( objectType )
    end,
    buildingCapacity = function( self )
        return integration.canBuildObjectType( self:getType(), self:getLocalisation() )
    end,
    buildingCapacityWithOutReinforcement = function ( self )
        return integration.canBuildObjectTypeWithoutReinforcement( self:getType(), self:getLocalisation() ) 
    end,
    buildingDotation = function( self )
        return integration.hasEnoughtDotationForObjectType( self:getType() )
    end,
    buildIt = masalife.brain.integration.startStopAction( 
    { 
        start = function( self )
            integration.startBuildIt( self, world.Object )
        end, 
        started = integration.updateBuildIt, 
        stop = integration.stopBuildIt
    } ),
    takeUpPosition = integration.takeUpPosition,
    isValid = function( self )
        if self.knowledge then
            return self.knowledge:isValid()
        else
            return true
        end
    end,
    canAnimateIt = function( self )
        if self.knowledge then
            return self.knowledge:canAnimateIt()
        else
            return false
        end
    end,
    animationLevel = function( self )
        if self.knowledge then
            return self.knowledge:animationLevel()
        else
            return 0
        end
    end,
    animateIt = function( self )
        if self.knowledge then
            return self.knowledge:animateIt()
        else
            return false
        end
    end,
    getLocalisation = function( self )
        return integration.getGenObjectLocation( self.source )
    end,
    getType = function( self )
        return integration.getGenObjectType( self.source )
    end,
    plannedworkToWork = function( self )
        if self.knowledge then
            return self.knowledge
        end
        local element = integration.obtenirObjetProcheDe( self:getLocalisation(), self:getType(), 20 )
        if element then
            return CreateKnowledge( world.Object, element )
        end  
        return self
    end,
    isManeuverObstacle = function( self )
      if self.knowledge then
        return self.knowledge:isManeuverObstacle()
      else
        return false
      end
    end,
    isActifManeuverObstacle = function( self )
      if self.knowledge then
        return self.knowledge:isActifManeuverObstacle()
      else
        return false
      end
    end,
    canBeImproved = function( self )
        return false
    end,
    delayedActivationTime = function( self )
        return integration.delaiAvantActivationForGenObject( self )
    end
}