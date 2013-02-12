-- Sealable Implementation

local startSealOffLocation = function( location, knowledge )
    if not knowledge.constructedObject then
        local border = DEC_Geometrie_CreerLocalisationPolyligne( DEC_Geometrie_ListePointsLocalisation( location ) )
        knowledge.sealPerceptionID = DEC_Perception_ActivateLocationProgressiveRecce( DEC_Geometrie_AgrandirLocalisation( location, 10 ), 2 )
        local sealoffarea = nil
        local sealoffarea = integration.obtenirObjetProcheDe( location, eTypeObjectSealOffArea, 10 )
        if sealoffarea == nil then -- need to create seal off area
            DEC_MagicGetOrCreateObject( eTypeObjectSealOffArea, border )
            meKnowledge:RC( eRC_DebutBouclageZone )			
        end
    end
    return true
end

integration.startSealOffArea = function( area )
    startSealOffLocation( area.source, area )
end

integration.startedSealOffArea = function( area )
    local SealOffArea = nil
    SealOffArea = integration.obtenirObjetProcheDe( area.source, eTypeObjectSealOffArea, 10 )
    if SealOffArea then
        if not area.constructedObject then
            area.constructedObject = SealOffArea
            area.constructedObject.actionAnimation = DEC__StartAnimerObjet( area.constructedObject )
            meKnowledge:RC( eRC_DebutAnimationObjet, area.constructedObject )
        end
    end
    return true
end

integration.stopSealOffArea  = function( area )
    if area.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( area.constructedObject  ) > 0 then
            DEC__StopAction( area.constructedObject.actionAnimation )
            meKnowledge:RC( eRC_FinAnimationObjet, area.constructedObject )
        end
        if DEC_ConnaissanceObjet_NiveauAnimation( area.constructedObject  ) == 0 then
            DEC_DetruireObjetSansDelais( area.constructedObject ) -- destroy it
            DEC_Perception_DesactiverReconnaissanceLocalisation( area.sealPerceptionID )
            meKnowledge:RC( eRC_FinBouclageZone )
            area.constructedObject = nil
        end
    end
    local ptRef = integration.getBarycentreZoneFromLocalisation( area.source )
    local lstObjets = integration.getKnowledgesObjectsInCircle( ptRef, 10, { eTypeObjectSealOffArea } ) --cas ou plusieurs pions ont construit un objet car ils sont arrivés au même tick
    if #lstObjets > 0 then
        for i = 1, #lstObjets do
            if DEC_ConnaissanceObjet_NiveauAnimation( lstObjets[i] ) == 0 then
                DEC_DetruireObjetSansDelais( lstObjets[i] ) -- destroy it
            end
        end
    end	
    return true
end

integration.startSealOffUrbanBlock = function( urbanBlock )
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return startSealOffLocation( buArea, urbanBlock )
end

integration.startedSealOffUrbanBlock = function( urbanBlock )
    local SealOffArea = nil
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    SealOffArea = integration.obtenirObjetProcheDe( buArea, eTypeObjectSealOffArea, 10 )
    if SealOffArea then
        if not urbanBlock.constructedObject then
            urbanBlock.constructedObject = SealOffArea
            urbanBlock.constructedObject.actionAnimation = DEC__StartAnimerObjet( urbanBlock.constructedObject )
            meKnowledge:RC( eRC_DebutAnimationObjet, urbanBlock.constructedObject )
        end
    end	
    return true
end

integration.stopSealOffUrbanBlock = function( urbanBlock )
    if urbanBlock.constructedObject  then 
        if DEC_ConnaissanceObjet_NiveauAnimation( urbanBlock.constructedObject  ) > 0 then
            DEC__StopAction( urbanBlock.constructedObject.actionAnimation )
            meKnowledge:RC( eRC_FinAnimationObjet, urbanBlock.constructedObject )
        end
        if DEC_ConnaissanceObjet_NiveauAnimation( urbanBlock.constructedObject  ) == 0 then
            DEC_DetruireObjetSansDelais( urbanBlock.constructedObject ) -- destroy it
            DEC_Perception_DesactiverReconnaissanceLocalisation( urbanBlock.sealPerceptionID )
            meKnowledge:RC( eRC_FinBouclageZone )
            urbanBlock.constructedObject = nil
        end
        local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
        local ptRef = integration.getBarycentreZoneFromLocalisation( buArea )
        local lstObjets = integration.getKnowledgesObjectsInCircle( ptRef, 10, { eTypeObjectSealOffArea } ) --cas ou plusieurs pions ont construit un objet car ils sont arrivés au même tick
        if #lstObjets > 0 then
            for i = 1, #lstObjets do
                if DEC_ConnaissanceObjet_NiveauAnimation( lstObjets[i] ) == 0 then
                    DEC_DetruireObjetSansDelais( lstObjets[i] ) -- destroy it
                end
            end
        end
    end
    return true
end