local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCommon
    end,

    fillParameters = function( self, companyTask, params )
        local objectives = companyTask:getEntitiesToFollow( params )
        if next( objectives ) then
             return { objectives = objectives }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge[ "agent.tasks.Follow" ] = t

return result