--Default Lima Implementation

integration.getLimaFromType = function( limaType )
    return DEC_GetLima( limaType )
end

integration.getLimaPoint = function( position )
    return DEC_GetLimaPoint( position )
end

integration.getTypeLimaFromPoint = function( position )
    return DEC_GetTypeLimaPoint( position )
end

integration.getPointsLimas = function ( typeLima, number )
    return DEC_Geometrie_GetPointsLimas( typeLima, number )
end

integration.computePositionsRelativeToLima = function( id, distance, number )
    return DEC_Geometrie_CalculerPositionsParRapportALima( id, distance, number )
end

integration.computePositionsRelativeToLima = function( id )
    return DEC_GetMissionLimaFlag( id )
end

integration.setMissionLimaFlag = function( lima, booleen )
    DEC_SetMissionLimaFlag( lima, booleen )
end

integration.setMissionLimaFlagSchedule = function( lima, booleen )
    DEC_SetMissionLimaFlagHoraire( lima, booleen )
end

integration.removeFromPointsCategory = function( point )
    return DEC_RemoveFromPointsCategory( point )
end