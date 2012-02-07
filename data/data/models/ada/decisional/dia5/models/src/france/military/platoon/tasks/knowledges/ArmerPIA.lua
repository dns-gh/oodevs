local result = 
{
    fillParameters = function( self, companyTask, params, entity )
        local positions = companyTask:getPositions( params )
        local direction = companyTask:getDirection( params )
        return { objective = direction, positions = positions }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.tasks.knowledges.ArmerPIA"] = t

return result
