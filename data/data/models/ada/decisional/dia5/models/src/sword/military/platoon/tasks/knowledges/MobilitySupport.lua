local result = 
{
    fillParameters = function( self, companyTask, params )
        return { entities = companyTask:getEntitiesToFollow( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.MobilitySupport"] = t

return result
