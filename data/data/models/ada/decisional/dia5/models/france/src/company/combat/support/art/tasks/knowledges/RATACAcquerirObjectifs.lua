return
{

    init = function( self, params )
        myself.leadData.deployPositionIndex = 0 
        myself.leadData.zonesAObserver = {}
        myself.leadData.positionIndex = 0 
    end,
    
    getPosition = function ( self, params )
        myself.leadData.positionIndex =  myself.leadData.positionIndex % #params.positionsDeploiement + 1
        return params.positionsDeploiement[myself.leadData.positionIndex]
    end,
    
    getZone = function (self, params)
        local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
        for i=1, #params.zonesAObserver do
            local objective = params.zonesAObserver[i]
            -- division de la zone en sous-zones
            local nbParts = math.max((nbFront /#params.zonesAObserver),1)
            local subAreas = integration.geometrySplitLocalisation( objective.source, nbParts, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction integration.geometrySplitLocalisation
            subAreas = subAreas.first
            for _, area in pairs( subAreas ) do
                myself.leadData.zonesAObserver[#myself.leadData.zonesAObserver + 1] = CreateKnowledge( world.Area, area )
            end
            if #subAreas == 0 then
                myself.leadData.zonesAObserver[#myself.leadData.zonesAObserver + 1] = objective -- cas ou la zone est hors limite
            end
        end
        myself.leadData.deployPositionIndex = myself.leadData.deployPositionIndex % #myself.leadData.zonesAObserver + 1
        return myself.leadData.zonesAObserver[ myself.leadData.deployPositionIndex ]
    end,
}
