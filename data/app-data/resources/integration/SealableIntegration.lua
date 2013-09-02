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

local startedSealOffLocation = function( location, knowledge )
    local SealOffArea = integration.obtenirObjetProcheDe( location, eTypeObjectSealOffArea, 10 )
    if SealOffArea then
        if not knowledge.constructedObject then
            knowledge.constructedObject = SealOffArea
            knowledge.constructedObject.actionAnimation = DEC__StartAnimerObjet( knowledge.constructedObject )
            meKnowledge:RC( eRC_DebutAnimationObjet, knowledge.constructedObject )
        end
    end
    return true
end

local stopSealOffLocation = function( location, knowledge )
    local DEC_ConnaissanceObjet_NiveauAnimation = DEC_ConnaissanceObjet_NiveauAnimation
    local DEC_DetruireObjetSansDelais = DEC_DetruireObjetSansDelais

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
    local ptRef = integration.getBarycentreZoneFromLocalisation( location )
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

integration.startSealOffArea = function( area )
    startSealOffLocation( area.source, area )
end

integration.startedSealOffArea = function( area )
    return startedSealOffLocation( area.source, area )
end

integration.stopSealOffArea  = function( area )
    return stopSealOffLocation( area.source, area )
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
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return stopSealOffLocation( buArea, urbanBlock )
end
