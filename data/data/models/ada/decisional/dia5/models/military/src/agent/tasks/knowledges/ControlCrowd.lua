local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params )
        return { area = companyTask:getControlCrowdArea( params ) }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.ControlCrowd"] = t

return result
