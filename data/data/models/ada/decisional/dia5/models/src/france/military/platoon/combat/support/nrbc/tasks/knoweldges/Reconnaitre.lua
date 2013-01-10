local result = 
{
    fillParameters = function( self, companyTask, params )
         local area = companyTask:getArea( params )
         return { area = area }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.nrbc.tasks.knoweldges.Reconnaitre"] = t

return result
