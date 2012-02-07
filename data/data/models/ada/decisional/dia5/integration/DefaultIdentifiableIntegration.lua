local eRadarType_Radar = 0
local eRadarType_Ecoute = 1
local eRadarType_EcouteRadar = 2

-- getAgentIdentificationLevel
-- @param knowledge of a unit
-- @author LMT -- modif MIA
-- @release 2010-02-03
integration.getAgentIdentificationLevel = function( unit )
    local identificationLevel = DEC_ConnaissanceAgent_NiveauPerceptionMax( unit.source )
    if identificationLevel == 1 then return 30
    elseif identificationLevel == 2 then return 60
    elseif identificationLevel == 3 then return 100
    else return 0
    end
end

--- Observe a knowledge
-- @param position of a knowledge
-- @author LMT
-- @release 2010-02-04
integration.identifyIt = function( objective )
    if not DEC_ConnaissanceAgent_EstDetruitTactique(objective.source) then
    -- meKnowledge:RC( eRC_VisionRadarVerrouilleeSur ) @TODO GGE a deplacer pour le gerer sans boucler
        if masalife.brain.core.class.isOfType( objective, integration.ontology.types.direction ) then
            DEC_Perception_VisionVerrouilleeSurDirection( objective.source )
        else
            DEC_Perception_VisionVerrouilleeSurPointPtr( objective:getPosition() )
            DEC_Perception_ActiverRadarSurPointPtr( eRadarType_Radar, objective:getPosition() )
        end
    end
end
