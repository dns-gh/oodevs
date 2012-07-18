local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params )
        local positions = companyTask:getReachable( params )
        local entities = companyTask:getEntities( params )
        return { objectives = entities, positions = positions }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.Fix"] = t

return result
