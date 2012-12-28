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

        -- découpage du fuseau principal en sous-fuseaus pour chaque unité
        local fuseaux = DEC_DecouperFuseau( meKnowledge.nbPionsMain )
        for _, fuseau in pairs( fuseaux ) do
            myself.leadData.fuseaux[ #myself.leadData.fuseaux + 1 ] = fuseau
        end

        -- Organisation du dsipositif initial à 300 metres devant la LCAR (moins en zurb ?) si elle est présente
        local LimaId = DEC_GetLima( eTypeLima_LCAR )
        if LimaId > 0 and meKnowledge.nbPionsMain > 0 then
            pointsBeforeLimas = DEC_Geometrie_CalculerPositionsParRapportALima( LimaId, 300, meKnowledge.nbPionsMain)
            for _, point in pairs( pointsBeforeLimas ) do
                myself.leadData.scoutPoints[ #myself.leadData.scoutPoints + 1 ]= CreateKnowledge( sword.military.world.Point, point )
            end
        end
        -- Si pas de lima on va s'aligner au niveau de l'unité la plus éloignée du début du fuseau
        local advance
        myself.leadData.advanceMax = 0
        local units = integration.getEntitiesFromAutomatCommunication( meKnowledge, "none", false )
        for i=1, #units do
           local unit = units[i]
           local coord = unit:getPosition()
           advance = DEC_Geometrie_PositionAdvanceAlongFuseauAutomat( coord )
           if advance > myself.leadData.advanceMax then
               myself.leadData.advanceMax = advance
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
        local positions = {} -- découpage en fuseau
        local lcar = false
        myself.leadData.fuseauxIndex = myself.leadData.fuseauxIndex or 0
        myself.leadData.fuseauxIndex = myself.leadData.fuseauxIndex % #myself.leadData.fuseaux + 1
        local fuseau = myself.leadData.fuseaux[ myself.leadData.fuseauxIndex ]
        for i=1, #myself.leadData.scoutPoints do
            local point = myself.leadData.scoutPoints[i]
            if DEC_Geometrie_EstPointDansFuseau_AvecParamFuseau( fuseau, point.source ) then
                positions[ #positions + 1 ] = point
                lcar = true
            end
        end
        
        -- 1ere position : alignement sur l'unité la plus éloignée du début du fuseau si on n'a pas de LCAR
        if not lcar then
            local pos = DEC_Geometrie_CalculerPointSurFuseau( fuseau, myself.leadData.advanceMax )
            positions[ #positions + 1 ] = CreateKnowledge( sword.military.world.Point, pos )
        end
        
        --Ajout de la destination finale
        positions[ #positions + 1 ] = CreateKnowledge( sword.military.world.Point, DEC_Geometrie_CalculerPointArriveePourFuseau( fuseau ) )
        
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
