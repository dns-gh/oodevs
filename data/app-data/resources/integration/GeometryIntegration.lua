--Default Geometry Implementation

integration.positionTranslateDir = function ( position, direction, dist )
    return DEC_Geometrie_PositionTranslateDir( position, direction, dist )
end

integration.locationInPoints = function( location )
    return DEC_Geometrie_ListePointsLocalisation( location )
end

integration.isPointInAOR_WithParam = function( fuseau, position )
    return DEC_Geometrie_EstPointDansFuseau_AvecParamFuseau( fuseau, position )
end

integration.computeArrivedPointForAOR = function( fuseau )
    return DEC_Geometrie_CalculerPointArriveePourFuseau( fuseau )
end

integration.computeLocationBarycenter = function( location )
    return DEC_Geometrie_CalculerBarycentreLocalisation( location )
end

integration.positionAdvanceAlongAORAutomat = function( position )
    return DEC_Geometrie_PositionAdvanceAlongFuseauAutomat( position )
end

integration.computePointOnAOR = function( fuseau, distance )
    return DEC_Geometrie_CalculerPointSurFuseau( fuseau, distance )
end

integration.findRoadIntersectionWithZone = function( area )
    return DEC_Geometrie_FindRoadIntersectionWithZone( area )
end

integration.isPointInsideLocation = function( position, location )
    return DEC_Geometrie_EstPointDansLocalisation( position, location )
end

integration.createLocationCircle = function( point, radius )
    return DEC_Geometrie_CreerLocalisationCercle( point, radius )
end

integration.advanceAlongAOR = function( agent )
    return DEC_Geometrie_AdvanceAlongFuseau( agent )
end

integration.compute3DDistance = function( firstPosition, firstHeight, secondPosition, secondHeight )
    return DEC_Geometrie_Distance3D( firstPosition, firstHeight, secondPosition, secondHeight )
end

integration.computeDistance = function( firstPosition, secondPosition )
    return DEC_Geometrie_Distance( firstPosition, secondPosition )
end

integration.postionAdvanceAlongAOR = function( position )
    return DEC_Geometrie_PositionAdvanceAlongFuseau( position )
end

integration.enlargeLocation = function( location, distance )
    return DEC_Geometrie_AgrandirLocalisation( location, distance )
end

integration.computeNearestBorder = function( position, scaledObject )
    return DEC_Geometrie_ComputeNearestBorder( position, scaledObject )
end

integration.positionsEqual = function( firstPosition, secondPosition )
    return DEC_Geometrie_PositionsEgales( firstPosition, secondPosition )
end

integration.computeUrbanBlockLocations = function( urbanBlock )
    return DEC_Geometrie_CalculerLocalisationsBU( urbanBlock )
end

integration.computePointNearLocationsInsideAOR = function( position )
    return DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau( position )
end

integration.createLocationPolyline = function( positionsList )
    return DEC_Geometrie_CreerLocalisationPolyligne( positionsList )
end

integration.createDirection = function( firstPosition, secondPosition )
    return DEC_Geometrie_CreerDirection( firstPosition, secondPosition )
end

integration.geometryX = function( position )
    return position:DEC_Geometrie_X()
end

integration.geometryY = function( position )
    return position:DEC_Geometrie_Y( )
end

integration.computeSafetyPosition = function( agent, safetyDistance )
    return DEC_Geometrie_CalculerPositionSurete( agent, safetyDistance )
end

integration.splitAORInSections = function( length )
    return DEC_Geometrie_DecoupeFuseauEnTroncons( length )
end

integration.clipperLocalisation = function( area, fuseau )
    return DEC_Geometrie_ClipperLocalisation( area.source, fuseau.source )
end

-- NMI
integration.convertAORtoLocalisation = function ( fuseau ) 
    return DEC_Geometrie_ConvertirFuseauEnLocalisation( fuseau.source )
end