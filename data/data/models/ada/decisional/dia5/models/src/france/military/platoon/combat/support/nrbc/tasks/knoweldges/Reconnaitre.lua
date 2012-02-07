local result = 
{
    fillParameters = function( self, companyTask, params )
         local entities = companyTask:getEntities( params )
         return { entities = entities }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.nrbc.tasks.knoweldges.Reconnaitre"] = t

return result
