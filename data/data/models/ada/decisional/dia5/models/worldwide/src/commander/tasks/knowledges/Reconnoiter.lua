
return
{
    init = function( self, params )
        local advance
        myself.leadData.advanceMax = 0
        for i=1, #myself.taskParams.objectives do
            local objective = myself.taskParams.objectives[ i ]
            local coord = objective:getPosition()
            advance = integration.positionAdvanceAlongAORAutomat( coord )
            if advance > myself.leadData.advanceMax then
                myself.leadData.advanceMax = advance
            end
        end
    end,

    getObjectives = function( self, params, entity )
        local positions = {}
        local addLastPoint = true
        local fuseau = self:getAOR( params )

        if not next( params.objectives ) then
            positions[ #positions + 1 ] = CreateKnowledge( world.Point, 
              integration.computeArrivedPointForAOR( fuseau.source ) )
            return positions, fuseau
        end

        for i = 1, #params.objectives do
            local objective = params.objectives[ i ]
            local myPositions = objective:getPositions()
            for i = 1, #myPositions do
                if integration.isPointInAOR_WithParam( fuseau.source, myPositions[ i ] ) then
                    if integration.positionAdvanceAlongAORAutomat( objective:getPosition() )
                     >= myself.leadData.advanceMax then
                      addLastPoint = false
                    end
                    if not exists( positions, objective ) then
                        positions[ #positions + 1 ] = objective
                    end
                end
            end
        end
        local function comp( w1,w2 )
            local dist1 = integration.distance( entity, w1 )
            local dist2 = integration.distance( entity, w2 )
            return ( dist1 < dist2 )
        end
        -- order the list from the nearest to the furthest.
        table.sort( positions, comp )
        if not next( positions ) or addLastPoint then
          local pos = integration.computePointOnAOR( fuseau.source, myself.leadData.advanceMax )
          positions[ #positions + 1 ] = CreateKnowledge( world.Point, pos )
        end
        return positions, fuseau
    end,

    getAOR = function( self, params )
        if not myself.leadData.currentReccePosition then
            myself.leadData.currentReccePosition = 0
            myself.leadData.fuseaux = {}
            local fuseaux = integration.query.getFuseaux( myself.taskParams.maxNbrFront )
            for _,fuseau in pairs( fuseaux ) do
                myself.leadData.fuseaux[ #myself.leadData.fuseaux +1 ] = CreateKnowledge( world.Fuseau, fuseau )
            end
        end
        myself.leadData.currentReccePosition = myself.leadData.currentReccePosition % #myself.leadData.fuseaux + 1
        return myself.leadData.fuseaux[ myself.leadData.currentReccePosition ]
    end,

    getScoutObjectives = function( self, params, entity )
        return self:getObjectives( params, entity )
    end,

    getPeiAOR = function( self, params )
        if not myself.leadData.currentPeiPosition then
            myself.leadData.currentPeiPosition = 0
            myself.leadData.fuseauxPei = {}
            local fuseaux = integration.query.getFuseaux( myself.taskParams.maxNbrFront )
            for _,fuseau in pairs( fuseaux ) do
                myself.leadData.fuseauxPei[ #myself.leadData.fuseauxPei +1 ] = CreateKnowledge( world.Fuseau, fuseau )
            end
        end
        myself.leadData.currentPeiPosition = myself.leadData.currentPeiPosition % #myself.leadData.fuseauxPei + 1
        return myself.leadData.fuseauxPei[ myself.leadData.currentPeiPosition ]
    end,

    getReachable = function( self, params, entity )
        if not params.meetingPoint then
            return {}
        end
        return params.meetingPoint 
    end,

    getObstacles = function( self, params )
        if not params.obstacles then
            return {}
        end
        if not myself.leadData.obstaclesIndex then
            myself.leadData.obstaclesIndex = 0 
        end

        myself.leadData.obstaclesIndex = myself.leadData.obstaclesIndex + 1
        if myself.leadData.obstaclesIndex > #params.obstacles then
            return {}
        end   
        return { params.obstacles[ myself.leadData.obstaclesIndex ] }
    end,

    getNbrFront = function( self )
      local nbrEchelon = myself.taskParams.echelonNumber or 0
      if nbrEchelon == NIL or nbrEchelon == 0 then
        nbrEchelon = 2 -- default value
      end
      return integration.query.getNbrFront( nbrEchelon )
    end,

    getTotalRecce = function( self )
        return false
    end,
}
