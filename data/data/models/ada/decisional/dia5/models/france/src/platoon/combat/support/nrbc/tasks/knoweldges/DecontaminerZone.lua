local result = 
{
    fillParameters = function( self, companyTask, params )
         local zone = companyTask:getZone( params )
         return { area = zone }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.nrbc.tasks.knoweldges.DecontaminerZone"] = t

return result
