local result = 
{
    fillParameters = function( self, companyTask, params, entity )
        local objectives= companyTask:getObjectives( params, entity )
        return { objectives = objectives }
    end,
    
    getMeetingPoint = function( self, params )
        return params.objectives
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.company.tasks.knowledges.ReachingEffect"] = t

return result
