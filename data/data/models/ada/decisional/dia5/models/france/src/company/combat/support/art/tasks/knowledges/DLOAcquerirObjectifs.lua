return
{

    init = function( self, params )
        myself.leadData.deployPositionIndex = 0 
        myself.leadData.zonesAcquisition = {}
        myself.leadData.positionIndex = 0
        myself.leadData.entityIndex = 0 
    end,
    
    getPosition = function ( self, params )
        myself.leadData.positionIndex =  myself.leadData.positionIndex % #params.positionsDeploiement + 1
        return params.positionsDeploiement[myself.leadData.positionIndex]
    end,
    
    getZone = function (self, params)
        local nbFront = math.min(self:getNbrFront(), params.maxNbrFront )
        for i=1, #params.zonesAcquisition do
            local objective = params.zonesAcquisition[i]
            -- division de la zone en sous-zones
            local nbParts = math.max((nbFront /#params.zonesAcquisition),1)
            local subAreas = integration.geometrySplitLocalisation( objective.source, nbParts, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction integration.geometrySplitLocalisation
            subAreas = subAreas.first
            for _, area in pairs( subAreas ) do
                myself.leadData.zonesAcquisition[#myself.leadData.zonesAcquisition + 1] = CreateKnowledge( world.Area, area )
            end
            if #subAreas == 0 then
                myself.leadData.zonesAcquisition[#myself.leadData.zonesAcquisition + 1] = objective -- cas ou la zone est hors limite
            end
        end
        myself.leadData.deployPositionIndex = myself.leadData.deployPositionIndex % #myself.leadData.zonesAcquisition + 1
        return myself.leadData.zonesAcquisition[ myself.leadData.deployPositionIndex ]
    end,
    
    getEntitiesToSupport = function (self, params)
        local entitiesToSupport = {}
        for i = 1, #params.compagniesAAppuyer do
            local entitiesToSupportFromAutomat
            local entitiesFromAutomat = integration.getEntitiesFromAutomatCommunication( params.compagniesAAppuyer[i], "none", true)
            if params.bretrogradeContext then --marche retrograde on suit le 2nd echelon sinon on suit le 1er
                entitiesToSupportFromAutomat = integration.filterPionWithEchelon( entitiesFromAutomat, eEtatEchelon_Second )
            else
                entitiesToSupportFromAutomat = integration.filterPionWithEchelon( entitiesFromAutomat, eEtatEchelon_First )
            end
            entitiesToSupport = fusionList( entitiesToSupport, entitiesToSupportFromAutomat )
        end

        return entitiesToSupport
    end,
    
    getEntity = function ( self, params )
        myself.leadData.entityIndex =  myself.leadData.entityIndex % #params.compagniesAAppuyer + 1
        return params.compagniesAAppuyer[myself.leadData.entityIndex]
    end,
}
