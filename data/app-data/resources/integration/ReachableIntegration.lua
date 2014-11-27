require "debug"

--- Returns the given agent knowledge's position.
-- Returns nil if the knowledge is not valid.
-- @param agent Simulation agent knowledge
-- @return Simulation position (or nil if the given knowledge is invalid)
integration.getKnowledgeAgentPosition = function( agent )
    return DEC_ConnaissanceAgent_Position( agent )
end

--- Returns the given agent's position.
-- @param platoon DirectIA agent
-- @return Simulation position
integration.getTeammatePosition = function( platoon )
    return DEC_Agent_PositionPtr( platoon.source )
end

--- Returns this entity's position.
-- This method can only be called by an agent.
-- @return Simulation position
integration.getBodyPosition = function()
    return DEC_Agent_Position()
end

--- Returns the given resource node's position.
-- @param resourceNode Resource node knowledge
-- @return Simulation position
integration.getRessourceNodePosition = function( resourceNode )
    return DEC_ReseauRessource_Position( resourceNode.source )
end

--- Returns the simulation position associated with the given position knowledge.
-- @param point Position knowledge
-- @return Simulation position
integration.getPointPosition = function( point )
    return point.source
end

--- Returns the last waypoint of the given path.
-- @param itinerary Path knowledge
-- @return Simulation position
integration.getItineraryPosition = function( itinerary )
    return DEC_Itineraire_DernierPoint( itinerary.source )
end

--- Returns the given object's position.
-- If the object is not valid, this method will return nil.
-- If this method was previously called on the given object and a position was returned,
-- then the same position will be returned, regardless of the current validity of the object.
-- If the object is surface-based, this method will return its barycenter.
-- @param object Object knowledge
-- @return Simulation position
integration.getObjectPosition = function( object )
    object.getObjectPositionResult = object.getObjectPositionResult  or ( DEC_IsValidKnowledgeObject( object.source ) and DEC_Geometrie_CalculerBarycentreLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) ) ) or nil
    return object.getObjectPositionResult
end

--- Returns a list of positions at the border of the given object.
-- If the object is not valid, this method will return an empty table.
-- If this method was previously called on the given object, then the
-- same table will be returned, regardless of the current validity of the object.
-- @param object Object knowledge
-- @return List of simulation positions
integration.getObjectPositions = function( object )
    object.getObjectPositionsResult = object.getObjectPositionsResult  or ( DEC_IsValidKnowledgeObject( object.source ) and DEC_Geometrie_ListePointsLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) ) ) or {}
    return object.getObjectPositionsResult
end

--- Returns the barycenter of the given planned object.
-- If this method was previously called on the given planned object,
-- then the same position will be returned,
-- regardless of the current validity of the planned object.
-- @param plannedObject Planned object knowledge
-- @return Simulation position
integration.getEngineerObjectPosition = function( plannedObject )
    plannedObject.getEngineerObjectPositionResult = plannedObject.getEngineerObjectPositionResult or DEC_Geometrie_CalculerBarycentreLocalisation( DEC_GenObject_Localisation( plannedObject.source ) )
    return plannedObject.getEngineerObjectPositionResult 
end

--- Returns the barycenter of the given area, unless the barycenter is in a non-trafficable urban block,
-- in which case the method returns a position inside the urban block instead.
-- @param area Area knowledge
-- @return Simulation position
integration.getAreaPosition = function( area )
    area.getAreaPositionResult = integration.getCentralAreaPosition( area )
    if integration.isPointInUrbanBlockTrafficable( CreateKnowledge( integration.ontology.types.point, area.getAreaPositionResult ), true ) == 0 then
        return integration.getAreaPositions( area )[1]
    end
    return area.getAreaPositionResult
end

--- Returns the barycenter of the given area.
-- Note that the barycenter may be outside of the area
-- @param area Simulation area
-- @return Simulation position
integration.getCentralAreaPosition = function( area )
    area.getCentralAreaPositionResult = area.getCentralAreaPositionResult or DEC_Geometrie_CalculerBarycentreLocalisation( area.source )
    return area.getCentralAreaPositionResult
end

--- Returns the contour of the given area.
-- If the given area is a single point, this method returns
-- a list with that point only.
-- If the given area is a polyline, this method returns it.
-- @param area Area knowledge
-- @return List of simulation positions
integration.getAreaPerimeterPositions = function( area )
    return DEC_Geometrie_ListePointsLocalisation( area.source )
end

--- Returns the barycenter of the given urban block.
-- @param urbanBlock Urban block knowledge
-- @return Simulation position
integration.getUrbanBlockPosition = function( urbanBlock )
    urbanBlock.getUrbanBlockPosition = urbanBlock.getUrbanBlockPosition or DEC_ConnaissanceUrbanBlock_BarycentreDansBU( urbanBlock.source )
    return urbanBlock.getUrbanBlockPosition
end

--- Returns positions the given urban block (positions at the edges + the urban block's barycenter).
-- @param urbanBlock Urban block knowledge
-- @return List of simulation positions
integration.getUrbanBlockPositions = function( urbanBlock )
  urbanBlock.getUrbanBlockPositionsResult = urbanBlock.getUrbanBlockPositionsResult or DEC_Geometrie_CalculerLocalisationsBU( urbanBlock.source )
  return urbanBlock.getUrbanBlockPositionsResult
end

--- Returns the nearest point to the provided position on the border of the urban block.
-- @param position Simulation position
-- @param urbanBlock Urban block knowledge
-- @return Simulation position
integration.getUrbanBlockNearestBorder = function( position, urbanBlock )
    urbanBlock.getUrbanBlockNearestBorderResult = urbanBlock.getUrbanBlockNearestBorderResult or {}
    urbanBlock.getUrbanBlockNearestBorderResult[ position ] = urbanBlock.getUrbanBlockNearestBorderResult[ position ] or DEC_Geometrie_ComputeNearestBorder( position, urbanBlock:getLocalisation() )
    return urbanBlock.getUrbanBlockNearestBorderResult[ position ]
end

--- Returns a list of trafficable positions :
-- <ul> <li> If the given area's barycenter is in an urban block, this method will return
-- positions in the urban block that are located inside the given area (if there are no
-- such positions, this method will return positions in the urban block) </li>
-- <li> Otherwise, this method will return a list with the barycenter of the given area as its sole element</li> </ul>
-- @param area Area knowledge
-- @return List of simulation positions
integration.getAreaPositions = function( area )
    area.getAreaPositionsResult = area.getAreaPositionsResult or DEC_Geometrie_CalculerTrafficablePointPourPoint( integration.getCentralAreaPosition( area ) )
    area.getTrafficableAreaPositionsResult = area.getTrafficableAreaPositionsResult or {}
    if #area.getTrafficableAreaPositionsResult == 0 then
        local integration = integration
        local DEC_Geometrie_EstPointDansLocalisation = DEC_Geometrie_EstPointDansLocalisation
        local simArea = area:getLocalisation()
        for i = 1, #area.getAreaPositionsResult do -- On ne veut que les positions qui sont dans la zone
            if DEC_Geometrie_EstPointDansLocalisation( area.getAreaPositionsResult[i], simArea ) then
               area.getTrafficableAreaPositionsResult[ #area.getTrafficableAreaPositionsResult + 1 ] = area.getAreaPositionsResult[i]
            end
        end
    end
    if #area.getTrafficableAreaPositionsResult > 0 then
        return area.getTrafficableAreaPositionsResult
    else
        return area.getAreaPositionsResult
    end
end

--- Returns a list of trafficable positions :
-- <ul> <li> If the given position is in an urban block,
-- this method will return positions in the urban block </li>
-- <li> Otherwise, this method will return a list with the given position as its sole element </li> </ul>
-- @param point Position knowledge
-- @return List of simulation positions
integration.getPointPositions = function( point )
  point.getPointPositionsResult = point.getPointPositionsResult or DEC_Geometrie_CalculerTrafficablePointPourPoint( point.source )
  return point.getPointPositionsResult
end

--- Returns true if the given position is inside the convex hull of all
-- urban blocks in the current map, false otherwise.
-- @param position Simulation position
-- @return Boolean
integration.pointIsInCity = function( position )
    position.isInCityCache = position.isInCityCache or DEC_IsPointInCity( position )
    return position.isInCityCache
end

--- Returns the 2D distance between two localized elements,
-- both defining a "getPosition" method returning a simulation position.
-- This method throws an error if either parameter equals nil, or
-- if either "getPosition" call returns nil. 
-- @param pos1 DirectIA knowledge
-- @param pos2 DirectIA knowledge
-- @return Float, the distance between the elements
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

--- Returns the proximity between two localized elements.
-- The proximity is a number between 1 and 100 (1 meaning remoteness,
-- and 100 meaning closeness), computed with a reversed linear interpolation
-- of the distance from [10; distanceMax] to [1; 100], with distanceMax
-- being the detection distance of this entity (or the defaultDistanceMax parameter
-- if this entity has no detection distance).
-- @see integration.distance
-- @param pos1 DirectIA localized element knowledge
-- @param pos2 DirectIA localized element knowledge
-- @param defaultDistanceMax Float, the default distance max for the linear interpolation (in meters, 4000 by default).
-- @return Float
integration.proximity = function( pos1, pos2, defaultDistanceMax ) -- $$$ MIA security. A mettre en commun avec integration.normalizedInversedDistance
    local pos1Pos = pos1:getPosition()
    local pos2Pos = pos2:getPosition()
    if not pos1Pos or not pos2Pos then
        return 1
    end 
    defaultDistanceMax = defaultDistanceMax or 4000
    local distanceMax = DEC_Detection_Distance and DEC_Detection_Distance() or defaultDistanceMax
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

--- Returns the proximity between two localized elements.
-- The proximity is a number between 1 and 100 (1 meaning remoteness,
-- and 100 meaning closeness), computed with a reversed linear interpolation
-- of the distance from [10; distanceMax] to [1; 100], with distanceMax
-- being the detection distance of this entity (or the defaultDistanceMax parameter
-- if this entity has no detection distance).
-- If a parameter is a surface, this method considers all vertices of the surface,
-- and picks the closest vertex to the other parameter.
-- @see integration.distance
-- @param pos1 DirectIA knowledge
-- @param pos2 DirectIA knowledge
-- @param defaultDistanceMax Float, the default distance max for the linear interpolation (in meters, 100000 by default).
-- @return Float
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

--- Applies a coefficient between 0 and 1 on the max speed of this agent.
-- If the "maxSpeed" parameter is set to 'true', then the agent will move at its max speed.
-- Otherwise, the applied coefficient depends on the "modulation" parameter, on whether or not
-- this agent is in safety/cover mode, on whether or not it is inside an urban block, and
-- on potential orders from its company to slow down.
-- This method can only be called by an agent.
-- @see integration.speedMaxModulation
-- @param modulation Float between 0 and 1 (1 by default).
-- @param maxSpeed Boolean
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
    integration.speedMaxModulation( math.max( currentModulation, 0.01 ) )
end

--- Sets the movement pace (i.e. the max speed) of this entity.
-- The movement pace set to this entity depends on whether or not this entity
-- is in an urban block.
-- This method can only be called by an agent.
-- @see integration.speedMaxModulation
-- @param urgency Boolean. If set to 'true', the modulation equals 1
-- @param modulation Float between 0 and 1 (0.5 by default)
integration.setPace = function( urgency, modulation )
    local currentModulation = modulation or 0.5
    if urgency then
        currentModulation = 1
    end
    if integration.isAgentInsideTown() then
        integration.speedMaxModulation( currentModulation / 2 )
    else
        integration.speedMaxModulation( currentModulation )
    end
end

--- Modify the max speed by the given modulation (between 0 and 1).
-- The unit's new max speed is a fraction of its original max speed, proportional to the modulation factor.
-- This method can only be called by an agent.
-- @param modulation Float, the max speed modulation (between 0 and 1)
integration.speedMaxModulation = function( modulation )
    DEC_ModulationVitesseMax( modulation )
end

--- Modify the current speed by the given modulation (between 0 and 1)
-- The unit will move at a fraction of its current speed, proportional to the modulation factor.
-- This method can only be called by an agent.
-- @param modulation Float, the current speed modulation (between 0 and 1)
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

--- Moves towards the given localized entity, along the provided waypoints (if any), with the given pathtype.
-- This method must be called continuously on several
-- consecutive ticks for the movement to take place.
-- Returns 'true' if the movement action is finished, 'false' otherwise.
-- This method can only be called by an agent.
-- @see integration.startMoveToIt
-- @see integration.updateMoveToIt
-- @see integration.stopMoveToIt
-- @see integration.deselectMoveToIt
-- @param entity DirectIA knowledge, the entity to reach.
-- @param pathType Integer, the path type among the following possible values :
-- <ul> <li> eTypeItiMouvement (normal movement, default value) </li>
-- <li> eTypeItiReconnaissance (reconnaissance) </li>
-- <li> eTypeItiInfiltration (stealth) </li>
-- <li> eTypeItiAttaque (attack) </li>
-- <li> eTypeItiRepli (withdrawal) </li>
-- <li> eTypeItiAppui (support) </li>
-- <li> eTypeItiDeminage (mine-sweeping)</li>
-- <li> eTypeItiLogistique (logistics) </li>
-- <li> eTypeItiCriminal (criminal) </li>
-- <li> eTypeItiNBC (CBRN) </li> </ul>
-- @param waypoints List of directIA knowledges
-- @param wantToStopBecauseOfObstacle Boolean
-- @return Boolean
integration.moveToItGeneric = masalife.brain.integration.startStopAction( 
{ 
    start = function( entity, pathType, waypoints )
        return integration.startMoveToIt( entity, pathType, waypoints ) 
    end,
    started = function( entity, pathType, waypoints, wantToStopBecauseOfObstacle )
        return integration.updateMoveToIt( entity, pathType, waypoints, wantToStopBecauseOfObstacle )
    end, 
    stop = function( entity, pathType, waypoints )
        if waypoints and #waypoints > 0 then
            return integration.deselectMoveToIt( entity )
        else
            return integration.stopMoveToIt( entity )
        end
    end,
} )

--- Starts moving towards the given objective, along the provided waypoints (if any), with the given pathtype.
-- If the objective is in a non-trafficable urban block, then a replacement position at the border will be computed.
-- This method computes a path for the movement action.
-- This method can only be called by an agent.
-- @see integration.updateMoveToIt
-- @see integration.stopMoveToIt
-- @param objective DirectIA knowledge, the entity to reach.
-- @param pathType Integer, the path type among the following possible values :
-- <ul> <li> eTypeItiMouvement (normal movement, default value) </li>
-- <li> eTypeItiReconnaissance (reconnaissance) </li>
-- <li> eTypeItiInfiltration (stealth) </li>
-- <li> eTypeItiAttaque (attack) </li>
-- <li> eTypeItiRepli (withdrawal) </li>
-- <li> eTypeItiAppui (support) </li>
-- <li> eTypeItiDeminage (mine-sweeping)</li>
-- <li> eTypeItiLogistique (logistics) </li>
-- <li> eTypeItiCriminal (criminal) </li>
-- <li> eTypeItiNBC (CBRN) </li> </ul>
-- @param waypoints List of directIA knowledges
-- @param ignorePreparation Boolean. If set to 'false', the agent will :
-- <ul> <li> Undeploy if it is deployed </li>
-- <li> Resume the current paused movement action if there is one </li>
-- <li> Equip its NBC outfit if the destination is in a contaminated object </li> </ul>
-- If set to 'true', the agent will do none of that.
-- @return Boolean, 'false'.
integration.startMoveToIt = function( objective, pathType, waypoints, ignorePreparation )
    myself.blocked = nil
    objective[ myself ] = objective[ myself ] or {}

    -- -------------------------------------------------------------------------------- 
    -- Leaving occupied position
    -- -------------------------------------------------------------------------------- 
    if myself.actionOccupy then
        myself.actionOccupy = DEC__StopAction( myself.actionOccupy )
    end
    myself.location = nil
    
    if not ignorePreparation then
        -- If the agent is undeployed, then deploy
        if integration.isDeployed() then
            integration.undeploy() -- call once, but deployment takes delays.
        end
        
        -- If a movement action was suspended, retrieve it
        if objective[ myself ].moveAction then
            DEC_ReprendAction( objective[ myself ].moveAction )
            return false
        end
        
        -- If the destination is in a contaminated object, equip the NBC outfit
        if DEC_ObjectKnowledge_IsPositionInside( meKnowledge.source, "contamination", objective:getPosition() ) then
            integration.equipNBCOutfit()
        end
    end
    
    -- -------------------------------------------------------------------------------- 
    -- Terrain management: reach a accessible position if position is not accessible 
    -- with a mounted agent.
    -- --------------------------------------------------------------------------------
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

--- Continues moving towards the given objective, along the provided waypoints (if any), with the given pathtype.
-- Returns 'true' if the movement action is finished, 'false' otherwise.
-- This entity will move towards the initial position of the given objective;
-- it will not follow it if the objective is moving.
-- This method can only be called by an agent.
-- @see integration.startMoveToIt
-- @see integration.stopMoveToIt
-- @param objective DirectIA knowledge, the entity to reach.
-- @param pathType Integer, the path type among the following possible values :
-- <ul> <li> eTypeItiMouvement (normal movement, default value) </li>
-- <li> eTypeItiReconnaissance (reconnaissance) </li>
-- <li> eTypeItiInfiltration (stealth) </li>
-- <li> eTypeItiAttaque (attack) </li>
-- <li> eTypeItiRepli (withdrawal) </li>
-- <li> eTypeItiAppui (support) </li>
-- <li> eTypeItiDeminage (mine-sweeping)</li>
-- <li> eTypeItiLogistique (logistics) </li>
-- <li> eTypeItiCriminal (criminal) </li>
-- <li> eTypeItiNBC (CBRN) </li> </ul>
-- @param waypoints List of directIA knowledges
-- @param wantToStopBecauseOfObstacle Boolean
-- @return Boolean, 'true' if the movement is finished, 'false' otherwise.
integration.updateMoveToIt = function( objective, pathType, waypoints, wantToStopBecauseOfObstacle )
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
        if wantToStopBecauseOfObstacle then -- Scipio compatibility
            DEC_PauseAction( objective[ myself ].moveAction )
            local allRes = {}
            local obstacles = {}
            obstacles = integration.getKnowledgesObjectsInCircle( meKnowledge:getPosition(), 1000,
                { eTypeObjectAbatis, eTypeObjectAntiTankObstacle, eTypeObjectBarricade, eTypeObjectBridgeDestruction, 
                  eTypeObjectMines, eTypeObjectLinearMinedArea, eTypeObjectScatteredMinedArea, 
                 eTypeObjectRoadDestruction, eTypeObjectLandslide } )
            for i = 1, #obstacles do
                allRes[ #allRes + 1 ] = CreateKnowledge( integration.ontology.types.object, obstacles[i] )
            end
            meKnowledge:sendArrivedOnMines( meKnowledge:getAutomat(), allRes )
        end
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

--- Pauses the current movement action (if any) towards the given objective.
-- This method can only be called by an agent
-- @param objective DirectIA knowledge
integration.deselectMoveToIt = function( objective )
    local myObjective = objective[ myself ]
    if myObjective and myObjective.moveAction then -- Temp : Wait deselect to be done before destroy
        DEC_PauseAction( myObjective.moveAction )
    end
end

--- Stops moving towards the given objective.
-- @see integration.startMoveToIt
-- @see integration.updateMoveToIt
-- @param objective DirectIA knowledge
-- @return 'false' if there currently is a movement action, nothing otherwise.
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


--- Continues moving towards the given objective with the provided path type.
-- This method should be used to follow a moving target. This entity will continue
-- to move towards the target until it is reached.
-- This method can only be called by an agent.
-- @see integration.startMoveToItArea
-- @see integration.updateMoveToIt
-- @see integration.stopMoveToIt
-- @param objective DirectIA knowledge, the entity to reach.
-- @param pathType Integer, the path type among the following possible values :
-- <ul> <li> eTypeItiMouvement (normal movement, default value) </li>
-- <li> eTypeItiReconnaissance (reconnaissance) </li>
-- <li> eTypeItiInfiltration (stealth) </li>
-- <li> eTypeItiAttaque (attack) </li>
-- <li> eTypeItiRepli (withdrawal) </li>
-- <li> eTypeItiAppui (support) </li>
-- <li> eTypeItiDeminage (mine-sweeping)</li>
-- <li> eTypeItiLogistique (logistics) </li>
-- <li> eTypeItiCriminal (criminal) </li>
-- <li> eTypeItiNBC (CBRN) </li> </ul>
-- @return Boolean, 'true' if the movement is finished, 'false' otherwise.
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
               and distance > 0 
               and DEC_IsPointInUrbanBlockTrafficable( objective:getPosition() ) then -- update movement computation once the objective is out from non trafficable urban block.
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

--- Starts moving along the given path
-- This method can only be called by an agent.
-- @see integration.creerItineraireAPartirListePoint
-- @param objective Path
-- @return Boolean, 'false'
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

--- Computes a path to the given objective with the provided path type.
-- This method can be called until the computation is finished.
-- Returns 'true' if the path is computed, 'false' otherwise.
-- This method can only be called by an agent.
-- @param objective DirectIA objective
-- @param pathType Integer, the path type among the following possible values :
-- <ul> <li> eTypeItiMouvement (normal movement, default value) </li>
-- <li> eTypeItiReconnaissance (reconnaissance) </li>
-- <li> eTypeItiInfiltration (stealth) </li>
-- <li> eTypeItiAttaque (attack) </li>
-- <li> eTypeItiRepli (withdrawal) </li>
-- <li> eTypeItiAppui (support) </li>
-- <li> eTypeItiDeminage (mine-sweeping)</li>
-- <li> eTypeItiLogistique (logistics) </li>
-- <li> eTypeItiCriminal (criminal) </li>
-- <li> eTypeItiNBC (CBRN) </li> </ul>
-- @return Boolean
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
    return pathCompute == 1
end


--- Starts moving towards the given objective.
-- This method can only be called by a crowd.
-- @see integration.updateMoveToItCrowd
-- @see integration.stopMoveToItCrowd
-- @param objective DirectIA knowledge
-- @return Boolean, 'false'
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

--- Continues moving towards the given objective.
-- This crowd will follow the objective if it moves.
-- Returns 'true' if the objective is reached, 'false' otherwise.
-- This method can only be called by a crowd.
-- @see integration.startMoveToItCrowd
-- @see integration.stopMoveToItCrowd
-- @param objective DirectIA knowledge
-- @param pathType Integer, the path type among the following possible values :
-- <ul> <li> eTypeItiMouvement (normal movement, default value) </li>
-- <li> eTypeItiReconnaissance (reconnaissance) </li>
-- <li> eTypeItiInfiltration (stealth) </li>
-- <li> eTypeItiAttaque (attack) </li>
-- <li> eTypeItiRepli (withdrawal) </li>
-- <li> eTypeItiAppui (support) </li>
-- <li> eTypeItiDeminage (mine-sweeping)</li>
-- <li> eTypeItiLogistique (logistics) </li>
-- <li> eTypeItiCriminal (criminal) </li>
-- <li> eTypeItiNBC (CBRN) </li> </ul>
-- @param inertness Boolean, determines how responsive this crowd is
-- to position changes of the objective ('true' for inert, 'false' for responsive).
-- @return Boolean
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

--- Stops moving towards the given objective.
-- @see integration.startMoveToItCrowd
-- @see integration.updateMoveToItCrowd
-- @param objective DirectIA knowledge
-- @return 'false' if there currently is a movement action, nothing otherwise.
integration.stopMoveToItCrowd = function( objective )
    objective[myself] = objective[myself] or {}
    if objective[myself].moveAction then
        DEC__StopAction( objective[myself].moveAction )
        objective[myself].moveAction = nil
        objective.destination = nil
        return false
    end
end

--- Starts moving towards the given objective along the given waypoints (if any).
-- This method can only be called by a crowd.
-- @see integration.updateMoveToItCrowd
-- @see integration.stopMoveToItCrowd
-- @param objective DirectIA knowledge
-- @return Boolean, 'false'
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

--- Returns 100 if the given urban block is trafficable for this agent, 0 otherwise.
-- If the structural state of the given urban block equals 0, then this method returns 0.
-- Otherwise, if the 'loaded' parameter is not set to 'true', then this method returns 100.
-- @param urbanBlock Urban block knowledge
-- @param loaded Boolean, whether this agent is on-board or off-board
-- @return Integer, 0 or 100
integration.isUrbanBlockTrafficable = function( urbanBlock, loaded )
    if integration.getUrbanBlockState( urbanBlock ) == 0 then
        return 0
    end
    if not loaded then
        return 100
    end
    if not urbanBlock.isUrbanBlockTrafficableCache then
        urbanBlock.isUrbanBlockTrafficableCache = ( DEC_ConnaissanceBlocUrbain_Traficabilite( urbanBlock.source ) >= 0 and 100 ) or 0
    end
    return urbanBlock.isUrbanBlockTrafficableCache
end


---- Moves towards the given localized entity, along the provided waypoints (if any), with the given pathtype.
-- Returns 'true' if the movement action is finished, 'false' otherwise.
-- This method can only be called by a crowd.
-- @see integration.startMoveToItCrowd
-- @see integration.updateMoveToItCrowd
-- @see integration.deselectMoveToIt
-- @param objective DirectIA knowledge, the entity to reach.
-- @param pathType Integer, the path type among the following possible values :
-- <ul> <li> eTypeItiMouvement (normal movement, default value) </li>
-- <li> eTypeItiReconnaissance (reconnaissance) </li>
-- <li> eTypeItiInfiltration (stealth) </li>
-- <li> eTypeItiAttaque (attack) </li>
-- <li> eTypeItiRepli (withdrawal) </li>
-- <li> eTypeItiAppui (support) </li>
-- <li> eTypeItiDeminage (mine-sweeping)</li>
-- <li> eTypeItiLogistique (logistics) </li>
-- <li> eTypeItiCriminal (criminal) </li>
-- <li> eTypeItiNBC (CBRN) </li> </ul>
-- @param waypoints List of directIA knowledges
-- @return Boolean
integration.moveToItCrowd = masalife.brain.integration.startStopAction( { 
        start = integration.startMoveToItCrowd,
        started = integration.updateMoveToItCrowd, 
        stop = integration.deselectMoveToIt } )

--- Moves along the given path with the provided pathtype.
-- Returns 'true' if the movement action is finished, 'false' otherwise.
-- This method can only be called by an agent.
-- @see integration.startMoveToItItinerary
-- @see integration.updateMoveToIt
-- @see integration.deselectMoveToIt
-- @param objective Path knowledge
-- @param pathType Integer, the path type among the following possible values :
-- <ul> <li> eTypeItiMouvement (normal movement, default value) </li>
-- <li> eTypeItiReconnaissance (reconnaissance) </li>
-- <li> eTypeItiInfiltration (stealth) </li>
-- <li> eTypeItiAttaque (attack) </li>
-- <li> eTypeItiRepli (withdrawal) </li>
-- <li> eTypeItiAppui (support) </li>
-- <li> eTypeItiDeminage (mine-sweeping)</li>
-- <li> eTypeItiLogistique (logistics) </li>
-- <li> eTypeItiCriminal (criminal) </li>
-- <li> eTypeItiNBC (CBRN) </li> </ul>
-- @return Boolean
integration.moveToItItinerary = masalife.brain.integration.startStopAction( { 
        start = integration.startMoveToItItinerary,
        started = integration.updateMoveToIt, 
        stop = integration.deselectMoveToIt } )
        
--- Returns 100 if the given localized element is in a trafficable position, 0 otherwise.
-- If the given localized element is in a destroyed urban block, this method returns 0.
-- If the 'loaded' parameter is not set to 'true', then this method returns 100.
-- @param location DirectIA knowledge defining a 'getPosition' method returning a simulation position.
-- @param loaded Boolean, whether this agent is on-board or off-board
-- @return Integer, 0 or 100
integration.isPointInUrbanBlockTrafficableForProxy = function( location, loaded )
    local pos = location:getPosition()
    if not pos or DEC_IsPointInDestroyedUrbanBlock( pos ) then 
        return 0
    end
    if not loaded then 
        return 100 
    end
    return DEC_IsPointInUrbanBlockTrafficable( pos ) and 100 or 0
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
        if integration.isAutomat( myself ) then 
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

--- Returns 'true' if the given localized element is in the provided surface, 'false' otherwise.
-- @param position DirectIA knowledge defining a "getPosition" method returning a simulation position
-- @param localisation DirectIA knowledge defining a "getLocalisation" method returning a simulation area
-- @return Boolean
integration.isPointInLocalisation = function( position, localisation )
    return integration.isPointInSimLocalisation( position, localisation:getLocalisation() )
end

--- Returns 'true' if the given localized element is in the provided area, 'false' otherwise.
-- @param position DirectIA knowledge defining a "getPosition" method returning a simulation position
-- @param localisation Simulation area
-- @return Boolean
integration.isPointInSimLocalisation = function( position, localisation )
    return DEC_Geometrie_EstPointDansLocalisation( position:getPosition(), localisation )
end

--- Returns 'true' if the given location is in a trafficable position, 'false' otherwise.
-- @param location DirectIA knowledge defining a "getPosition" method returning a simulation position
-- @return Boolean
integration.isPointTrafficable = function( location )
    local pos = location:getPosition()
    return pos and DEC_IsPointInUrbanBlockTrafficable( pos )
end

--- Returns a list of positions outside a urban block.
-- If the given position is inside a urban block,this method will return
-- a list of positions around this urban block (witout the given position).
-- Otherwise, this method will return a list containing only the given position.
-- @param simPoint Simulation position
-- @return List of simulation positions
integration.getPositionsOutsideUrbanBlock = function( simPoint )
    return DEC_Geometrie_CalculerTrafficablePointPourPoint( simPoint )
end

integration.getUnitPositions = integration.getPositionsOutsideUrbanBlock

--- Returns 'true' if the given position is in a trafficable position for the provided agent, 'false' otherwise.
-- @param platoon Simulation agent
-- @param position Simulation point
-- @return Boolean
integration.isPointInUrbanBlockTrafficableForPlatoon = function( platoon, position )
    return DEC_IsPointInUrbanBlockTrafficableForPlatoon( platoon, position )
end

--- Returns a list of all crossroads on the current terrain.
-- This method can only be called by an agent.
-- @return List of point knowledges
integration.getCrossroads = function()
    local crossroads = {}
    DEC_Crossroads( crossroads )
    return crossroads
end

--- Creates a path from the given list of positions.
-- @param listPoint List of point knowledges
-- @return Path knowledge
integration.creerItineraireAPartirListePoint = function( listPoint )
    local listPointSource = {}
    for i=1,#listPoint do 
        listPointSource[#listPointSource + 1] = listPoint[i].source
    end
    return CreateKnowledge( integration.ontology.types.itinerary, DEC_CreerItineraireListe( listPointSource, eTypeItiMouvement ) )
end

--- Returns 'true' if the given localized element is inside this entity's
-- current area of responsibility, 'false' otherwise.
-- If there is no current area of responsibility, this method returns 'true'.
-- This method can only be called by an agent or a company.
-- @param location DirectIA knowledge defining a "getPosition" method returning a simulation position
-- @return Boolean
integration.isElementInAOR = function( location )
    return DEC_Geometrie_EstPointDansFuseau( location:getPosition() )
end

--- Returns a copy of the given position.
-- @param position Simulation position
-- @return Simulation position
integration.copyPoint = function( position )
    return DEC_Geometrie_CopiePoint( position )
end

--- Returns a list of path keypoints for this entity.
-- Path keypoints are points of interest along a path, such as
-- intersection between a path and a lima, and points before
-- crossroads, bridges, entry into urban or forest areas.
-- The distance before these points can be customised in the authoring tool for each unit.
-- @return List of simulation path keypoints
integration.getPointsCategory = function()
    return DEC_GetPointsCategory()
end

--- Returns 'true' if the given position is a key front position, 'false' otherwise.
-- @param position Simulation path keypoint
-- @return Boolean
integration.isBeforePoint = function( position )
    return DEC_IsAvantPoint( position)
end

--- Returns the type of the given key position.
-- @param position Simulation path keypoint
-- @return Integer, the position's type among the following :
-- <ul> <li> eTypePointNormal </li>
-- <li> eTypePointObservation </li>
-- <li> eTypePointParticulier </li>
-- <li> eTypePointBond </li>
-- <li> eTypePointLima </li>
-- <li> eTypePointSonde </li>
-- <li> eTypePointCCT </li> </ul>
integration.getTypePoint = function( position )
    return DEC_GetTypePoint( position )
end

--- Returns the destination point of the given key position.
-- @param position Simulation key position
-- @return Simulation key position
integration.getDestinationPoint = function( position )
    return DEC_GetDestPoint( position )
end

--- Returns the geometry of a pathPoint (a simulation point) allowing 
-- the decisional models to create a ML knowledge of such a pathPoint
-- @param pathPoint Simulation path keypoint
-- @return Simulation position
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

--- Returns the time (minutes) needed to move towards the
-- provided position with a straight line itinerary
-- @param position DirectIA knowledge, the target with respect to which the time will be computed
-- @return Float
integration.getAgentTimeToReachPosition = function( position )
    return DEC_Agent_TempsPourParcourirDistanceEnLigneDroite( DEC_Geometrie_Distance( meKnowledge:getPosition(), position:getPosition() ) )
end

--- Returns the time left (minutes) during which the agent
-- can remain autonomous while moving.
-- This method can only be called by an agent
-- @return Float
integration.getAgentMovementAutonomy = function()
    return DEC_Agent_AutonomieEnDeplacement()
end

--- Returns a list of all the elements inside the given table without
-- those at the same location as the provided position.
-- @param position Simulation position
-- @param tab Table with DirectIA knowledges as values
-- @return List of DirectIA knowledges
removeListElementWithSameLocation = function( position, tab )
    local result = {}
    for _, elemListReturn in pairs( tab ) do
        if position ~= elemListReturn:getPosition() then
            result[#result+1] = elemListReturn
        end
    end
    return result
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated, use integration.getKnowledgeAgentPosition instead
integration.getPlatoonPosition = function( platoon )
    if integration.isKnowledgeAgentValid( platoon.source ) then
        return integration.getKnowledgeAgentPosition( platoon.source )
    else
        return nil
    end
end

--- Deprecated, use one of these two methods instead :
-- <ul> <li> integration.getBodyPosition to get the position of this agent </li>
-- <li> integration.getTeammatePosition to get the position of a teammate </li> </ul>
integration.getPlatoonAllyPosition = function( platoon )
    if platoon == myself then -- @TODO MGD replace by shared_ptr< DEC_Knowledge_Agent_ABC > and proxy for allies
        return DEC_Agent_Position()
    else
        return DEC_Agent_PositionPtr( platoon.source )
    end
end

--- Deprecated, use integration.getPointPositions instead
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

--- Deprecated
integration.updateMoveToListPointCrowd = function( objective, pathType, wayPoints )
end

--- Deprecated, use integration.beginReinforcementPlatoon instead
integration.renfortMouvement = function( entity )
    DEC_Renforce( entity.source )
end

--- Deprecated, use integration.cancelReinforcement instead
integration.annuleRenfortMouvement = function()
    DEC_AnnuleRenforcement()
end

--- Deprecated, see integration.startMoveToIt
integration.startMoveToItArea = function( objective, pathType, waypoints )
    return integration.startMoveToIt( objective, pathType, waypoints, true )
end