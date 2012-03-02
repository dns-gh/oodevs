-- Sealable Implementation

local startSealOffLocation = function( location, knowledge )
    local border = DEC_Geometrie_CreerLocalisationPolyligne( DEC_Geometrie_ListePointsLocalisation( location ) )
    knowledge.sealPerceptionID = DEC_Perception_ActivateLocationProgressiveRecce( DEC_Geometrie_AgrandirLocalisation( location, 10 ), 2 )
    if not knowledge.constructedObject then
        local objects = integration.getObjectsInArea( location, { "seal off area" } ) or {}
        if  #objects < 1 then
            DEC_CreerObjetSansDelais( "seal off area", border ) 
            meKnowledge:RC( eRC_DebutBouclageZone )
        else
            knowledge.constructedObject = objects[ 1 ]
            integration.startAnimateIt( knowledge.constructedObject )
        end
    end
    return true
end

integration.startSealOffArea = function( area )
    startSealOffLocation( area.source, area )
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
    DEC_Perception_DesactiverReconnaissanceLocalisation( area.sealPerceptionID )
    return true
end

integration.startSealOffUrbanBlock = function( urbanBlock )
    local buArea = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return startSealOffLocation( buArea, urbanBlock )
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