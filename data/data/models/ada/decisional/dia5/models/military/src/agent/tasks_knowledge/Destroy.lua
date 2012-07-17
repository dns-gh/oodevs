local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombat
    end,

    fillParameters = function( self, companyTask, params )
        local entities = companyTask:getReachable( params )
        if next( entities ) then
            return { entities = entities }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Destroy"] = t

return result
