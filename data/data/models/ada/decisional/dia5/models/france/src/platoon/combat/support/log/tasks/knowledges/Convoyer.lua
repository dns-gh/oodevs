local result = 
{
    fillParameters = function( self, companyTask, params )
        return {}
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.log.tasks.knowledges.Convoyer"] = t

return result
