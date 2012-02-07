return
{
-- Partage des objectifs: chaque pion reçoit un objectif
getControlCrowdArea = function( self, params )
    return params.area
end,

getMeetingPoint = function( self, params )
    -- le PC va au centre de la premiere zone
    return { params.area:getMyPosition() }
end,
}