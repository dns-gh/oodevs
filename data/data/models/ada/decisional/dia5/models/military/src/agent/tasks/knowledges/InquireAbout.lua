local result = 
{
    fillParameters = function( self, companyTask, objectives ) end,
}

local t = initTaskKnowledge( result )
taskKnowledge["agent.tasks.knowledges.InquireAbout"] = t

return result
