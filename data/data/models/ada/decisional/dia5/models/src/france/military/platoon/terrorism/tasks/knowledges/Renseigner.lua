local result = 
{
    fillParameters = function( self, companyTask, params, entity )
        return { destinataire = companyTask:getDestinataire( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.terrorism.tasks.knowledges.Renseigner"] = t

return result