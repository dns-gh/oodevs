--- Returns the identification level of the given agent knowledge by this entity's knowledge group :
-- <ul> <li> 0 if the agent knowledge is not detected at all <li>
-- <li> 30 if the agent knowledge is detected </li>
-- <li> 60 if the agent knowledge is recognized </li>
-- <li> 100 if the agent knowledge is identified </li> </ul>
-- @param unit DirectIA agent knowledge
-- @return Float between 0 and 100
integration.getAgentIdentificationLevel = function( unit )
    local identificationLevel = DEC_ConnaissanceAgent_NiveauPerceptionMax( unit.source )
    if identificationLevel == 1 then return 30 -- The unit is detected
    elseif identificationLevel == 2 then return 60 -- The unit is recognized
    elseif identificationLevel == 3 then return 100 -- The unit is identified
    else return 0 -- The unit is not detected
    end
end

--- Orientates this entity's sensors towards the objective in order to identify it.
-- This method does nothing if the objective is tactically destroyed.
-- This method can only be called by an agent.
-- @param objective DirectIA agent knowledge defining a "getPosition" method returning a simulation position.
integration.identifyIt = function( objective )
    if not DEC_ConnaissanceAgent_EstDetruitTactique( objective.source ) then
        DEC_Perception_VisionVerrouilleeSurPointPtr( objective:getPosition() )
        DEC_Perception_ActiverRadarSurPointPtr( eRadarType_Radar, objective:getPosition() )
    end
end

--- Returns true if the given target is within identification range of this entity, false otherwise.
-- This method can only be called by an agent.
-- @param target Knowledge defining a "getPosition" method returning a simulation position
-- @return Boolean
integration.isInIdentificationRange = function( target )
    return integration.distance( meKnowledge, target ) < ( DEC_Reconnoissance_MajorComponentMinDistance() * 0.9 )
end