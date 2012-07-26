local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyRecon
    end,

    fillParameters = function( self, companyTask, params )
        local camp = companyTask:getReachable( params )
        local elements = companyTask:getElements( params )
        if next( camp ) and next( elements ) then
            return { camp = camp[ 1 ], entities = elements }
        end
        return nil --use by keepBest in Lead skill
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.tasks.knowledges.LoadAndDischarge"] = t

return result
