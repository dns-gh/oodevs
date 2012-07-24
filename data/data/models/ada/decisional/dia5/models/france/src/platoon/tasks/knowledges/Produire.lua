local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyEngineeringSupport
    end,

    fillParameters = function( self, companyTask, params )
        return { objective = companyTask:getObjective( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.tasks.knowledges.Produire"] = t

return result
