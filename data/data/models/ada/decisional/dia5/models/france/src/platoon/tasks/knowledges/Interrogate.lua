local result = 
{
    fillParameters = function( self, companyTask )
        return { entity = companyTask:getEntity( params ) }
    end,
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.tasks.knowledges.Interrogate"] = t

return result
