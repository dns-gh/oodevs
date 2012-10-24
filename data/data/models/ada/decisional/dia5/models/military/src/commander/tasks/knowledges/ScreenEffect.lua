return
{
    init = function( self, params )
        myself.leadData.fuseaux = {}
        myself.leadData.scoutPoints = {}
        myself.leadData.pionsLima1 = {}
        myself.leadData.pionsLima2 = {}
        meKnowledge.nbPionsMain = meKnowledge.nbPionsMain or 0

        self.numberEchelons = myself.taskParams.echelonNumber or 0
        if self.numberEchelons == NIL or self.numberEchelons == 0 then
            self.numberEchelons = 1 -- par défaut les pions décrochent sur 1 échelon
        end

        local fuseaux = integration.query.getFuseaux( meKnowledge.nbPionsMain )
        for _, fuseau in pairs( fuseaux ) do
            myself.leadData.fuseaux[ #myself.leadData.fuseaux + 1 ] = fuseau
        end

        -- Organisation du dsipositif initial à 300 metres devant la LCAR (moins en zurb ?)
        local LimaId = integration.getLimaFromType( eTypeLima_LCAR )
        if LimaId > 0 and meKnowledge.nbPionsMain > 0 then
            pointsBeforeLimas = integration.computePositionsRelativeToLima( LimaId, 300, meKnowledge.nbPionsMain)
            for _, point in pairs( pointsBeforeLimas ) do
                myself.leadData.scoutPoints[ #myself.leadData.scoutPoints + 1 ]= CreateKnowledge( world.Point, point )
            end
        end
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
            if integration.isPointInAOR_WithParam( fuseau, point.source ) then
                positions[ #positions + 1 ] = point
            end
        end

       --Ajout de la destination finale
        positions[ #positions + 1 ] = params.meetingPoint

        if not myself.leadData.currentMoveToPosition then
            myself.leadData.currentMoveToPosition = 0
        end
        myself.leadData.currentMoveToPosition = myself.leadData.currentMoveToPosition + 1

        -- Mise à jour des echelons: A revoir pour un nombre d'échelons > 2
        if myself.leadData.currentMoveToPosition <= ( meKnowledge.nbPionsMain + meKnowledge.nbPionsMain%2)/self.numberEchelons  then
            myself.leadData.pionsLima1[entity] = entity
            F_Pion_SeteEtatEchelon( entity.source, eEtatEchelon_First )
        else
            myself.leadData.pionsLima2[entity] = entity
            F_Pion_SeteEtatEchelon( entity.source, eEtatEchelon_Second )
        end
      
        return positions
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
