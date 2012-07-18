return
{
 
    getObjectivesPei = function( self, params, entity, objectif )
        if not myself.leadData.objectivesIndex then
            myself.leadData.objectivesIndex = 0 
        end
        myself.leadData.objectivesIndex = myself.leadData.objectivesIndex % #params.positions + 1
        return { params.positions[myself.leadData.objectivesIndex] }
    end,  
    
    getDirection = function( self, params, entity, objectif )     
    
        if not myself.leadData.directionIndex then
            myself.leadData.directionIndex = 0 
        end

        local function compare( e1,e2 )
            return ( e1.perceptionLevel < e2.perceptionLevel )
        end
  
        -- On range par ordre croissant de perception par rapport à l'objectif       
       local ptrTable = {}
        for _, objective in pairs( params.objectives ) do

            local percLevel = integration.distance( objective, objectif ) 
            ptrTable[ #ptrTable +1 ] = { objective = objective,
                                         perceptionLevel = percLevel }                   
        end
        table.sort( ptrTable, compare ) 
     
        myself.leadData.currentPosition =  myself.leadData.currentPosition % #params.objectives + 1          
        local current = myself.leadData.currentPosition
             
        return params.objectives[myself.leadData.currentPosition]   
    
    end, 
    
    getObjectives = function( self, params, entity )
        return params.objectives 
    end,

    getObjective = function( self, params )
        return { params.objective }
    end,
    
    -- Affectation de positions aux pions par rapport à l'objectif à Barrer: si le nombre d'objectif <= nombre de pion , tout objectif est couvert par au moins un pion
    -- chaque pion reçoit la première position la plus efficace pour observer l'objectif + les autres positions si elles ne sont pas les plus efficaces pour les autres pions
    -- exemple:  3 pions et 6 positions {p1, p2, p3, p4, p5, p6} rangés pas ordre de proximité sur l'objectif ( à voir s'il faut les ranger par ordre de meilleure observaiton sur l'objectif: integration.getEstimatePerceptionForPion)
    -- Pion 1 recoit: p1, p4, p5, p6
    -- Pion 2 recoit: p2, p4, p5, p6
    -- Pion 3 recoit: p3, p4, p5, p6
 
     getPositions = function( self, params, entity, objectif )
        if not myself.leadData.currentPosition then
            myself.leadData.currentPosition = 0 
        end
        local i 
        local nbrFront = params.maxNbrFront
    
        local function compare( e1,e2 )
            return ( e1.perceptionLevel < e2.perceptionLevel )
        end
  
        -- On range par ordre croissant de perception par rapport à l''objectif       
       local ptrTable = {}
        for _, position in pairs( params.positions ) do

            local percLevel = integration.distance( position, objectif ) 
            ptrTable[ #ptrTable +1 ] = { position = position,
                                         perceptionLevel = percLevel }                   
        end
        table.sort( ptrTable, compare ) 
     
        local min = math.min(nbrFront ,#params.positions )
        myself.leadData.currentPosition =  myself.leadData.currentPosition % min + 1          
        local current = myself.leadData.currentPosition
             
        local ptrPosForEntity = {}
        ptrPosForEntity [ #ptrPosForEntity + 1 ] = ptrTable[ current ].position
        for i = nbrFront + 1, #ptrTable do
            ptrPosForEntity [ #ptrPosForEntity + 1 ] = ptrTable[ i ].position
        end
        
        return ptrPosForEntity
     end,


    getPosition = function( self, params )
        return { params.position }
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
        return { params.obstacles[myself.leadData.obstaclesIndex] }
    end,

    getWithImprovement = function( self, params )
        return params.withImprovement or false
    end,
}