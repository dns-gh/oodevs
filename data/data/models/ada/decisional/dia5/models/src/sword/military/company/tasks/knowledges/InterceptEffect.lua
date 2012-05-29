return
{
    init = function( self, params )        
        myself.leadData.observingIndex = 0
        myself.leadData.observingPositions = {}
    end,
    
    getReachable = function( self, params )
        return params.objective
    end,

    getObjectives = function( self, params )
        return { params.objective }
    end,

    getEntities = function( self, params )
        return params.entities
    end,

    getPeiObjectives = function( self, params )
        return { params.objective }
    end,

    getObserving = function( self, params )
        if params.meetingPoints and params.meetingPoints ~= NIL then
            myself.leadData.observingIndex = myself.leadData.observingIndex % #params.meetingPoints + 1
            return { params.meetingPoints[myself.leadData.observingIndex] }
        else
            if myself.leadData.observingIndex == 0 then
                -- urban environnement, get all the BU in circle around elements to destroy
                if #myself.leadData.observingPositions < 1 then
                    local radius = 150 -- in meters. Defined the radius of circle in witch we need to find terrain poisition to destroy elements
                    local elementPosition = params.objective:getPosition()
                    if elementPosition then
                        local blocksInCircle = DEC_Connaissances_BlocUrbainDansCercle( elementPosition, radius )
                        for _, block in pairs( blocksInCircle ) do
                            myself.leadData.observingPositions[ #myself.leadData.observingPositions + 1 ] = CreateKnowledge( sword.military.world.UrbanBlock, block )
                        end
                    end
                end
                -- Area case, add positions in the area if needed
                local nbParts = math.min( self:getNbrFront(), params.maxNbrFront ) - #myself.leadData.observingPositions
                if nbParts > 0 then
                    if masalife.brain.core.class.isOfType( params.objective, sword.military.world.Area) then
                        local subAreas = DEC_Geometry_SplitLocalisation( params.objective.source, nbParts, nil )
                        for _, subArea in pairs( subAreas.first ) do
                            local pos = DEC_Geometrie_CalculerBarycentreLocalisation( subArea )
                            myself.leadData.observingPositions[ #myself.leadData.observingPositions + 1 ] = CreateKnowledge( sword.military.world.Point, pos )
                        end
                    end
                end   
            end
            myself.leadData.observingIndex = myself.leadData.observingIndex % #myself.leadData.observingPositions + 1 
            return { myself.leadData.observingPositions[myself.leadData.observingIndex] }
        end
    end,
    
    getMeetingPoint = function( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
            return { params.meetingPoint } 
        end
        return NIL
    end,
    
    getObstacles = function( self, params )
        if not params.obstacles then
            return NIL
        end
        if not myself.leadData.obstaclesIndex then
            myself.leadData.obstaclesIndex = 0 
        end
      
        myself.leadData.obstaclesIndex = myself.leadData.obstaclesIndex + 1
        if myself.leadData.obstaclesIndex > #params.obstacles then
            return NIL
        end   
        return { params.obstacles[myself.leadData.obstaclesIndex] }
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
    
    getNbrFront = function( self )
      local nbrEchelon = myself.taskParams.echelonNumber or 0
      if nbrEchelon == 0 then
        nbrEchelon = 2 -- default value
      end
      return integration.query.getNbrFront( nbrEchelon )
    end
}
