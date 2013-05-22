integration.clipperLocalisation = function( area, fuseau )
    return DEC_Geometrie_ClipperLocalisation( area.source, fuseau.source )
end

integration.computeArrivedPointForAOR = function( fuseau )
    return DEC_Geometrie_CalculerPointArriveePourFuseau( fuseau )
end

integration.getPointsLimas = function ( typeLima, number )
    return DEC_Geometrie_GetPointsLimas( typeLima, number )
end