return
{
    getEntities = function( self, params )
        if not myself.leadData.entitiesIndex then
            myself.leadData.entitiesIndex = 0
        end
        myself.leadData.entitiesIndex = myself.leadData.entitiesIndex % #params.entities + 1
        return { params.entities[myself.leadData.entitiesIndex] }
    end,
    
    -- point du stationnement
    getReachable = function( self, params )
        if params.meetingPoint and params.meetingPoint ~= NIL then
           return integration.randomPositionOnCircle( params.meetingPoint, 100 )
        end
        return params.entities[ 1 ] -- Le PC va vers 1 des unités à défendre si pas de position donnée par l'opérateur
    end,
}
