local result = 
{
    fillParameters = function( self, companyTask, params, entity)
        return { entities = companyTask:getEntities( params, entity ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.art.tasks.knowledges.DefendreUnites"] = t

return result
