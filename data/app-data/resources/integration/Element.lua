--- Returns the priority to approach the given knowledge.
-- @param target Knowledge defining a "proximityLevel" method returning a float
-- @return Float between 0 and 1, 0 meaning the lowest priority and 1 meaning the highest priority
integration.approachPriority = function( target )
    return math.max( target:proximityLevel(), 1 ) / 100
end

--- Changes the company of this entity.
-- If the first given parameter is a company, then the second parameter should
-- be 'true', and this entity will change its superior to the given company.
-- If the first given parameter is an agent, then the second parameter should
-- be 'false', and this entity will change its superior to the given agent's company.
-- The new and the old companies must be in the same army.
-- This method can only be called by an agent.
-- @param ally DirectIA agent or company
-- @param isAutomat Boolean, whether or not the first parameter is a company
-- @return Boolean, whether or not the change was successful
integration.changeAutomate = function( ally, isAutomat )
    if isAutomat then
        -- No need to change if the old and new companies are the same
        if ally.source ~= DEC_GetAutomate( myself ) then
            return DEC_Pion_ChangeAutomate( ally.source ) 
        end
    else 
        -- No need to change if the old and new companies are the same
        if DEC_GetAutomate( ally.source ) ~= DEC_GetAutomate( myself ) then
            return DEC_Pion_ChangeAutomate( DEC_GetAutomate( ally.source ) ) 
        end
    end
end

--- Returns true if the given agent is jammed, false otherwise.
-- @param agent DirectIA agent
-- @return Boolean
integration.agentEstBrouille = function( agent )
    return DEC_Agent_EstBrouille( agent.source )
end

--- Returns true if the given company's hq unit is jammed (or
-- in partial or total radio silence), false otherwise
-- @param company Company knowledge
-- @return Boolean
integration.pcEstBrouille = function( company )
    local pc = DEC_Pion_PionPCDeAutomate( company.source )
    return DEC_Agent_EstBrouille( pc ) or DEC_Agent_EstEnSilenceRadioEmission( pc ) or DEC_Agent_EstEnSilenceRadioReception( pc )
end