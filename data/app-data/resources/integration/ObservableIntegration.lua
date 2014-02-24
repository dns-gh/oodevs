-------------------------------------------------------------------------------
-- Observable Implementation : 
-- Regroup function to manage observation
-- @author MGD
-- @created 2010-10-26
-- @modified MGD 2010-07-07
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

--- Compute diplomatie between brain's team and agent's team
-- @param knowledge on an agent 
-- @author MGD
-- @release 2010-01-22
integration.computeRelationAgent = function( unit )
  if( DEC_ConnaissanceAgent_EstUnEnnemi( unit.source ) == 0 ) then -- Note 0 correspond to tristate:true
    return 0
  elseif( DEC_ConnaissanceAgent_EstUnAllie( unit.source ) == 0 ) then
    return 100
  else
    return 50
  end
end

--- Compute diplomatie between brain's team and  tacticalObject's team
-- @param knowledge on an object
-- @author MGD
-- @release 2010-01-22
integration.computeRelationObject = function( object )
  if( DEC_ConnaissanceObjet_EstUnEnnemi( object.source ) == 0 ) then
    return 0
  elseif( DEC_ConnaissanceObjet_EstUnAllie( object.source ) == 0 ) then
    return 100
  else
    return 50
  end
end

--  Get unit perception level
--  Return 100 if agent is perceived, 0 otherwise
-- @param knowledge on an unit
-- @author SLG -- modi MIA
-- @release 2010-01-27
integration.getAgentPerceptionLevel = function( unit )
    if DEC_ConnaissanceAgent_EstPercuParUnite( unit.source ) then
        return 100
    else
        return 0
    end
end

--  Get crowd perception level
--  Return 100 if agent is perceived, 0 otherwise
-- @param knowledge on a crowd
integration.getCrowdPerceptionLevel = function( crowd )
    if DEC_ConnaissancePopulation_EstPercueParUnite( crowd.source ) then
        return 100
    else
        return 0
    end
end

--- Get object perception
-- @param knowledge on an object
-- @author SLG
-- @release 2010-01-27
integration.getObjectPerception = function( object )
  local perceptionLevel = DEC_ConnaissanceObjet_NiveauDePerceptionCourant( object.source )
  if ( perceptionLevel == 1 ) then
    return 30
  elseif ( perceptionLevel == 2 ) then
    return 60
  elseif ( perceptionLevel == 3 ) then
    return 100
  else
    return 0   
  end
end


integration.getUrbanBlockPerception = function( urbanBlock )
  return integration.getUrbanBlockCurrentReconnaissanceState( urbanBlock.source ) * 100
end

integration.getUrbanBlockCurrentReconnaissanceState = function( urbanBlock )
  return DEC_ConnaissanceUrbanBlock_NiveauDeReconnaissanceCourant( urbanBlock )
end

-- Orientate the agent's sensors toward the given entities (a localized element)
-- @param objective a 'LocalizedElement' knowledge (such as 'Point', 'Area', UrbanBlock', 'Object' types of objective)
-- @param activeRadar a boolean to determine if the agent activates his radar for detection
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
  -- reportFunction(eRC_VisionVerrouilleeSur ) @TODO GGE à déplacer pour être gérer sans boucler
end

--- Orientate the agent's sensors toward the given direction
-- @param a 'Direction'  knowledge 
integration.observeDirection = function( direction )
        DEC_Perception_VisionVerrouilleeSurDirection( direction.source )
end

--- Find the perception level from position to objective for static objects
-- @param objective, position
-- @author LMT
-- @modified MGD
-- @release 2010-07-07
integration.getEstimatePerception = function ( objective, position )
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


--- Find the perception level from position to objective for static objects
-- @pions: pion, objective, position
-- @author DDA
-- @release 2010-10-13
integration.getEstimatePerceptionForPion = function ( entity, objective, position )
    if not position.estimatePerceptions then
        position.estimatePerceptions = {}
    end
    if not position.estimatePerceptions[ objective ] then
        local estimatePerception = DEC_Automate_PerceptionPourPion( entity.source, objective:getPosition(), position:getPosition())
        position.estimatePerceptions[ objective ] = LinearInterpolation( 0, 100, 0, 4000, true, estimatePerception, position:getPosition() )
    end
    return position.estimatePerceptions[ objective ]
end

integration.isPointVisible = function ( point )
    return DEC_Perception_PointEstVisible( point.source )
end

integration.getDetectionDistance = function ()
    return DEC_Detection_Distance()
end

integration.isAgentPerceivesAgent = function ( agent )
    return DEC_ConnaissanceAgent_PercoitUnite( agent )
end

integration.getIdentificationDistance = function()
    return DEC_Identification_DistanceMaxCompMajeure()
end

-- Return relevance of a unit knowledge (between 0 and 1)
-- @param knowledge on an unit
integration.getRelevance = function( knowledge )
    return knowledge.source:DEC_AgentKnowledge_Relevance()
end 

