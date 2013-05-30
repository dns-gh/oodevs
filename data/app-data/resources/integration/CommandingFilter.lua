-------------------------------------------------------------------------------
-- Commanding filter Implementation :
-- Regroup function to filter platoon in a company
-- @author MGD
-- @created 2010-06-17
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

--- Return all platoon in a company
-- @author MGD
-- @release 2011-01-20
integration.defaultPlatoonInCompany = function( )
    local allRes = {}
    local listPlatoon = DEC_Automate_PionsAvecPCCommunication()
    local CreateKnowledge = CreateKnowledge
    for _,pion in pairs( listPlatoon ) do
        allRes[ #allRes + 1 ] = CreateKnowledge( integration.ontology.types.agent, pion )
    end
    return allRes
end

--- Filter platoonAlly corresponding to a role
-- @param listPlatoonAlly A list of PlatoonAlly
-- @param role The role to find
-- @author MGD
-- @release 2010-06-17
integration.filterPionWithRole = function( listPlatoonAlly, role )
    local _res =  {}
    for _,x in pairs( listPlatoonAlly or emptyTable ) do
        if x:getRole() == role then
            _res[ #_res + 1 ] = x
        end
    end
    return _res
end

--- Filter platoonAlly corresponding to an echelon level
-- @param listPlatoonAlly A list of PlatoonAlly
-- @param echelon The echelon level to find
-- @author MGD
-- @release 2010-06-24
integration.filterPionWithEchelon = function( listPlatoonAlly, echelon )
    local _res =  {}
    for _,x in pairs( listPlatoonAlly or emptyTable ) do
        if F_Pion_GeteEtatEchelon( x.source ) == echelon then
            _res[ #_res + 1 ] = x
        end
    end
    return _res
end

-- -------------------------------------------------------------------------------- 
-- Return all echelons:
-- result[1]: pions in first echelon
-- result[2]: pions in second echelon and in the reserve
-- result[3]: pions in scout echelon 
-- result[4]: pions in reserve echelon 
-- result[5]: pions none
-- @author DDA
-- @release 2010-12-01
-- --------------------------------------------------------------------------------
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
-- -------------------------------------------------------------------------------- 
-- Set echelons
-- @author DDA
-- @release 2011-02-04
-- --------------------------------------------------------------------------------
integration.setPionsEchelons = function( pions, echelon )
     for _, pion in pairs( pions ) do
       F_Pion_SeteEtatEchelon( pion.source, echelon )
     end
 end
 
--- Filter platoonAlly by echelon level
-- @param listPlatoonAlly A list of PlatoonAlly
-- @return Map indexed by echelon level
-- @author MGD
-- @release 2010-06-24
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

--- Number of potentional platoons to scout missions in automat
-- @author DDA
-- @release 2010-01-11
integration.reconPlatoons = function( listPlatoonAlly )
    local nb =  0
    for _, platoon in pairs( listPlatoonAlly or emptyTable ) do
       if platoon:computePhysicalEfficiency( ePionEfficiencyRecon ) > 80 then
        nb = nb + 1 
       end
    end
    return nb
end

--- Number of platoons who have the targetTask  
-- @author DDA
-- @release 2010-01-11
integration.nbPlatoonsHaveTask = function( listPlatoonAlly, targetTask )
    local nb =  0
    for _, platoon in pairs( listPlatoonAlly or emptyTable ) do
        if DEC_IsMissionAvailable( platoon.source, targetTask ) then
            nb = nb + 1
        end
    end
    return nb
end

integration.isCompanyMoving = function( company )
    local subordinates = company.source:DEC_Automate_PionsAvecPC()
    for _, subordinate in pairs( subordinates or emptyTable ) do
        if subordinate:DEC_Agent_IsMoving() then
            return true
        end
    end
    return false
end

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
