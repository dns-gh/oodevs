local result = 
{
    fillParameters = function( self, companyTask, objectives ) end,
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.stabilisation.tasks.knowledge.InquireAbout"] = t

return result
