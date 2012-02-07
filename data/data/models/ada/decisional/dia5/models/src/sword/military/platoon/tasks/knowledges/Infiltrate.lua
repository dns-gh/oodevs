local result = 
{
    fillParameters = function( self, companyTask, objectives ) end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.tasks.knowledges.Infiltrate"] = t

return result
