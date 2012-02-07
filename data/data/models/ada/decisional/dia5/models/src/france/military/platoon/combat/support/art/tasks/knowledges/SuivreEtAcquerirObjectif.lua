local result = 
{
    fillParameters = function( self, companyTask, params, entity)
        return { zone = companyTask:getZone( params ), positionDebarquement = NIL, entity = companyTask:getEntity( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.art.tasks.knowledges.SuivreEtAcquerirObjectif"] = t

return result
