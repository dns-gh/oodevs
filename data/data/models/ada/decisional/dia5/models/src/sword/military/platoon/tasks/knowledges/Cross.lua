local result = 
{
    fillParameters = function( self, companyTask, params, entity )
        return { objectives = companyTask:getMeetingPoint( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Cross"] = t

return result
