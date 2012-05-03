local result = 
{
    fillParameters = function( self, companyTask, objectives )
        return { entity = companyTask:getEntity( params ) }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Interrogate"] = t

return result
