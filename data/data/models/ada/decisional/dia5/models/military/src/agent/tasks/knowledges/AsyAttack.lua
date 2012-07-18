local result = 
{
    fillParameters = function( self, companyTask, objectives ) end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.AsyAttack"] = t

return result
