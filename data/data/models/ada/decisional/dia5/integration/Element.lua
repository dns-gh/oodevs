

------------------------------------------------------------------
--- APPROACHABLE INTERFACE IMPLEMENTATION SPECIFIC TO AN ELEMENT
------------------------------------------------------------------
integration.approachPriority = function( self )
    return math.max( self:proximityLevel(), 1 ) / 100 --NORMALEMENT DEVRAIT ETRE L'ACCESSIBILITYLEVEL
end

integration.changeAutomate = function( self, isAutomat )
    if isAutomat then
        if self.source ~= DEC_GetAutomate( meKnowledge.source ) then -- Si c'est le m�me automate, pas besoin d'effectuer le changement
            return DEC_Pion_ChangeAutomate( self.source ) 
        end
    else 
        if DEC_GetAutomate( self.source ) ~= DEC_GetAutomate( meKnowledge.source ) then -- Si c'est le m�me automate, pas besoin d'effectuer le changement
            return DEC_Pion_ChangeAutomate( DEC_GetAutomate( self.source ) ) 
        end
    end
end

integration.agentEstBrouille = function( self )
    return DEC_Agent_EstBrouille( meKnowledge.source )
end

integration.pcEstBrouille = function( self )
    local pc = DEC_Pion_PionPCDeAutomate( meKnowledge.source )
    return DEC_Agent_EstBrouille( pc ) or DEC_Agent_EstEnSilenceRadioEmission( pc ) or DEC_Agent_EstEnSilenceRadioReception( pc )
end