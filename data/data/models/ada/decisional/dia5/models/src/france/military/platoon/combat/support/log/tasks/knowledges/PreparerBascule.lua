local result = 
{
    fillParameters = function( self, companyTask, params )
        local position = companyTask:getPosition( params )
        if position == nil or position == NIL then
            return { automat = companyTask:getAutomat( params ) }
        else
            return { position = position, automat = companyTask:getAutomat( params ) }
        end
    end
}

local t = initTaskKnowledge( result )
taskKnowledge["france.military.platoon.combat.support.log.tasks.knowledges.PreparerBascule"] = t

return result
