local result = 
{
    fillParameters = function( self, companyTask, params )
         local position = companyTask:getPosition( params )
         return { position = position }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.nrbc.tasks.knoweldges.AnimerPlotDecontamination"] = t

return result
