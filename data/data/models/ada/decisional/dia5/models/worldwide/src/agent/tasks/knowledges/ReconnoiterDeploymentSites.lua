local result = 
{
    fillParameters = function( self, companyTask, params, entity, retrogradeContext, objectif, taskName )
        local entities = companyTask:getObstaclesPlan( params, entity, taskName )
        local meetingPosition = companyTask:getFinalMeetingPoint( params )
        if next( entities ) then
            return { deploymentPositions = entities, 
                     meetingPosition = meetingPosition }
        end
        return nil
    end,

    getObstaclesParameter = function( self, companyTask, params )
        return companyTask:getObstaclesParameter( params )
    end,

    hasDotation = function( self, entity, obstacle )
        return DEC_Agent_AgentADotationPourConstruireObjet( entity.source, tostring( obstacle:getType() ) )
    end,

    canDoIt = function( self, entity, object )
        local localisation = object:getLocalisation()
        return DEC_Agent_AgentPeutConstruireObjetEmbarqueAvecLocalisation( 
                    entity.source, tostring( object:getType() ), localisation )
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.ReconnoiterDeploymentSites" ] = t

return result