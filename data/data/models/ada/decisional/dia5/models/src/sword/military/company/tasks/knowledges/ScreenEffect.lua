return
{
    init = function( self, params )
        myself.leadData.fuseaux = {}
        myself.leadData.scoutPoints = {}
        local nbPions = integration.nbPlatoonsHaveTask( integration.getEntitiesFromAutomat( meKnowledge, "none", false ), params.mainTasks )
        local fuseaux = DEC_DecouperFuseau( nbPions )
        for _, fuseau in pairs( fuseaux ) do
            myself.leadData.fuseaux[ #myself.leadData.fuseaux + 1 ] = fuseau
        end
        local pointsOnLimas = DEC_Geometrie_GetPointsLimas( eTypeLima_LCAR, nbPions )
        for _, points in pairs( pointsOnLimas ) do
            for _, point in pairs( points ) do
                myself.leadData.scoutPoints[ #myself.leadData.scoutPoints + 1 ]= CreateKnowledge( sword.military.world.Point, point )
            end
        end
        myself.leadData.pionsLima1 = {}
        myself.leadData.pionsLima2 = {}
    end,

    getReachable = function( self, params, entity )  
        if params.meetingPoint then
            return { params.meetingPoint }
        end
        return {}
    end,
 
    getScreenPositions = function( self, params, entity )
        local positions = {}
        myself.leadData.fuseauxIndex = myself.leadData.fuseauxIndex or 0
        myself.leadData.fuseauxIndex = myself.leadData.fuseauxIndex % #myself.leadData.fuseaux + 1
        local fuseau = myself.leadData.fuseaux[ myself.leadData.fuseauxIndex ]
        for i=1, #myself.leadData.scoutPoints do
            local point = myself.leadData.scoutPoints[i]
            if DEC_Geometrie_EstPointDansFuseau_AvecParamFuseau( fuseau, point.source ) then
                positions[ #positions + 1 ] = point
            end
        end
        positions[ #positions + 1 ] = params.meetingPoint
        return positions, CreateKnowledge( sword.military.world.Fuseau, fuseau )
    end,

    getObstacles = function( self, params )
        return { params.obstacles }
    end,

    getNbrFront = function( self )
        return integration.query.getNbrFront( 1 )
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
}
