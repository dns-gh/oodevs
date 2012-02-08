local result = 
{
    fillParameters = function( self, companyTask, params, entity, marcheRetrograde, objectif, taskName )
        local entities = companyTask:getObstaclesPlan( params, entity, taskName )
        local meetingPoint = companyTask:getFinalMeetingPoint( params )
        local withImprovement = companyTask:getWithImprovement( params )
        if next( entities ) then
            return { entities = entities, meetingPoint = meetingPoint, withImprovement = withImprovement }
        end
        return nil
    end,
    
    getObstaclesParameter = function( self, companyTask, params )
        return companyTask:getObstaclesParameter( params )
    end,

    hasDotation = function( self, entity, obstacle )
        return DEC_Agent_AgentADotationPourConstruireObjet( entity.source, tostring(obstacle:getType()) )
    end,

    canDoIt = function( self, entity, obstacle )
        return  DEC_Agent_AgentPeutConstruireObjetEmbarque( entity.source, tostring(obstacle:getType()) )
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Build"] = t

return result
