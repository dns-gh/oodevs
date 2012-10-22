local result = 
{
    fillParameters = function( self, companyTask, params )
        return { deploymentPositions = companyTask:getFirePositions( params ), 
                 moveAutomatically   = companyTask:getChangeAutomatically( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.knowledges.DeployOnPositions" ] = t

return result