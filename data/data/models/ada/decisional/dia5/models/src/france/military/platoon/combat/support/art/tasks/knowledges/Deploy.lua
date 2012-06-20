local result = 
{
    fillParameters = function( self, companyTask, params )
        return { firePositions = companyTask:getFirePositions( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.art.tasks.knowledges.Deploy"] = t

return result
