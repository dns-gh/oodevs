-- Sealable Implementation

local warnFriendsForBuildingSealOffArea = function( clearSealOffArea )
    if not meKnowledge:isSelfCommanding() then -- automat mission
        local simAgents = DEC_Pion_PionsAvecPC()
        for i = 1, #simAgents do
            local friend = CreateKnowledge( sword.military.world.PlatoonAlly, simAgents[i] )
            if clearSealOffArea then -- clear 
                meKnowledge:sendClearBuildingSealOffArea( friend )  
            else
                if friend ~= meKnowledge then -- warn friends that I built seal off area
                    meKnowledge:sendStartBuildingSealOffArea( friend, meKnowledge )
                end
            end
        end
    end
end

local startSealOffLocation = function( location, knowledge )
    local border = DEC_Geometrie_CreerLocalisationPolyligne( DEC_Geometrie_ListePointsLocalisation( location ) )
    knowledge.sealPerceptionID = DEC_Perception_ActivateLocationProgressiveRecce( DEC_Geometrie_AgrandirLocalisation( location, 10 ), 2 )
    local sealOffArea = integration.obtenirObjetProcheDe( location, eTypeObjectSealOffArea, 10 )
    myself.buildingObjectSealOffArea = myself.buildingObjectSealOffArea or false
    if sealOffArea == nil and not myself.buildingObjectSealOffArea then -- need to create seal off area
        DEC_MagicGetOrCreateObject( eTypeObjectSealOffArea, border )
        meKnowledge:RC( eRC_DebutBouclageZone )
        warnFriendsForBuildingSealOffArea( false )
    end
    return true
end

local startedSealOffLocation = function( location, knowledge )
    local sealOffArea = integration.obtenirObjetProcheDe( location, eTypeObjectSealOffArea, 10 )
    if sealOffArea then
        if not knowledge.constructedObject then
            knowledge.constructedObject = sealOffArea
            knowledge.constructedObject.actionAnimation = DEC__StartAnimerObjet( knowledge.constructedObject )
            meKnowledge:RC( eRC_DebutAnimationObjet, knowledge.constructedObject )
        end
    end
    return true
end

local stopSealOffLocation = function( knowledge )
    if knowledge.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( knowledge.constructedObject  ) > 0 then
            DEC__StopAction( knowledge.constructedObject.actionAnimation )
            meKnowledge:RC( eRC_FinAnimationObjet, knowledge.constructedObject )
        end
        if DEC_ConnaissanceObjet_NiveauAnimation( knowledge.constructedObject  ) == 0 then
            DEC_DetruireObjetSansDelais( knowledge.constructedObject ) -- destroy it
            DEC_Perception_DesactiverReconnaissanceLocalisation( knowledge.sealPerceptionID )
            meKnowledge:RC( eRC_FinBouclageZone )
            knowledge.constructedObject = nil
        end
    end
    warnFriendsForBuildingSealOffArea( true )
    return true
end

integration.startSealOffArea = function( area )
    return startSealOffLocation( area.source, area )
end

integration.startedSealOffArea = function( area )
    return startedSealOffLocation( area.source, area )
end

integration.stopSealOffArea = function( area )
    return stopSealOffLocation( area )
end

integration.startSealOffUrbanBlock = function( urbanBlock )
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return startSealOffLocation( buArea, urbanBlock )
end

integration.startedSealOffUrbanBlock = function( urbanBlock )
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return startedSealOffLocation( buArea, urbanBlock )
end

integration.stopSealOffUrbanBlock = function( urbanBlock )
    return stopSealOffLocation( urbanBlock )
end
