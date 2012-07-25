local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params )
        local position = companyTask:getReachable( params )
        local elements = companyTask:getElements( params )
        if next( position ) and next( elements ) then
            return { camp = position[ 1 ], entities = elements }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.EscortRefugeesToCamp"] = t

return result
