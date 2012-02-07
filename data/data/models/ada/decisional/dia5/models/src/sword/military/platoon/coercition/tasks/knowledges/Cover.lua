local result = 
{
    getPionEfficiency = function( self )
        return ePionEfficiencyCombatSupport
    end,

    fillParameters = function( self, companyTask, params )
         return { positions = companyTask:getPositions( params ), directionPoints = companyTask:getDirectionPoints( params ), entities = companyTask:getEntities( params ) }
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["sword.military.platoon.coercition.tasks.knowledges.Cover"] = t

return result
