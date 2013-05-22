return
{
    init = function( self, params )
        self.mainPoints = {}
        self.list = {}
        myself.leadData.pionsLima1 = {}
        myself.leadData.pionsLima2 = {} 
        myself.leadData.fuseaux = {}
        myself.leadData.lcar = true
               
        self.numberEchelons = myself.taskParams.echelonNumber or 0
        if self.numberEchelons == NIL or self.numberEchelons == 0 then
            self.numberEchelons = 1 -- By default units disengage in one echelon
        end
        meKnowledge.nbPionsMain = meKnowledge.nbPionsMain or 0
        
        -- Division of the main area of responsability into sub-zone for each unit

        local fuseaux = integration.query.getFuseaux( meKnowledge.nbPionsMain )
        for _, fuseau in pairs( fuseaux ) do
            myself.leadData.fuseaux[ #myself.leadData.fuseaux + 1 ] = fuseau
        end
        -- Points on blocking lines
        self.pointsOnLimas = integration.getPointsLimas( eTypeLima_LCAR, ( meKnowledge.nbPionsMain + meKnowledge.nbPionsMain % 2 ) /self.numberEchelons )
        local compt = 0
        for _, points in pairs( self.pointsOnLimas ) do
            for _, point in pairs( points ) do
                if meKnowledge.nbPionsMain % 2 == 0 or compt % ( meKnowledge.nbPionsMain + 1 ) ~= meKnowledge.nbPionsMain then
                    self.mainPoints[ #self.mainPoints + 1 ] = CreateKnowledge( sword.military.world.Point, point )
                end
                compt = compt + 1  
            end
        end
      
        local nb = 1
        for pos = 1, ((#self.pointsOnLimas + #self.pointsOnLimas%2 )/self.numberEchelons + 1 ) do
            self.list[ pos ] = {}
            for i = 1, meKnowledge.nbPionsMain do  
                table.insert(self.list[ pos ], self.mainPoints[ nb ])
                nb = nb + 1
            end
        end
        
        if not self.pointsOnLimas or #self.pointsOnLimas == 0 then
            -- Si pas de lima on va s'aligner au niveau de l'unité la plus éloignée du début du fuseau
            meKnowledge:RC( eRC_LimaParameterNotPresent, eTypeLima_LCAR ) 
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
             myself.leadData.lcar = false
        end
    end,

    getReachable = function( self, params, entity )  
        if params.meetingPoint then
            return { params.meetingPoint }
        end
        return {}
    end,
 
    getScreenPositions = function( self, params, entity )
        local objectives = {}

        if not myself.leadData.currentMoveToPosition then
            myself.leadData.currentMoveToPosition = 0
        end
        myself.leadData.currentMoveToPosition = myself.leadData.currentMoveToPosition + 1
        --Add blocking lines points
        for _, points in pairs( self.list ) do
            objectives[ #objectives + 1 ] = points[ myself.leadData.currentMoveToPosition ]
        end
        
        -- 1ere position : alignement sur l'unité la plus éloignée du début du fuseau si on n'a pas de LCAR
        if not myself.leadData.lcar then
            myself.leadData.fuseauxIndex = myself.leadData.fuseauxIndex or 0
            myself.leadData.fuseauxIndex = myself.leadData.fuseauxIndex % #myself.leadData.fuseaux + 1
            local fuseau = myself.leadData.fuseaux[ myself.leadData.fuseauxIndex ]
            local pos = DEC_Geometrie_CalculerPointSurFuseau( fuseau, myself.leadData.advanceMax )
            objectives[ #objectives + 1 ] = CreateKnowledge( sword.military.world.Point, pos )
        end
        
        --Add final destination
        myself.leadData.fuseauxIndex = myself.leadData.fuseauxIndex or 0
        myself.leadData.fuseauxIndex = myself.leadData.fuseauxIndex % #myself.leadData.fuseaux + 1
        DEC_Trace("#myself.leadData.fuseaux : "..tostring(#myself.leadData.fuseaux))
        local fuseau = myself.leadData.fuseaux[ myself.leadData.fuseauxIndex ]
        objectives[ #objectives + 1 ] = CreateKnowledge( integration.ontology.types.point, integration.computeArrivedPointForAOR( fuseau ) )

        -- Update echelons (careful if more than 2 echelons : bad management)
        if myself.leadData.currentMoveToPosition <= ( meKnowledge.nbPionsMain + meKnowledge.nbPionsMain%2)/self.numberEchelons  then
            myself.leadData.pionsLima1[entity] = entity
            F_Pion_SeteEtatEchelon( entity.source, eEtatEchelon_First )
        else
            myself.leadData.pionsLima2[entity] = entity
            F_Pion_SeteEtatEchelon( entity.source, eEtatEchelon_Second )
        end
      
        return objectives
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
