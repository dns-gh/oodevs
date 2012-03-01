local result = 
{
    fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif, taskName )
        local entities =companyTask:getObstaclesPlan( params, entity, taskName )
        local meetingPoint = companyTask:getMeetingPoint( params )
        local withImprovement = companyTask:getWithImprovement( params )
        if next(entities) then
            return { entities = entities, meetingPoint = meetingPoint, withImprovement = withImprovement }
        else
           return nil
        end
    end,

    getObstaclesParameter = function( self, companyTask, params )
        return companyTask:getObstaclesParameter( params )
    end,

    hasDotation = function( self, entity, obstacle )
        return DEC_Agent_AgentADotationPourConstruireObjet( entity.source, tostring(obstacle:getType()) )
    end,

    canDoIt = function( self, entity, obstacle )
        local localisation = obstacle:getLocalisation()
        if localisation ~= nil then
            return DEC_Agent_AgentPeutConstruireObjetEmbarqueAvecLocalisation( entity.source, tostring(obstacle:getType()), localisation )
        else
            return false
        end
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.engineer.tasks.knowledges.AppuiContreMobilite"] = t

return result
