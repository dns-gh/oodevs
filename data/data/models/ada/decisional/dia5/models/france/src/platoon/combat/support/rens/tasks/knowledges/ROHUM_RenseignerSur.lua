local result = 
{
    fillParameters = function( self, companyTask, params )
         local area = companyTask:getArea( params )
         local byNight = companyTask:getByNight( params )
         return { area = area, byNight = byNight }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["platoon.combat.support.rens.tasks.knowledges.ROHUM_RenseignerSur"] = t

return result
