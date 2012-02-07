local result = 
{
    fillParameters = function( self, companyTask, params )
        local objective = companyTask:getObject( params )
        local destination = companyTask:getDestination( params )
        return { objective = objective, destination = destination }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.GetDecon"] = t

return result
