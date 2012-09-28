local result = 
{
    fillParameters = function( self, companyTask, params, entity, retrogradeContext, objectif, taskName )
        local entities = companyTask:getObstaclesPlan( params, entity, taskName )
        local meetingPoint = companyTask:getFinalMeetingPoint( params )
        local withImprovement = companyTask:getWithImprovement( params )
        local waitForReinforcement = companyTask:getWithReinforcement( params )
        if next( entities ) then
            return { entities             = entities, 
                     meetingPoint         = meetingPoint, 
                     withImprovement      = withImprovement, 
                     waitForReinforcement = waitForReinforcement }
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
        local localisation = obstacle:getLocalisation()
        return DEC_Agent_AgentPeutConstruireObjetEmbarqueAvecLocalisation( 
                    entity.source, tostring(obstacle:getType()), localisation )
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.Build" ] = t

return result