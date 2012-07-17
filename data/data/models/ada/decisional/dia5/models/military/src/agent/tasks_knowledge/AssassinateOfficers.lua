local result = 
{
    fillParameters = function( self, companyTask, objectives ) end,

}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks_knowledge.AssassinateOfficers"] = t

return result
