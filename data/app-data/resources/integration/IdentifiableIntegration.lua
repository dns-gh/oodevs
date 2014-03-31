-- getAgentIdentificationLevel
-- @param knowledge of a unit
-- @author LMT -- modif MIA
-- @release 2010-02-03
integration.getAgentIdentificationLevel = function( unit )
    local identificationLevel = DEC_ConnaissanceAgent_NiveauPerceptionMax( unit.source )
    if identificationLevel == 1 then return 30 -- The unit is detected
    elseif identificationLevel == 2 then return 60 -- The unit is recognized
    elseif identificationLevel == 3 then return 100 -- The unit is identified
    else return 0 -- The unit is not detected
    end
end

--- Observe a knowledge
-- @param position of a knowledge
-- @author LMT
-- @release 2010-02-04
integration.identifyIt = function( objective )
    if not DEC_ConnaissanceAgent_EstDetruitTactique(objective.source) then
    -- reportFunction(eRC_VisionRadarVerrouilleeSur ) @TODO GGE a deplacer pour le gerer sans boucler
        if masalife.brain.core.class.isOfType( objective, integration.ontology.types.direction ) then
            DEC_Perception_VisionVerrouilleeSurDirection( objective.source )
        else
            DEC_Perception_VisionVerrouilleeSurPointPtr( objective:getPosition() )
            DEC_Perception_ActiverRadarSurPointPtr( eRadarType_Radar, objective:getPosition() )
        end
    end
end
