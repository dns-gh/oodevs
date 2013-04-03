local result = 
{
    fillParameters = function( self, companyTask, params, entity)
        return { zone = companyTask:getZone( params ), position = companyTask:getPosition( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.art.tasks.knowledges.AcquerirObjectif"] = t

return result