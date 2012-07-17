local result = 
{
    fillParameters = function( self, companyTask, objectives ) end
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.Infiltrate"] = t

return result
