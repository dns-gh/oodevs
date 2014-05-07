-------------------------------------------------------------------
---- OBSERVABLE INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

---Returns the hostility level of the given DirectIA agent knowledge regarding
-- the diplomacy matrix
-- @param unit, a DirectIA agent knowledge
-- @return Numeric, the level of hostility  (a percentage). A value of '0' means 'hostile', 
-- a value of '100' means 'friend', a value '50' means neutral.
integration.computeRelationAgent = function( unit )
    if( DEC_ConnaissanceAgent_EstUnEnnemi( unit.source ) == 0 ) then
        return 0
    elseif( DEC_ConnaissanceAgent_EstUnAllie( unit.source ) == 0 ) then
        return 100
    else
        return 50
    end
end

---Returns the hostility level of the given DirectIA object knowledge regarding
-- the diplomacy matrix.
-- @param object, a DirectIA object knowledge
-- @return Numeric, the level of hostility (a percentage). A value of '0' means 'hostile', 
-- a value of '100' means 'friend', a value '50' means neutral.
integration.computeRelationObject = function( object )
    if( DEC_ConnaissanceObjet_EstUnEnnemi( object.source ) == 0 ) then
        return 0
    elseif( DEC_ConnaissanceObjet_EstUnAllie( object.source ) == 0 ) then
        return 100
    else
        return 50
    end
end

---Returns the perception level of the given DirectIA agent knowledge.
-- @param unit, a DirectIA agent knowledge to get in sight
-- @return Numeric, the level of perception  (a percentage). A value of '0' means 'not perceived', 
-- a value of '100' means 'perceived' (the target 'unit' is in sight).
integration.getAgentPerceptionLevel = function( unit )
    if DEC_ConnaissanceAgent_EstPercuParUnite( unit.source ) then
        return 100
    else
        return 0
    end
end

---Returns the perception level of the given DirectIA crowd knowledge.
-- @param crowd, a DirectIA crowd knowledge to get in sight
-- @return Numeric, the level of perception  (a percentage). A value of '0' means 'not perceived', 
-- a value of '100' means 'perceived' (the target 'crowd' is in sight).
integration.getCrowdPerceptionLevel = function( crowd )
    if DEC_ConnaissancePopulation_EstPercueParUnite( crowd.source ) then
        return 100
    else
        return 0
    end
end

---Returns the instant identification level of the given DirectIA object knowledge.
-- @param object, a DirectIA object knowledge
-- @return Numeric, the level of identification  (a percentage). A value of '0' means 'not perceived', 
-- a value of '100' means 'identified', a value of '60' means 'recognized', a value of 30 means 'detected'.
integration.getObjectPerception = function( object )
    local perceptionLevel = DEC_ConnaissanceObjet_NiveauDePerceptionCourant( object.source )
    if ( perceptionLevel == 1 ) then
        return 30 -- in sight, target is detected
    elseif ( perceptionLevel == 2 ) then
        return 60 -- in sight, target is recognized
    elseif ( perceptionLevel == 3 ) then
        return 100 -- in sight, target is identified
    else
        return 0 -- not perceived
    end
end

---Returns the reconnaissance level ( a percentage of military reconnaissance) of 
-- the given DirectIA urbanBlock knowledge.
-- @param urbanBlock, a DirectIA urbanBlock knowledge
-- @return Numeric, the level of reconnaissance (a percentage). A value of '0' means 'not reconnoitered' at all,
-- a value of '100' means totally 'reconnoitered'.
-- @see integration.getUrbanBlockCurrentReconnaissanceState
integration.getUrbanBlockPerception = function( urbanBlock )
    return integration.getUrbanBlockCurrentReconnaissanceState( urbanBlock.source ) * 100
end

---Returns the reconnaissance level ( a percentage of military reconnaissance) of 
-- the given Simulation urbanBlock knowledge.
-- @param urbanBlock, a Simulation urbanBlock knowledge
-- @return Numeric, the level of reconnaissance. A value of '0' means 'not reconnoitered' at all,
-- a value of '1' means totally 'reconnoitered'.
-- @see integration.getUrbanBlockPerception
integration.getUrbanBlockCurrentReconnaissanceState = function( urbanBlock )
  return DEC_ConnaissanceUrbanBlock_NiveauDeReconnaissanceCourant( urbanBlock )
end

--- Action to orientate the agent's sensors toward the given objective
-- @param objective, a DirectIA 'LocalizedElement' knowledge (such as 'Point', 'Area', UrbanBlock', 'Object' types)
-- @param activeRadar, a boolean to determine if the agent should activates special sensors in addition to normal sensors
-- (see sensors definitions in authoring tool). 
integration.observeIt = function( objective, activeRadar )
    if masalife.brain.core.class.isOfType( objective, integration.ontology.types.direction ) then
        DEC_Perception_VisionVerrouilleeSurDirection( objective.source )
    else
        if objective:isValid() then
            DEC_Perception_VisionVerrouilleeSurPointPtr( objective:getPosition() )
            if activeRadar then
                DEC_Perception_ActiverRadarSurPointPtr( eRadarType_Radar, objective:getPosition() )
            end
        end
    end
end

--- Action to orientate the agent's sensors toward the given direction
-- @param direction, a 'Direction' DirectIA knowledge
integration.observeDirection = function( direction )
    DEC_Perception_VisionVerrouilleeSurDirection( direction.source )
end

--- Estimate the efficiency (a percentage) of the an agent perception capability from the provided position, 
-- toward the given objective.
-- @param objective, a Directia knowledge defining the element to observe.
-- @param position, a Directia knowledge defining the position from which to observe.
-- @return Numeric, the value of ray tracing between the given position and the objective.
-- @see integration.getPerception method
integration.getEstimatePerception = function( objective, position )
    if not position.getEstimatePerceptionsResult then
        position.getEstimatePerceptionsResult = {}
    end
    if not position.getEstimatePerceptionsResult[ objective ] then
        position.getEstimatePerceptionsResult[ objective ] 
        = LinearInterpolation( 0, 100, 0, DEC_Detection_Distance(), true, 
        integration.getPerception( objective, position ) )
    end
    return position.getEstimatePerceptionsResult[ objective ]
end


--- For a commander agent, to estimate the perception efficiency (a percentage) 
-- from the provided position, toward the given objective, regarding the physical
-- characteristics of the given 'entity'. 
-- This method can only be called by an commander agent.
-- @param entity, a DirectIA agent knowledge.
-- @param objective, a Directia 'observable' knowledge defining the element to observe.
-- @param position, a Directia 'observing' knowledge defining the position from which to observe.
-- @return Numeric, the value of ray tracing between the given position and the objective.
-- @see integration.getEstimatePerception method
integration.getEstimatePerceptionForPion = function( entity, objective, position )
    if not position.estimatePerceptions then
        position.estimatePerceptions = {}
    end
    if not position.estimatePerceptions[ objective ] then
        local estimatePerception = DEC_Automate_PerceptionPourPion( entity.source, objective:getPosition(), position:getPosition())
        position.estimatePerceptions[ objective ] = LinearInterpolation( 0, 100, 0, 4000, true, estimatePerception, position:getPosition() )
    end
    return position.estimatePerceptions[ objective ]
end

--- Defines whether the given 'point' is perceived by the agent or not.
-- @param point, a Directia knowledge defining the element to observe.
-- @return Boolean, 'true' if the point is in sight, 'false' otherwise.
integration.isPointVisible = function( point )
    return DEC_Perception_PointEstVisible( point.source )
end

--- Returns the current revelance of the given agent knowledge.
-- A value of '100' means that the knowledge agent is perfecly known (so it is 
-- currently in sight by the agent or one of its teammates belonging to the same knowledge group). 
-- A value strictly greater than '0' means that the knowledge is no more perceived.
-- A knowledge agent with a revelance of '0' does not exist anymore, it disapears.
-- When not perceived by any of the knowledge group agent, the relevance value decreases 
-- every tick until it reaches '0'. In that case it is 'invalid' 
-- (@see integration.isKnowledgeAgentValid )
-- @param knowledge, a Directia knowledge agent. 
-- @return Numeric, the current relevance of the knowledge.
integration.getRelevance = function( knowledge )
  return knowledge.source:DEC_AgentKnowledge_Relevance()
end

--- Returns the agent maximum perception distance (METERS), regarding physical characteristics 
-- of sensors (see sensors definition in authoring tool).
-- This method can only be called by an agent.
-- @return Numeric, the perception distance in meters
integration.getDetectionDistance = function()
    return DEC_Detection_Distance()
end

--- Informs if the agent is currently perceiving the given knowledge agent.
-- This method can only be called by an agent.
-- @param agent, a knowledge agent
-- @return Boolean, 'true' if the knowledge agent is perceiving the agent, 'false' otherwise.
integration.isAgentPerceivesAgent = function( agent )
    return DEC_ConnaissanceAgent_PercoitUnite( agent )
end

--- Returns the agent maximum identification distance (METERS) regarding physical characteristics of 
-- major components. 
-- of sensors (see sensors definition in authoring tool).
-- This method can only be called by an agent.
-- @return Numeric, the identification distance in meters
integration.getIdentificationDistance = function()
    return DEC_Identification_DistanceMaxCompMajeure()
end
