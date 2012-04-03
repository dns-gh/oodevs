require "debug"

--Reachable Implementation
--TODO use map to convert string to int/ or other  move function approach
integration.getPlatoonPosition = function( platoon )
    if DEC_ConnaissanceAgent_EstValide( platoon.source ) then
        return DEC_ConnaissanceAgent_Position( platoon.source )
    else
        return nil
    end
end

-- Ally
integration.getPlatoonAllyPosition = function( platoon )
  if platoon == myself then -- @TODO MGD replace by shared_ptr< DEC_Knowledge_Agent_ABC > and proxy for allies
    return DEC_Agent_Position()
  else
    return DEC_Agent_PositionPtr( platoon.source )
  end
end

integration.getHeadPosition = function( crowd )
    if DEC_ConnaissancePopulation_HasFlow( crowd.source ) then
        local pointEnTete = DEC_ConnaissancePopulation_PointEnTeteDeFlux( myself, crowd.source )
        crowd.getPointEnTeteResult = crowd.getPointEnTeteResult or pointEnTete
        if DEC_Geometrie_DistanceBetweenPoints( crowd.getPointEnTeteResult,  pointEnTete) > 30 then
            crowd.getPointEnTeteResult = pointEnTete
        end
        return crowd.getPointEnTeteResult
    else
        return integration.getCrowdPosition( crowd )
    end
end

integration.getCrowdPosition = function( crowd )
    if crowd == meKnowledge then
        return DEC_GetPosition()
    else
        local pointLePlusProche = DEC_ConnaissancePopulation_PointPlusProche( crowd.source )
        crowd.getCrowdPositionResult =  crowd.getCrowdPositionResult or pointLePlusProche
        if DEC_Geometrie_DistanceBetweenPoints( pointLePlusProche, crowd.getCrowdPositionResult ) > 30 then
            crowd.getCrowdPositionResult = pointLePlusProche
        end
        return crowd.getCrowdPositionResult
    end
end

integration.getPointPosition = function( point )
  return point.source
end
integration.getItineraryPosition = function( itinerary )
  return DEC_Itineraire_DernierPoint( itinerary.source )
end

integration.getObjectPosition = function( object )
    object.getObjectPositionResult = object.getObjectPositionResult  or ( DEC_IsValidKnowledgeObject( object.source ) and DEC_Geometrie_CalculerBarycentreLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) ) ) or nil
    return object.getObjectPositionResult
end
integration.getObjectPositions = function( object )
    object.getObjectPositionsResult = object.getObjectPositionsResult  or ( DEC_IsValidKnowledgeObject( object.source ) and DEC_Geometrie_ListePointsLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) ) ) or {}
    return object.getObjectPositionsResult
end
integration.getEngineerObjectPosition = function( object )
  object.getEngineerObjectPositionResult = object.getEngineerObjectPositionResult or DEC_Geometrie_CalculerBarycentreLocalisation( DEC_GenObject_Localisation( object.source ) )
  return object.getEngineerObjectPositionResult 
end
integration.getAreaPosition = function( area )
    area.getAreaPositionResult = integration.getCentralAreaPosition( area )
    if integration.isPointInUrbanBlockTrafficable(CreateKnowledge( sword.military.world.Point, area.getAreaPositionResult), true ) == 0 then
        return integration.getAreaPositions( area )[1]
    end
    return area.getAreaPositionResult
end
integration.getCentralAreaPosition = function( area )
    area.getCentralAreaPositionResult = area.getCentralAreaPositionResult or DEC_Geometrie_CalculerBarycentreLocalisation( area.source )
    return area.getCentralAreaPositionResult
end
integration.getUrbanBlockPosition = function( urbanBlock )
  urbanBlock.getUrbanBlockPosition = urbanBlock.getUrbanBlockPosition or DEC_ConnaissanceUrbanBlock_BarycentreDansBU( urbanBlock.source )
  return urbanBlock.getUrbanBlockPosition
end
integration.getUrbanBlockPositions = function( urbanBlock )
  urbanBlock.getUrbanBlockPositionsResult = urbanBlock.getUrbanBlockPositionsResult or DEC_Geometrie_CalculerLocalisationsBU( urbanBlock.source )
  return urbanBlock.getUrbanBlockPositionsResult
end
integration.getAreaPositions = function( area )
    area.getAreaPositionsResult = area.getAreaPositionsResult or DEC_Geometrie_CalculerTrafficablePointPourPoint( integration.getCentralAreaPosition( area ) )
    area.getTrafficableAreaPositionsResult = area.getTrafficableAreaPositionsResult or {}
    if #area.getTrafficableAreaPositionsResult == 0 then
        for i = 1, #area.getAreaPositionsResult do -- On ne veut que les positions qui sont dans la zone
            if integration.isPointInLocalisation(CreateKnowledge( sword.military.world.Point, area.getAreaPositionsResult[i] ), area) then
               area.getTrafficableAreaPositionsResult[#area.getTrafficableAreaPositionsResult] = area.getAreaPositionsResult[i]
            end
        end
    end
    if #area.getTrafficableAreaPositionsResult > 0 then
        return area.getTrafficableAreaPositionsResult
    else
        return area.getAreaPositionsResult
    end
end
integration.getPointPositions = function( point )
  point.getPointPositionsResult = point.getPointPositionsResult or DEC_Geometrie_CalculerTrafficablePointPourPoint( point.source )
  return point.getPointPositionsResult
end

integration.getPointPositionsForProxy = function( point )
  local urbanblock = DEC_Connaissances_BlocUrbainPourPosition( point.source )
  if urbanblock and ( point.getPointPositionsResultBlock == urbanblock ) then
    return point.getPointPositionsResult
  end
  point.getPointPositionsResultBlock = urbanblock
  if not point.getPointPositionsResultBlock then
    return { point.source }
  end
  point.getPointPositionsResult = DEC_Geometrie_CalculerTrafficablePointPourPoint( point.source )
  return point.getPointPositionsResult
end

integration.pointIsInCity = function( position )
    position.isInCityCache = position.isInCityCache or DEC_IsPointInCity( position )
    return position.isInCityCache
end

integration.distance = function( pos1, pos2 )
    if not pos1 or not pos2 then
        error( "knowledge invalid with a nil position" )
    end
    local posA = pos1:getPosition()
    local posB = pos2:getPosition()
    if posA and posB then
        return DEC_Geometrie_DistanceBetweenPoints( posA, posB )
    else
        error( "knowledge invalid with a nil position" )
    end
end

integration.normalizedInversedDistance = function( pos1, pos2 )
    if not pos1 or not pos2 then
        return 1
    end
    local pos1Pos = pos1:getPosition()
    if not pos1Pos then
        return 1
    end
    local pos2Pos = pos2:getPosition()
    if not pos2Pos then
        return 1
    end
    local distanceMax = DEC_Detection_Distance and DEC_Detection_Distance() or 4000
    return LinearInterpolation( 1, 100, 10, distanceMax, false, integration.distance( pos1, pos2 ) )
end




-- ============================================================================
-- MOVEMENT MANAGEMENT
-- Manage Mount and dismount terrain constraints
-- ============================================================================
--  Tool functions
-- check if two points are inside same urban block
local pointsInsideSameUrbanBlock = function( simPosA, simPosB )
    local urbanBlockA = DEC_Connaissances_BlocUrbainPourPosition( simPosA )
    local urbanBlockB = DEC_Connaissances_BlocUrbainPourPosition( simPosB )
    if urbanBlockA and urbanBlockB then
        return urbanBlockA == urbanBlockB
    else
        return false
    end
end
-- check get nearest points among those issued as parameters
local getNearestSimPoint = function( target, simPoints )
    local nearest = simPoints[ 1 ]
    local DEC_Geometrie_DistanceBetweenPoints = DEC_Geometrie_DistanceBetweenPoints
    local nPositions = #simPoints
    local nearestDistance = DEC_Geometrie_DistanceBetweenPoints( target, nearest )
    for i = 2, nPositions do
        local position = simPoints[ i ]
        local currentDistance = DEC_Geometrie_DistanceBetweenPoints( target, position )
        if currentDistance < nearestDistance then
            nearest = position
            nearestDistance = currentDistance
        end
    end
    return nearest
end

-- ****************************************************************************
-- integration.moveToItGeneric
-- ****************************************************************************
integration.moveToItGeneric = masalife.brain.integration.startStopAction( 
{ 
    start   = function( entity, pathType )
                integration.startMoveToIt( entity, pathType ) 
              end,
    started = function( entity, pathType )
                  integration.updateMoveToIt( entity, pathType )
              end, 
    stop    = function( entity, pathType )
               integration.deselectMoveToIt( entity )
              end,
} )

-- ****************************************************************************
-- integration.startMoveToIt 
--  mount dismount agent capabilities
--  pathfinding
--  movement along computed path
-- ****************************************************************************
integration.startMoveToIt = function( objective, pathType )

    objective[ myself ] = objective[ myself ] or {}

    -- -------------------------------------------------------------------------------- 
    -- Leaving occupied position
    -- -------------------------------------------------------------------------------- 
    if myself.actionOccupy then
      myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    myself.location = nil

    -- -------------------------------------------------------------------------------- 
    -- Retreive current paused movement action if it was suspended
    -- --------------------------------------------------------------------------------
    if objective[ myself ].moveAction then
        DEC_ReprendAction( objective[ myself ].moveAction )
        return false
    end
    
    -- -------------------------------------------------------------------------------- 
    -- Terrain management: reach a accessible position if position is not accessible 
    -- with a mounted agent.
    -- --------------------------------------------------------------------------------
    objective.initialeDestination = DEC_Geometrie_CopiePoint( objective:getPosition() )
    if not DEC_IsPointInUrbanBlockTrafficable( objective.initialeDestination )
       and not pointsInsideSameUrbanBlock( objective.initialeDestination, DEC_Agent_Position() ) then
        local simPositions = DEC_Geometrie_CalculerTrafficablePointPourPoint( objective.initialeDestination )
        objective.destination = DEC_Geometrie_CopiePoint(  getNearestSimPoint( objective.initialeDestination, simPositions ) )
    else
        objective.destination = objective.initialeDestination
    end
    

    -- -------------------------------------------------------------------------------- 
    -- Compute path and start moving along.
    -- --------------------------------------------------------------------------------
    local pathType = pathType or eTypeItiMouvement
    local it = DEC_CreerItineraireBM( objective.destination, pathType )
    F_Pion_SetitMvt( meKnowledge.source, it )
    objective[ myself ].moveAction = DEC_StartDeplacement( it )
    local moveAction = objective[ myself ].moveAction
    actionCallbacks[ objective[ myself ].moveAction ] = function( arg )
        objective[ myself ].etat = arg
    end

    return false
end


-- **************************************************************************** 
-- integration.updateMoveToIt
-- comments: 
-- make agent mount or dismount depending on terrain traficability
--
-- ****************************************************************************
integration.updateMoveToIt = function( objective, pathType )

    local etat = objective[ myself ].etat

    -- -------------------------------------------------------------------------------- 
    -- End of movement, check if objective is traficable
    -- --------------------------------------------------------------------------------    
    if etat == eEtatActionDeplacement_Termine then -- obective is reached or sub-objective if unit needs to dismount  
        if myself.canBeBlocked then
            myself.blocked = true
            myself.canBeBlocked = nil
            return false
        end
        if objective.initialeDestination ~= objective.destination -- moving toward a non-traficable urban block for instance
           and not myself.dismountedDone then
            -- Entering in a non-traficable element: dismount then re-compute path
            myself.enteringNonTrafficableElement = true
            
            -- Dismount if mounted, else compute a new path
            if not DEC_Agent_EstDebarque() then
                myself.dismount = myself.dismount or DEC_StartDebarquement()
            else
                if myself.dismount then
                    DEC__StopAction( myself.dismount )
                    myself.dismount = nil
                end
                DEC__StopAction( objective[ myself ].moveAction )
                myself.dismounted = true
                -- Once unit is dismounted, move to the real objective
                objective.destination = DEC_Geometrie_CopiePoint( objective:getPosition() )
                objective.initialeDestination = objective.destination -- for updateMoveToItArea
                local pathType = pathType or eTypeItiMouvement
                local it = DEC_CreerItineraireBM( objective.destination, pathType )
                F_Pion_SetitMvt( meKnowledge.source, it )
                local escort = F_Pion_GetpionEnEscorte( meKnowledge.source )
                if escort then -- Une unit� m'escorte, la mission MoveToward devient StayClose
                    local escortUnit = CreateKnowledge( sword.military.world.PlatoonAlly, escort )
                    escortUnit:sendNeedRoute( meKnowledge, true )
                    escortUnit:sendGetRouteForEscorting( meKnowledge, false )
                end
                objective[ myself ].moveAction = DEC_StartDeplacement( it )      
                objective[ myself ].etat = nil
                actionCallbacks[ objective[ myself ].moveAction ] = function( arg )
                    objective[ myself ].etat = arg
                end
            end
        
            return false
        end
        return true 

    -- -------------------------------------------------------------------------------- 
    -- Movement pause: other movement action is active or agent is mounting
    -- --------------------------------------------------------------------------------
    elseif etat == eEtatActionDeplacement_Pause then
        if myself.mount == nil then
            DEC_ReprendAction( objective[ myself ].moveAction )
        elseif DEC_Agent_EstEmbarque() then  -- agent is mounting
            DEC__StopAction( myself.mount )
            myself.mount = nil
            DEC_ReprendAction( objective[ myself ].moveAction )
        end

    -- -------------------------------------------------------------------------------- 
    -- Movement is running
    -- $$$ Add DEC_Agent_EstEmbarquable() ??? un pion � pied n'est pas "d�barqu�" si?
    -- --------------------------------------------------------------------------------
    elseif etat == eEtatActionDeplacement_EnCours then

        if objective.destination and DEC_Agent_TransporteursPret() then -- if unit has troop transporters
            -- If agent exit from a non traficable element toward a traficable destination, mount
            -- if no no order to stay dismountes has been isssued 
            if DEC_IsPointInUrbanBlockTrafficable( DEC_Agent_Position() ) 
               and DEC_IsPointInUrbanBlockTrafficable( objective.destination ) 
               and not myself.enteringNonTrafficableElement then
                if DEC_Agent_EstDebarque() and not myself.stayDismounted then
                    if myself.mount == nil then
                        myself.mount = DEC_StartEmbarquement()
                        DEC_PauseAction( objective[ myself ].moveAction )
                    end
                end
            end
        end

        -- $$$ Specific case for escort, mount if going outside non traficable element
        if objective.destinationEscort then 
             if DEC_IsPointInUrbanBlockTrafficable( DEC_Agent_Position() ) 
                and DEC_IsPointInUrbanBlockTrafficable( objective.destinationEscort ) then
                if DEC_Agent_EstDebarque() and not myself.stayDismounted then
                    if myself.mount == nil then
                        myself.mount = DEC_StartEmbarquement()
                        DEC_PauseAction( objective[ myself ].moveAction )
                    end
                end
            end
        end

    -- -------------------------------------------------------------------------------- 
    -- Other feedbacks
    -- --------------------------------------------------------------------------------    
    elseif etat == eEtatActionDeplacement_ManqueCarburant then
        DEC_PauseAction( objective[myself].moveAction )
    elseif etat == eEtatActionDeplacement_NonAutorise then
        if objective[myself].rcDone ~= etat then
            meKnowledge:RC( eRC_TerrainDifficile )
            objective[ myself ].rcDone = etat
            myself.canBeBlocked = true
        end
    elseif etat == eEtatActionDeplacement_CheminPartiel then
        myself.canBeBlocked = true
    elseif etat == eEtatActionDeplacement_DejaEnDeplacement then
        if etat ~= objective[ myself ].rcDone then
            DEC_Trace( "Already moving" )
            objective[ myself ].rcDone = etat
        end
    end

    return false
end


-- ****************************************************************************
-- integration.deselectMoveToIt
-- comments: pause movement
-- ****************************************************************************
integration.deselectMoveToIt = function( objective )
    if objective[ myself ].moveAction then -- Temp : Wait deselect to be done before destroy
        DEC_PauseAction( objective[ myself ].moveAction )
    end
end


-- ****************************************************************************
-- integration.stopMoveToIt
-- comments: stop movement
-- ****************************************************************************
integration.stopMoveToIt = function( objective )
    F_Pion_SetitMvt( meKnowledge.source, nil ) -- for escort
    objective[ myself ] = objective[ myself ] or {}
    if objective[ myself ].moveAction then
        DEC__StopAction( objective[ myself ].moveAction )
        objective[ myself ].moveAction = nil
        objective[ myself ].etat = nil
        objective[ myself ].rcDone = nil
        myself.dismountedDone = nil
        if myself.dismount then
            DEC__StopAction( myself.dismount )
            myself.dismount = nil
        end
        if myself.mount then
            DEC__StopAction( myself.mount )
            myself.mount = nil
        end
        myself.enteringNonTrafficableElement = nil
        return false
    end
end


-- ****************************************************************************
-- integration.startMoveToItArea
-- comments: used to move toward agent (objective.proxy is the agent)
-- only start/stop action exept for dismounting action
-- ****************************************************************************
integration.startMoveToItArea = function( objective, pathType )

    objective[ myself ] = objective[ myself ] or {}

    -- -------------------------------------------------------------------------------- 
    -- Leaving occupied position
    -- -------------------------------------------------------------------------------- 
    if myself.actionOccupy then
        myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    myself.location = nil
    
    -- -------------------------------------------------------------------------------- 
    -- Terrain management: reach a accessible position if objective is not accessible 
    -- with a mounted agent.
    -- --------------------------------------------------------------------------------
    objective.initialeDestination = DEC_Geometrie_CopiePoint( objective:getPosition() )
    if not DEC_IsPointInUrbanBlockTrafficable( objective.initialeDestination )
       and not pointsInsideSameUrbanBlock( objective.initialeDestination, DEC_Agent_Position() ) then
        local simPositions = DEC_Geometrie_CalculerTrafficablePointPourPoint( objective.initialeDestination )
        objective.destination = DEC_Geometrie_CopiePoint(  getNearestSimPoint( objective.initialeDestination, simPositions ) )
    else
        objective.destination = objective.initialeDestination
    end

    -- -------------------------------------------------------------------------------- 
    -- Compute path and start moving along.
    -- --------------------------------------------------------------------------------
    local pathType = pathType or eTypeItiMouvement
    local it = DEC_CreerItineraireBM( objective.destination, pathType )
    F_Pion_SetitMvt( meKnowledge.source, it )
    objective[ myself ] = {}
    objective[ myself ].moveAction = DEC_StartDeplacement( it )
    local moveAction = objective[ myself ].moveAction
    actionCallbacks[ objective[ myself ].moveAction ] = function( arg )
        objective[ myself ].etat = arg
    end

    return false
end


-- ****************************************************************************
-- integration.updateMoveToItArea
-- comments: used to move toward agent
-- ****************************************************************************
integration.updateMoveToItArea = function( objective, pathType )

    if objective.destination and objective.initialeDestination then
        -- -------------------------------------------------------------------------------- 
        -- Objective has moved: compute a new path if moving distance is greater than
        -- defined distanceMax.
        -- --------------------------------------------------------------------------------
        if objective[ myself ].etat == eEtatActionDeplacement_Termine then
            local distance = DEC_Geometrie_DistanceBetweenPoints( DEC_Agent_Position(), objective:getPosition() )
            if not DEC_Geometrie_PositionsEgales( objective.initialeDestination, objective:getPosition() ) 
                and distance > 0 and objective.destination == objective.initialeDestination then -- mean objective is accessible, no sub-objective to dismount
                integration.stopMoveToIt( objective )
                return integration.startMoveToItArea( objective, pathType )
           end
        end
    end
    return integration.updateMoveToIt( objective )
end

-- **************************************************************************** 
-- integration.startMoveToItItinerary
-- comments: 
-- make agent mount or dismount depending on terrain traficability
-- ****************************************************************************
integration.startMoveToItItinerary = function( objective )
    -- Leave tactical object
    if myself.actionOccupy then
        myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    objective[myself] = objective[myself] or {}
    if objective[myself].moveAction then
        DEC_ReprendAction( objective[myself].moveAction )
        return false 
    end
    myself.location = nil

    local it = objective.source -- itineraire calcul� dans la SIM par l'unit� que l'on escorte
    objective.destinationEscort = DEC_Itineraire_DernierPoint( objective.source )
    F_Pion_SetitMvt( meKnowledge.source, it )
    objective[ myself ].moveAction = DEC_StartDeplacement( it )
    local moveAction = objective[ myself ].moveAction
    actionCallbacks[ objective[ myself ].moveAction ] = function( arg )
    objective[ myself ].etat = arg end
    return false
end

integration.startMoveToItCrowd = function( objective )
    -- Leave tactical object
    if myself.actionOccupy then
    	myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    objective[myself] = objective[myself] or {}
    if objective[myself].moveAction then
        DEC_ReprendAction( objective[myself].moveAction )
        return false
    end
    
    myself.location = nil
    objective.destination = DEC_Geometrie_CopiePoint( objective:getPosition() )
    objective.initialPosition = DEC_Geometrie_CopiePoint( meKnowledge:getPosition() )
    objective[myself].moveAction = DEC__StartDeplacement( objective.destination )    
    return false
end


integration.updateMoveToItCrowd = function( objective, pathType )
    local reachableDestination = objective.destination
    if reachableDestination then
        local currentPosition = objective:getPosition()
        local epsilon = 10
        local distance = DEC_Geometrie_DistanceBetweenPoints( reachableDestination, currentPosition )
        if distance > epsilon then
            integration.stopMoveToItCrowd( objective )
            return integration.startMoveToItCrowd( objective, pathType )
        end
        objective.lastPosition = objective.lastPosition or DEC_Geometrie_CopiePoint( currentPosition )
        local currentDistance = DEC_Geometrie_DistanceBetweenPoints( objective.lastPosition, currentPosition )
        if( currentDistance > 3 * epsilon ) then
            objective.lastPosition = currentPosition
            integration.stopMoveToItCrowd( objective )
            return integration.startMoveToItCrowd( objective, pathType)
        end
    end
    if DEC_Population_HasReachedDestinationCompletely( objective:getPosition() ) then
        return true
    end
    return false
end


integration.stopMoveToItCrowd = function( objective )
    objective[myself] = objective[myself] or {}
    if objective[myself].moveAction then
        DEC__StopAction( objective[myself].moveAction )
        objective[myself].moveAction = nil
        objective.destination = nil
        return false
    end
end

integration.isUrbanBlockTrafficable = function( self, loaded )
    if not loaded then return 100 end
    if not self.isUrbanBlockTrafficableCache then
        self.isUrbanBlockTrafficableCache = ( DEC_ConnaissanceBlocUrbain_Traficabilite( self.source ) >= 0 and 100 ) or 0
    end
    return self.isUrbanBlockTrafficableCache
end



integration.moveToItCrowd = masalife.brain.integration.startStopAction( { 
        start = integration.startMoveToItCrowd,
        started = integration.updateMoveToItCrowd, 
        stop = integration.deselectMoveToIt } )

integration.moveToItItinerary = masalife.brain.integration.startStopAction( { 
        start = integration.startMoveToItItinerary,
        started = integration.updateMoveToIt, 
        stop = integration.deselectMoveToIt } )
        
integration.isPointInUrbanBlockTrafficableForProxy = function( self, loaded )
    if not loaded then return 100 end
    local pos = self:getPosition()
    return ( pos and DEC_IsPointInUrbanBlockTrafficable( pos ) and 100 ) or 0
end

integration.isPointInUrbanBlockTrafficable = function( self, loaded )
    if not loaded then return 100 end
    local isTrafficable = false
    if not self.isPointInUrbanBlockTrafficableCache then
        local pos = self:getPosition()
        if masalife.brain.core.class.isOfType( meKnowledge, sword.military.world.Company  ) then 
            local platoons = DEC_Automate_PionsAvecPC()
            for i = 1, #platoons do -- Est ce que ce point est trafficable pour tous les pions de l'automate
                if integration.isPointInUrbanBlockTrafficableForPlatoon( platoons[i], self.source ) then
                    isTrafficable = true
                end
            end
            self.isPointInUrbanBlockTrafficableCache = ( pos and isTrafficable and 100 ) or 0   
        else
          self.isPointInUrbanBlockTrafficableCache = ( pos and DEC_IsPointInUrbanBlockTrafficable( pos ) and 100 ) or 0
        end
    end
    return self.isPointInUrbanBlockTrafficableCache
end

integration.renfortMouvement = function( entity )
    DEC_Renforce( entity.source )
end

integration.annuleRenfortMouvement = function()
    DEC_AnnuleRenforcement()
end

integration.isPointInLocalisation = function( position, localisation )
    return DEC_Geometrie_EstPointDansLocalisation( position:getPosition(), localisation:getLocalisation() )
end

integration.isPointInUrbanBlockTrafficableForPlatoon = function( platoon, localisation )
    return DEC_IsPointInUrbanBlockTrafficableForPlatoon( platoon, localisation)
end
