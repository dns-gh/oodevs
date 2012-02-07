local result =  
{
    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getEntityToEscort( params )
        return { objective = objective }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Escort"] = t

return result
