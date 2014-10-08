require "debug"

--Reachable Implementation
--TODO use map to convert string to int/ or other  move function approach
integration.getPlatoonPosition = function( platoon )
    if integration.isKnowledgeAgentValid( platoon.source ) then
        return integration.getKnowledgeAgentPosition( platoon.source )
    else
        return nil
    end
end

integration.getKnowledgeAgentPosition = function( agent )
    return DEC_ConnaissanceAgent_Position( agent )
end

integration.getTeammatePosition = function( platoon )
    return DEC_Agent_PositionPtr( platoon.source )
end

integration.getBodyPosition = function()
    return DEC_Agent_Position()
end

integration.getRessourceNodePosition = function( resourceNode )
    return DEC_ReseauRessource_Position( resourceNode.source )
end


-- Ally
integration.getPlatoonAllyPosition = function( platoon )
  if platoon == myself then -- @TODO MGD replace by shared_ptr< DEC_Knowledge_Agent_ABC > and proxy for allies
    return DEC_Agent_Position()
  else
    return DEC_Agent_PositionPtr( platoon.source )
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
    if integration.isPointInUrbanBlockTrafficable(CreateKnowledge( integration.ontology.types.point, area.getAreaPositionResult), true ) == 0 then
        return integration.getAreaPositions( area )[1]
    end
    return area.getAreaPositionResult
end
integration.getCentralAreaPosition = function( area )
    area.getCentralAreaPositionResult = area.getCentralAreaPositionResult or DEC_Geometrie_CalculerBarycentreLocalisation( area.source )
    return area.getCentralAreaPositionResult
end
integration.getAreaPerimeterPositions = function( area )
    return DEC_Geometrie_ListePointsLocalisation( area.source )
end

integration.getUrbanBlockPosition = function( urbanBlock )
  urbanBlock.getUrbanBlockPosition = urbanBlock.getUrbanBlockPosition or DEC_ConnaissanceUrbanBlock_BarycentreDansBU( urbanBlock.source )
  return urbanBlock.getUrbanBlockPosition
end

integration.getUrbanBlockPositions = function( urbanBlock )
  urbanBlock.getUrbanBlockPositionsResult = urbanBlock.getUrbanBlockPositionsResult or DEC_Geometrie_CalculerLocalisationsBU( urbanBlock.source )
  return urbanBlock.getUrbanBlockPositionsResult
end

integration.getUrbanBlockNearestBorder = function( position, urbanBlock )
    urbanBlock.getUrbanBlockNearestBorderResult = urbanBlock.getUrbanBlockNearestBorderResult or DEC_Geometrie_ComputeNearestBorder( position, urbanBlock:getLocalisation() )
    return urbanBlock.getUrbanBlockNearestBorderResult
end

integration.getAreaPositions = function( area )
    area.getAreaPositionsResult = area.getAreaPositionsResult or DEC_Geometrie_CalculerTrafficablePointPourPoint( integration.getCentralAreaPosition( area ) )
    area.getTrafficableAreaPositionsResult = area.getTrafficableAreaPositionsResult or {}
    if #area.getTrafficableAreaPositionsResult == 0 then
        local integration = integration
        local CreateKnowledge = CreateKnowledge
        for i = 1, #area.getAreaPositionsResult do -- On ne veut que les positions qui sont dans la zone
            if integration.isPointInLocalisation( CreateKnowledge( integration.ontology.types.point, area.getAreaPositionsResult[i] ), area ) then
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

integration.proximity = function( pos1, pos2 ) -- $$$ MIA security. A mettre en commun avec integration.normalizedInversedDistance
    local pos1Pos = pos1:getPosition()
    local pos2Pos = pos2:getPosition()
    if not pos1Pos or not pos2Pos then
        return 1
    end 
	local distanceMax = DEC_Detection_Distance and DEC_Detection_Distance() or 4000
    return LinearInterpolation( 1, 100, 10, distanceMax, false, integration.distance( pos1, pos2 ) )
end

local normalizedInverseDistanceSimSim = function( simPos1, simPos2, distanceMax )
    if not simPos1 or not simPos2 then
        return 1
    end
    return LinearInterpolation( 1, 100, 10, distanceMax, false, DEC_Geometrie_DistanceBetweenPoints( simPos1, simPos2 ) )
end

local normalizedInverseDistanceSimListSim = function( simList, simPos, distanceMax )
    local distance = 1
    if not simList then return 1 end
    for _, simPos2 in pairs( simList ) do
          local currentDistance = normalizedInverseDistanceSimSim( simPos2, simPos, distanceMax )
          if distance < currentDistance then
              distance = currentDistance
          end
    end
    return distance

end

local listLocalisationPoints = function( pos )
    local localisationPoints = DEC_Geometrie_ListePointsLocalisation( pos:getLocalisation() )
    localisationPoints[ #localisationPoints + 1 ] = pos:getPosition()
    return localisationPoints
end

local normalizedInverseDistanceSim = function( simPos, pos2, distanceMax )
    if not simPos or not pos2 then return 1 end
    if pos2.getLocalisation and pos2:getLocalisation() then 
        return normalizedInverseDistanceSimListSim( listLocalisationPoints( pos2 ), simPos, distanceMax )
    end
    return normalizedInverseDistanceSimSim( simPos, pos2:getPosition(), distanceMax )
end

local normalizedInverseDistanceSimList = function( posList, pos2, distanceMax )
    if not posList or not pos2 then
        return 1
    end
    local distance = 1
    for _, simPos in pairs( posList ) do
          local currentDistance = normalizedInverseDistanceSim( simPos, pos2, distanceMax )
          if distance < currentDistance then
              distance = currentDistance
          end
    end
    return distance
end

-- Return 1 if positions are far, 100 if they are near. 'Near/far' is defined according to detection distance.
integration.normalizedInversedDistance = function( pos1, pos2, distanceMaxGiven )
    if not pos1 or not pos2 then
        return 1
    end
    local distanceMax = distanceMaxGiven or 100000 -- hard coded : same order of scale for an operational theater ; distanceMaxGiven for scipio compatibility
    if pos1.getLocalisation and pos1:getLocalisation() then
        return normalizedInverseDistanceSimList( listLocalisationPoints( pos1 ), pos2, distanceMax )
    end
    if pos2.getLocalisation and pos2:getLocalisation() then
        return normalizedInverseDistanceSimList( listLocalisationPoints( pos2 ), pos1, distanceMax )
    end
    return normalizedInverseDistanceSimSim ( pos1:getPosition(), pos2:getPosition(), distanceMax )
end

-- ============================================================================
-- MOVEMENT MANAGEMENT
-- Manage Mount and dismount terrain constraints / speed of movement etc.
-- ============================================================================

-- ****************************************************************************
-- Speed of movement modulation for MILITARY agent
-- ****************************************************************************
integration.setMovementPace = function( modulation, maxSpeed ) -- maxSpeed TRUE/FALSE 

    local myself = myself
    local integration = integration
    -- -------------------------------------------------------------------------------- 
    -- Urgency, moves at maximum.
    -- --------------------------------------------------------------------------------
    if maxSpeed then
        DEC_ModulationVitesseMax( 1 )
        return
    end

    -- -------------------------------------------------------------------------------- 
    -- Moderate the speed of movement
    -- --------------------------------------------------------------------------------
    myself.speedModulations = myself.speedModulations or {}
    local currentModulation = modulation or 1

    -- -------------------------------------------------------------------------------- 
    -- Terrain modulation
    -- --------------------------------------------------------------------------------
    if integration.isAgentInsideTown() and not integration.isFlying() then -- $$$ MIA TODO integration.isFlying à virer qd on aura fait un moveto pour les flying agent.
        currentModulation = currentModulation / 2
    end

    -- -------------------------------------------------------------------------------- 
    -- Safety vs. cover vs. normal modulation
    -- --------------------------------------------------------------------------------
    if myself.safetyMode then
        myself.speedModulations.safetyModulation = currentModulation / 2
    else
        myself.speedModulations.safetyModulation = currentModulation
    end

    -- integration.getForceRatio: 0 means very good, 1 very bad
    if myself.coverMode then
        myself.speedModulations.coverModulation = math.min( currentModulation / 2, 
                                                            integration.getForceRatio() )
    else
        myself.speedModulations.coverModulation = currentModulation
    end

    if myself.slowDown then
        myself.speedModulations.slowDownModulation = 0.01
    else
        myself.speedModulations.slowDownModulation = currentModulation
    end

    -- Determine global pace
    for _, modulation in pairs( myself.speedModulations ) do
        currentModulation = math.min( currentModulation , modulation )
    end
    DEC_ModulationVitesseMax( math.max( currentModulation, 0.01 ) )
end

-- -----------------------------------------------------------------------------
-- Determine the pace of a security agent
-- -----------------------------------------------------------------------------
integration.setPace = function( urgency ) -- urgence TRUE/FALSE
    local currentModulation = 0.5
    if urgency then
        currentModulation = 1
    end
    if integration.isAgentInsideTown() then
        integration.speedMaxModulation( currentModulation / 2 )
    else
        integration.speedMaxModulation( currentModulation )
    end
end

-- Modify the max speed by the given modulation (between 0 and 1)
-- @param modulation Real the max speed modulation (between 0 and 1)
integration.speedMaxModulation = function( modulation )
    DEC_ModulationVitesseMax( modulation )
end

-- Modify the current speed by the given modulation (between 0 and 1))
-- @param modulation Real the current speed modulation (between 0 and 1)
integration.speedCurrentModulation = function( modulation )
    DEC_ModulationVitesseCourante( modulation )
end

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
    start = function( entity, pathType, waypoints )
        return integration.startMoveToIt( entity, pathType, waypoints ) 
    end,
    started = function( entity, pathType, waypoints )
        return integration.updateMoveToIt( entity, pathType, waypoints )
    end, 
    stop = function( entity, pathType, waypoints )
        if waypoints and #waypoints > 0 then
            return integration.deselectMoveToIt( entity )
        else
            return integration.stopMoveToIt( entity )
        end
    end,
} )

-- ****************************************************************************
-- integration.startMoveToIt 
--  mount dismount agent capabilities
--  pathfinding
--  movement along computed path
-- ****************************************************************************
integration.startMoveToIt = function( objective, pathType, waypoints )
    myself.blocked = nil
    objective[ myself ] = objective[ myself ] or {}

    -- -------------------------------------------------------------------------------- 
    -- Leaving occupied position
    -- -------------------------------------------------------------------------------- 
    if myself.actionOccupy then
      myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    myself.location = nil

    -- -------------------------------------------------------------------------------- 
    --if the agent is ordered to move and if it is deployed, undeploy before starting 
    -- to move. 
    -- -------------------------------------------------------------------------------- 
    if integration.isDeployed() then
        integration.undeploy() -- call once, but deployment takes delays.
    end

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
    if DEC_ObjectKnowledge_IsPositionInside(meKnowledge.source,"contamination", objective:getPosition()) then
        integration.equipNBCOutfit()
    end
    objective.initialeDestination = DEC_Geometrie_CopiePoint( objective:getPosition() )
    if ( not DEC_IsPointInUrbanBlockTrafficable( objective.initialeDestination ) or DEC_IsPointInDestroyedUrbanBlock( objective.initialeDestination ) )
        and not pointsInsideSameUrbanBlock( objective.initialeDestination, DEC_Agent_Position() ) 
        and not DEC_Agent_PionCanFly( myself ) then
        local simPositions = DEC_Geometrie_CalculerTrafficablePointPourPoint( objective.initialeDestination )
        objective.destination = DEC_Geometrie_CopiePoint(  getNearestSimPoint( objective.initialeDestination, simPositions ) )
    else
        objective.destination = objective.initialeDestination
    end
    

    -- -------------------------------------------------------------------------------- 
    -- Compute path and start moving along.
    -- --------------------------------------------------------------------------------
    if pathType == nil or pathType == NIL then -- pathType can have the MASA Life 'NIL' value
        pathType = eTypeItiMouvement
    end

    local itinerary
    local simWaypoints = {}
    if waypoints ~= nil then
        for i = 1, #waypoints do
            simWaypoints[ i ] = waypoints[ i ]:getPosition()
        end
    end
    if #simWaypoints > 0 then
        simWaypoints[ #simWaypoints + 1 ] = objective.destination
        myself.movingOnPath = true
        itinerary = DEC_CreerItineraireListe( simWaypoints, pathType )
    else
        itinerary = DEC_CreerItineraireBM( objective.destination, pathType )
    end

    F_Pion_SetitMvt( meKnowledge.source, itinerary )
    objective[ myself ].moveAction = DEC_StartDeplacement( itinerary )
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
integration.updateMoveToIt = function( objective, pathType, waypoints )
    local etat = objective[ myself ].etat
    local integration = integration
    local myself = myself
    local meKnowledge = meKnowledge

    if not objective[ myself ].moveAction then
        integration.startMoveToIt( objective, pathType, waypoints )
    end

    -- --------------------------------------------------------------------------------
    -- End of movement, check if objective is traficable
    -- --------------------------------------------------------------------------------
    if etat == eEtatActionDeplacement_Termine then -- obective is reached or sub-objective if unit needs to dismount
        -- Specific case if agent is blocked by an obstacle
        if myself.canBeBlocked then
            reportFunction( eRC_DestinationNonReachable )
            myself.blocked = true
            myself.canBeBlocked = nil
            return false
        end
        if objective.initialeDestination ~= objective.destination -- moving toward a non-traficable urban block for instance
           and not myself.dismountedDone
           and integration.canDismount()
           and not DEC_IsPointInDestroyedUrbanBlock( objective.initialeDestination ) then

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
                if pathType == NIL  or pathType == nil then -- pathType can have the MASA Life 'NIL' value
                    pathType = eTypeItiMouvement
                end
                local it = DEC_CreerItineraireBM( objective.destination, pathType )
                F_Pion_SetitMvt( myself, it )
                local escort = integration.getAgentEscort( myself )
                if escort then -- Une unite m'escorte, la mission MoveToward devient StayClose
                    local escortUnit = CreateKnowledge( integration.ontology.types.agent, escort )
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
    -- $$$ Add DEC_Agent_EstEmbarquable() ??? un pion à pied n'est pas "débarqué" si?
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
            reportFunction(eRC_TerrainDifficile )
            objective[ myself ].rcDone = etat
            myself.canBeBlocked = true
            DEC_Trace( " ** IMPOSSIBLE TO COMPUTE A PATH TO DESTINATION ** " )
        end
    elseif etat == eEtatActionDeplacement_CheminPartiel then
        myself.canBeBlocked = true
    elseif etat == eEtatActionDeplacement_BloqueParObjet then
        if not myself.canBeBlocked then
            reportFunction( eRC_Bloquee )
        end
        myself.canBeBlocked = true
    elseif etat == eEtatActionDeplacement_DejaEnDeplacement then
        if etat ~= objective[ myself ].rcDone then
            DEC_Trace( "Already moving" )
            objective[ myself ].rcDone = etat
        end
    elseif etat == eEtatActionDeplacement_Teleporte and not myself.movingOnPath then
        DEC__StopAction( objective[ myself ].moveAction )
        integration.stopMoveToIt( objective )
        integration.startMoveToIt( objective, pathType )
    end

    return false
end


-- ****************************************************************************
-- integration.deselectMoveToIt
-- comments: pause movement
-- ****************************************************************************
integration.deselectMoveToIt = function( objective )
    local myObjective = objective[ myself ]
    if myObjective and myObjective.moveAction then -- Temp : Wait deselect to be done before destroy
        DEC_PauseAction( myObjective.moveAction )
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
         myself.movingOnPath = false
        return false
    end
end


-- ****************************************************************************
-- integration.startMoveToItArea
-- comments: used to move toward agent (objective.proxy is the agent)
-- only start/stop action exept for dismounting action
-- ****************************************************************************
integration.startMoveToItArea = function(  objective, pathType, waypoints )

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
       and not pointsInsideSameUrbanBlock( objective.initialeDestination, DEC_Agent_Position() )
       and not DEC_Agent_PionCanFly( myself ) then
        local simPositions = DEC_Geometrie_CalculerTrafficablePointPourPoint( objective.initialeDestination )
        objective.destination = DEC_Geometrie_CopiePoint(  getNearestSimPoint( objective.initialeDestination, simPositions ) )
    else
        objective.destination = objective.initialeDestination
    end

    -- -------------------------------------------------------------------------------- 
    -- Compute path and start moving along.
    -- --------------------------------------------------------------------------------
    if pathType == NIL  or pathType == nil then -- pathType can have the MASA Life 'NIL' value 
        pathType = eTypeItiMouvement
    end
    local itinerary
    local simWaypoints = {}
    if waypoints ~= nil then
        for i = 1, #waypoints do
            simWaypoints[ i ] = waypoints[ i ]:getPosition()
        end
    end
    if #simWaypoints > 0 then
        simWaypoints[ #simWaypoints + 1 ] = objective.destination
        myself.movingOnPath = true
        itinerary = DEC_CreerItineraireListe( simWaypoints, pathType )
    else
        itinerary = DEC_CreerItineraireBM( objective.destination, pathType )
    end
    F_Pion_SetitMvt( meKnowledge.source, it )
    objective[ myself ] = {}
    objective[ myself ].moveAction = DEC_StartDeplacement( itinerary )
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

            if DEC_Agent_PionCanFly( myself ) then -- simple case for flying agent.
                if distance > 0  then
                    integration.stopMoveToIt( objective )
                    return integration.startMoveToItArea( objective, pathType )
                end
            end
            if not DEC_Geometrie_PositionsEgales( objective.initialeDestination, objective:getPosition() ) -- specific case, itinerary computed on my own poition.
            and distance > 0 and objective.destination == objective.initialeDestination then -- mean objective is accessible, no sub-objective to dismount
                integration.stopMoveToIt( objective )
                return integration.startMoveToItArea( objective, pathType )
           end
        else
            -- If the agent has not reached its objective yet, and the followed unit has covered a distance greater
            -- than the distance that the agent can cover in a fixed short amount of time (defined by the
            -- delayInSeconds constant), then a new objective should be computed. This is done to ensure
            -- a better following behaviour.
            
            local distance = DEC_Geometrie_DistanceBetweenPoints( objective.initialeDestination, objective:getPosition() )
            local delayInSeconds = 40
            
            if distance > integration.getMaxSpeed( meKnowledge )*delayInSeconds then
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
    myself.blocked = nil
    if myself.actionOccupy then
        myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    objective[myself] = objective[myself] or {}
    if objective[myself].moveAction then
        DEC_ReprendAction( objective[myself].moveAction )
        return false 
    end
    myself.location = nil

    local it = objective.source -- itineraire calculé dans la SIM par l'unité que l'on escorte
    objective.destinationEscort = DEC_Itineraire_DernierPoint( objective.source )
    F_Pion_SetitMvt( meKnowledge.source, it )
    objective[ myself ].moveAction = DEC_StartDeplacement( it )
    local moveAction = objective[ myself ].moveAction
    actionCallbacks[ objective[ myself ].moveAction ] = function( arg )
    objective[ myself ].etat = arg end
    return false
end

-- **************************************************************************** 
-- integration.computePath
-- comments: ask the simulation to compute a path to the given destination
-- Returns 'true' when computing is over.
-- ****************************************************************************
integration.computePath = function( objective, pathType )
    objective[ myself ] = objective[ myself ] or {}
        
    objective.initialeDestination = DEC_Geometrie_CopiePoint( objective:getPosition() )
    if not DEC_IsPointInUrbanBlockTrafficable( objective.initialeDestination )
       and not pointsInsideSameUrbanBlock( objective.initialeDestination, DEC_Agent_Position() ) then
        local simPositions = DEC_Geometrie_CalculerTrafficablePointPourPoint( objective.initialeDestination )
        objective.destination = DEC_Geometrie_CopiePoint(  getNearestSimPoint( objective.initialeDestination, simPositions ) )
    else
        objective.destination = objective.initialeDestination
    end
     
    if not objective[ myself ].it then
        objective[ myself ].it = DEC_CreerItineraireBM( objective.destination, pathType )
        objective[ myself ].moveAction = DEC_StartMovementSuspended( objective[ myself ].it )
    end
    local pathCompute
    if objective[ myself ].moveAction then
        pathCompute = getEtatAction( objective[ myself ].moveAction )
    end
    if pathCompute == 1 then
        return true
    end
    return false
end

-- **************************************************************************** 
-- CROWD AGENT MOVEMENT
-- integration.startMoveToItCrowd
-- ****************************************************************************
integration.startMoveToItCrowd = function( objective )
    -- Leave tactical object
    if myself.actionOccupy then
    	myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    objective[ myself ] = objective[myself] or {}
    if objective[ myself ].moveAction then
        DEC_ReprendAction( objective[ myself ].moveAction )
        return false
    end
    
    myself.location = nil
    objective.destination = DEC_Geometrie_CopiePoint( objective:getPosition() )
    objective.initialPosition = DEC_Geometrie_CopiePoint( meKnowledge:getPosition() )
    objective[ myself ].moveAction = DEC__StartDeplacement( objective.destination )    
    return false
end


-- **************************************************************************** 
-- CROWD AGENT MOVEMENT
-- integration.updateMoveToItCrowd
-- ****************************************************************************
integration.updateMoveToItCrowd = function( objective, pathType, inertness )

    local epsilon = 0
    if not inertness then
        -- Scipio Crowds
        epsilon = 10
    end
    local reachableDestination = objective.destination
    if reachableDestination then
        local currentPosition = objective:getPosition()
        local distance = DEC_Geometrie_DistanceBetweenPoints( reachableDestination, currentPosition )
        local distanceDestination = DEC_Geometrie_DistanceBetweenPoints( reachableDestination, objective.initialPosition )
        if distanceDestination <= 10 then -- LMT : HardCode distance in MIL_PopulationConcentration::IsNearPosition function
            return true
        end
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
        return true -- the all crowd has reached the objective
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

-- **************************************************************************** 
-- CROWD AGENT MOVEMENT
-- integration.startMoveToListPointCrowd
-- ****************************************************************************
integration.startMoveToListPointCrowd = function( objective, pathType, wayPoints )
    -- Leave tactical object
    if myself.actionOccupy then
      myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    objective[ myself ] = objective[myself] or {}
    if objective[ myself ].moveAction then
        DEC_ReprendAction( objective[ myself ].moveAction )
        return false
    end

    myself.location = nil
    local itinerary = {}
    for i=1,#wayPoints do
        itinerary[ i ] = wayPoints[ i ]:getPosition()
    end
    itinerary[ #itinerary + 1 ] = objective:getPosition()
    objective[ myself ].moveAction = DEC_StartDeplacementItineraire( itinerary )
    return false
end

-- **************************************************************************** 
-- CROWD AGENT MOVEMENT
-- integration.updateMoveToListPointCrowd
-- ****************************************************************************
integration.updateMoveToListPointCrowd = function( objective, pathType, wayPoints )
end

integration.isUrbanBlockTrafficable = function( self, loaded )
    if integration.getUrbanBlockState( self ) == 0 then return 0 end
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
        
integration.isPointInUrbanBlockTrafficableForProxy = function( location, loaded )
    local pos = location:getPosition()
    if DEC_IsPointInDestroyedUrbanBlock( pos ) then return 0 end
    if not loaded then return 100 end
    local pos = location:getPosition()
    return ( pos and DEC_IsPointInUrbanBlockTrafficable( pos ) and 100 ) or 0
end

--- Informs about the accessibility of a given location.
-- @param location DirectIA knowledge
-- @param loaded Boolean The agent state used to access the location (loaded:on board, unloaded:off board)
-- @return Integer, the accessibility level from 0 to 100, 0 meaning the location is not accessible at all.
integration.isPointInUrbanBlockTrafficable = function( location, loaded )
    if not location or not location.getPosition then
        return loaded and 0 or 100
    end
    local pos = location:getPosition()
    if DEC_IsPointInDestroyedUrbanBlock( pos ) then return 0 end
    if not loaded then return 100 end
    local isTrafficable = false
    if not location.isPointInUrbanBlockTrafficableCache then
        if masalife.brain.core.class.isOfType( meKnowledge, integration.ontology.types.automat ) then 
            local platoons = DEC_Automate_PionsAvecPC()
            for i = 1, #platoons do
                if integration.isPointInUrbanBlockTrafficableForPlatoon( platoons[i], location.source ) then
                    isTrafficable = true
                end
            end
            location.isPointInUrbanBlockTrafficableCache = ( pos and isTrafficable and 100 ) or 0
        else
          location.isPointInUrbanBlockTrafficableCache = ( pos and DEC_IsPointInUrbanBlockTrafficable( pos ) and 100 ) or 0
        end
    end
    return location.isPointInUrbanBlockTrafficableCache
end

--- Informs if the given position is located inside a destroyed urban block.
-- @param simPosition Simulation point 
-- @return Boolean, true if the position  is located inside a destroyed urban block
integration.isPointInDestroyedUrbanBlock = function( simPosition )
    return DEC_IsPointInDestroyedUrbanBlock( simPosition )
end

integration.renfortMouvement = function( entity )
    DEC_Renforce( entity.source )
end

integration.annuleRenfortMouvement = function()
    DEC_AnnuleRenforcement()
end

integration.isPointInLocalisation = function( position, localisation )
    return integration.isPointInSimLocalisation( position, localisation:getLocalisation() )
end

integration.isPointInSimLocalisation = function( position, localisation )
    return DEC_Geometrie_EstPointDansLocalisation( position:getPosition(), localisation )
end

integration.isPointTrafficable = function( self )
    if not self.isPointInUrbanBlockTrafficableCache then
        local pos = self:getPosition()
        self.isPointInUrbanBlockTrafficableCache = ( pos and DEC_IsPointInUrbanBlockTrafficable( pos ) )
    end
    return self.isPointInUrbanBlockTrafficableCache
end

integration.getUnitPositions = function( simPoint )
    return DEC_Geometrie_CalculerTrafficablePointPourPoint( simPoint )
end

integration.isPointInUrbanBlockTrafficableForPlatoon = function( platoon, localisation )
    return DEC_IsPointInUrbanBlockTrafficableForPlatoon( platoon, localisation)
end

integration.getCrossroads = function()
    local crossroads = {}
    DEC_Crossroads( crossroads )
    return crossroads
end

integration.creerItineraireAPartirListePoint = function( listPoint )
    local listPointSource = {}
    for i=1,#listPoint do 
        listPointSource[#listPointSource + 1] = listPoint[i].source
    end
    return CreateKnowledge( integration.ontology.types.itinerary, DEC_CreerItineraireListe(listPointSource, eTypeItiMouvement) )
end

integration.isElementInAOR = function( self )
    return DEC_Geometrie_EstPointDansFuseau( self:getPosition() )
end

integration.copyPoint = function( position )
    return DEC_Geometrie_CopiePoint( position )
end

integration.getPointsCategory = function()
    return DEC_GetPointsCategory()
end

integration.isBeforePoint = function( position )
    return DEC_IsAvantPoint( position)
end

integration.getTypePoint = function( position )
    return DEC_GetTypePoint( position)
end

integration.getDestinationPoint = function( position )
    return DEC_GetDestPoint( position )
end

-- Returns the geometry of a pathPoint (a simulation point) allowing 
-- the decisional models to create a ML knowledge of such a pathPoint
integration.getSimPointFromPathPoint = function( pathPoint )
    return DEC_GetRepPoint( pathPoint )
end

--- Returns a list of safety positions defined as :
-- <ul> <li> all crossroads less than the given radius away from this agent </li>
-- <li> positions along roads at the given safety distance of the previously found crossroads </li> </ul>
-- This method can only be called by an agent.
-- @see integration.findSafetyPositions
-- @param radius Float, the radius around the entity (in meters)
-- @param safetyDistance Float, the safety distance around the crossroads (in meters)
-- @return List of simulation point
integration.findSafetyPositions = function( radius, safetyDistance )
    return DEC_FindSafetyPositions( radius, safetyDistance )
end

--- Returns a list of safety positions defined as :
-- <ul> <li> all crossroads less than the given radius away from this agent </li>
-- <li> positions along roads at the given safety distance of the previously found crossroads </li> </ul>
-- This method can only be called by an agent.
-- @see integration.findSafetyPositions
-- @param radius Float, the radius around the entity (in meters)
-- @param safetyDistance Float, the safety distance around the crossroads (in meters)
-- @return List of point knowledges
function integration.getSafetyPositions( radius, safetyDistance )
    -- Warning: If this method is called with different parameters, the simulation cache will be invalidated so
    -- it's good practice not to call it with different parameters for the same brain.
    local points = integration.findSafetyPositions( radius, safetyDistance )
    local result = {}
    local CreateKnowledge = CreateKnowledge
    for i = 1, #points do
        result[ i ] = CreateKnowledge( integration.ontology.types.point, points[ i ] )
    end
    return result
end

-- Returns the time (minutes) needed to move towards the
-- provided position with a straight line itinerary
-- @params position : the position with respect to which the time will be computed
integration.getAgentTimeToReachPosition = function( position )
    return DEC_Agent_TempsPourParcourirDistanceEnLigneDroite( DEC_Geometrie_Distance( meKnowledge:getPosition(), position:getPosition() ) )
end

-- Returns the time (minutes) during which the agent
-- can remain autonomous while moving
integration.getAgentMovementAutonomy = function()
    return DEC_Agent_AutonomieEnDeplacement()
end

-- Return the list without element from list
removeListElementWithSameLocation = function( position, list )
    local result = {}
    for i, elemListReturn in pairs( list ) do
        if position ~= elemListReturn:getPosition() then
            result[#result+1] = elemListReturn
        end
    end
    return result
end
