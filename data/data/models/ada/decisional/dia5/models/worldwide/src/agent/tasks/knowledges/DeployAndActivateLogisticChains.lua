local result = 
{
    fillParameters = function( self, companyTask, params )
        return { deploymentPosition = companyTask:getDeploymentPosition( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.DeployAndActivateLogisticChains" ] = t

return result