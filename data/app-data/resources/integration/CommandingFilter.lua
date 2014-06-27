-------------------------------------------------------------------
---- COMMANDINGFILTER INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

--- Returns all platoons in the company
-- This method can only be called by a company
-- @return list of DirectIA agents
integration.defaultPlatoonInCompany = function( )
    local allRes = {}
    local listPlatoon = DEC_Automate_PionsAvecPCCommunication()
    local CreateKnowledge = CreateKnowledge
    local integration = integration
    for _,pion in pairs( listPlatoon ) do
        allRes[ #allRes + 1 ] = CreateKnowledge( integration.ontology.types.agent, pion )
    end
    return allRes
end

--- Filter a list of agents corresponding to the given role
-- @param listPlatoonAlly, A list of DirectIA agents
-- @param role, The role to find (Adaptation tools, tab "units", "type" field)
-- @return list of DirectIA agents with the appropriate role
integration.filterPionWithRole = function( listPlatoonAlly, role )
    local _res =  {}
    for _,x in pairs( listPlatoonAlly or emptyTable ) do
        if x:getRole() == role then
            _res[ #_res + 1 ] = x
        end
    end
    return _res
end

--- Filter a list of agents corresponding to the given echelon level
-- @param listPlatoonAlly, A list of DirectIA agents
-- @param echelon, The echelon level to find (eEtatEchelon_None, eEtatEchelon_Second, eEtatEchelon_First, eEtatEchelon_Scout, eEtatEchelon_Reserve)
-- @return list of DirectIA agents with the appropriate echelon
integration.filterPionWithEchelon = function( listPlatoonAlly, echelon )
    local _res =  {}
    for _,x in pairs( listPlatoonAlly or emptyTable ) do
        if F_Pion_GeteEtatEchelon( x.source ) == echelon and x:isOperational() then
            _res[ #_res + 1 ] = x
        end
    end
    return _res
end

-- Return a table of agents sorted by the echelon level
-- @param pions, A list of DirectIA agents
-- @return table of DirectIA agents sorting by the echelon level like
-- @return tab[1]: list of DirectIA agents in first echelon
-- @return tab[2]: list of DirectIA agents in second echelon and in the reserve
-- @return tab[3]: list of DirectIA agents in scout echelon 
-- @return tab[4]: list of DirectIA agents in reserve echelon 
-- @return tab[5]: list of DirectIA agents in none echelon
integration.getPionsInEchelons = function( pions )
 
    local result = { {}, {}, {}, {}, {} }

    for _, pion in pairs ( pions ) do
        local echelon = F_Pion_GeteEtatEchelon( pion.source )
        if echelon == eEtatEchelon_First then
            result [1][#result[1]+ 1] = pion     
        elseif echelon == eEtatEchelon_Second then
            result [2][#result[2] + 1] = pion
        elseif echelon == eEtatEchelon_Scout then
            result [3][#result[3] + 1] = pion
        elseif echelon == eEtatEchelon_Reserve then
            result [4][#result[4] + 1] = pion
        else -- None
            result [5][#result[5] + 1] = pion
        end
     end
     return result
 end

-- Modify the echelon of all agents with the given echelon
-- @param pions, A list of DirectIA agents
-- @param echelon, The echelon level to set (eEtatEchelon_None, eEtatEchelon_Second, eEtatEchelon_First, eEtatEchelon_Scout, eEtatEchelon_Reserve)
integration.setPionsEchelons = function( pions, echelon )
     for _, pion in pairs( pions ) do
       F_Pion_SeteEtatEchelon( pion.source, echelon )
     end
 end
 
-- Return a table of agents sorting by the echelon level
-- @param listPlatoonAlly, A list of DirectIA agents
-- @return list of DirectIA agents indexed by echelon level
integration.filterPionByEchelon = function( listPlatoonAlly )
    local _res =  {}
    for _,x in pairs( listPlatoonAlly or emptyTable ) do
        local echelon = F_Pion_GeteEtatEchelon( x.source )
        if _res[ echelon ] then
            _res[ echelon ][ #_res[ echelon ] + 1 ] = x
        else
            _res[ echelon ] = { x }
        end
    end
    return _res
end

--- Returns the number of agents in automat with a reconnaissance efficiency (Adaptation tool, tab "units", part "skills", filed "Recon" ) greater than the given minEfficiency (80 by default)
-- @param listPlatoonAlly, A list of DirectIA agents
-- @param minEfficiency, An percentage of efficiency (80 by default)
-- @return Integer, the number of DirectIA agents with a good reconnaissance efficiency
integration.reconPlatoons = function( listPlatoonAlly, minEfficiency )
    minEfficiency = minEfficiency or 80
    local nb =  0
    for _, platoon in pairs( listPlatoonAlly or emptyTable ) do
       if platoon:computePhysicalEfficiency( ePionEfficiencyRecon ) > minEfficiency then
        nb = nb + 1 
       end
    end
    return nb
end

--- Number of agents in automat who can perform the given targetTask  (Adaptation tool, tab "Doctrine models")
-- @param listPlatoonAlly, A list of DirectIA agents
-- @param targetTask, A task name
-- @param onlyOperational Boolean, whether this method should ignore the tactically destroyed platoons or not
-- @return Integer, the number of DirectIA agents with the specific task
integration.nbPlatoonsHaveTask = function( listPlatoonAlly, targetTask, onlyOperational )
    local nb =  0
    for _, platoon in pairs( listPlatoonAlly or emptyTable ) do
        if DEC_IsMissionAvailable( platoon.source, targetTask ) and ( not onlyOperational or not integration.UnitIsNeutralized( platoon ) ) then
            nb = nb + 1
        end
    end
    return nb
end

--- Returns true if the automat is moving (indeed one of his agents), false otherwise
-- @param company, DirectIA automat
-- @return Boolean, true if the automat is moving (indeed one of his agents), false otherwise
integration.isCompanyMoving = function( company )
    local subordinates = company.source:DEC_Automate_PionsAvecPC()
    for _, subordinate in pairs( subordinates or emptyTable ) do
        if subordinate:DEC_Agent_EstEnMouvement() then
            return true
        end
    end
    return false
end

--- Returns true if the automat is flying (indeed all of his agents), false otherwise
-- @param company, DirectIA automat
-- @return Boolean, true if the automat is moving (indeed all of his agents), false otherwise
integration.isCompanyFlying = function( company )
    local subordinates = company.source:DEC_Automate_PionsAvecPC()
    local flyingUnits = {}
    for _, subordinate in pairs( subordinates or emptyTable ) do
        if subordinate:DEC_Agent_IsFlying() then
            flyingUnits[ #flyingUnits + 1 ] = subordinate
        end
    end
    if #flyingUnits == #subordinates then
        return true
    else
        return false
    end
end