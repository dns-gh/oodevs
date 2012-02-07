-- Sealable Implementation

integration.startSealOffArea = function( area )
    local bords = DEC_Geometrie_CreerLocalisationPolyligne( DEC_Geometrie_ListePointsLocalisation( area.source ) )
    area.perceptionID = DEC_Perception_ActivateLocationProgressiveRecce( DEC_Geometrie_AgrandirLocalisation( bords, 10 ), 2 )
    if not area.constructedObject then
        local objects = integration.getObjectsInArea( area.source, { "seal off area" } ) or {}
        if  #objects < 1 then
            local bords = DEC_Geometrie_CreerLocalisationPolyligne( DEC_Geometrie_ListePointsLocalisation( area.source ) )
            DEC_CreerObjetSansDelais( "seal off area", bords )
            meKnowledge:RC( eRC_DebutBouclageZone )
        else
            area.constructedObject = objects[ next( objects ) ]
            integration.startAnimateIt( area.constructedObject )
        end
    end
    return true
end

integration.startedSealOffArea = function( area )
    if not area.constructedObject then
        local objects = integration.getObjectsInArea( area.source, { "seal off area" } )
        if #objects >= 1 then
            area.constructedObject = objects[ next( objects ) ]
            integration.startAnimateIt( area.constructedObject )
        end
    end
    return true
end

integration.stopSealOffArea  = function( area )
    local objects = integration.getObjectsInArea( area.source, { "seal off area" } )
    if objects then
        local object = objects[ next( objects ) ]
        if integration.animationLevel( object ) > 0 then
            integration.stopAnimateIt( object )
        end
        if integration.animationLevel( object ) == 0 then
            DEC_DetruireObjetSansDelais( object.source )
            meKnowledge:RC( eRC_FinBouclageZone )
        end
    end
    area.constructedObject = nil
    DEC_Perception_DesactiverReconnaissanceLocalisation( area.perceptionID )
    return true
end

integration.startSealOffUrbanBlock = function( urbanBlock )
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    local buLine = DEC_Geometrie_CreerLocalisationPolyligne( DEC_Geometrie_ListePointsLocalisation( buArea ) )
    urbanBlock.sealPerceptionID = DEC_Perception_ActivateLocationProgressiveRecce( DEC_Geometrie_AgrandirLocalisation( buLine, 10 ), 2 )
    if not urbanBlock.constructedObject then
        local objects = integration.getObjectsInArea( buArea, { "seal off area" } ) or {}
        if  #objects < 1 then
            DEC_CreerObjetSansDelais( "seal off area", buLine ) 
            meKnowledge:RC( eRC_DebutBouclageZone )
        else
            urbanBlock.constructedObject = objects[ next( objects ) ]
            integration.startAnimateIt( urbanBlock.constructedObject )
        end
    end
    return true
end

integration.startedSealOffUrbanBlock = function( urbanBlock )
    if not urbanBlock.constructedObject then
        local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
        local objects = integration.getObjectsInArea( buArea, { "seal off area" } )
        if #objects >= 1 then
            urbanBlock.constructedObject = objects[ next( objects ) ]
            integration.startAnimateIt( urbanBlock.constructedObject )
        end
    end
    return true
end

integration.stopSealOffUrbanBlock = function( urbanBlock )
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    local objects = integration.getObjectsInArea( buArea, { "seal off area" } )
    if objects then
        local object = objects[ next( objects ) ]
        if integration.animationLevel( object ) > 0 then
            integration.stopAnimateIt( object )
        end
        if integration.animationLevel( object ) == 0 then
            DEC_DetruireObjetSansDelais( object.source )
            meKnowledge:RC( eRC_FinBouclageZone )
        end
    end
    urbanBlock.constructedObject = nil
    DEC_Perception_DesactiverReconnaissanceLocalisation( urbanBlock.sealPerceptionID )
    return true
end