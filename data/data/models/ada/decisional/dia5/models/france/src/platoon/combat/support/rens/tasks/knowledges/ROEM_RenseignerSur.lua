local result = 
{
    fillParameters = function( self, companyTask, params )
         local area = companyTask:getArea( params )
         return { area = area }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.rens.tasks.knowledges.ROEM_RenseignerSur"] = t

return result
