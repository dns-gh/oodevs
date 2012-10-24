return
{
    init = function( self, params )
        myself.leadData.objectiveIndex = 0 
        myself.leadData.entities = {}
        myself.leadData.nbObj = #myself.taskParams.entities
        myself.leadData.done = false
        self.nbAreas = 0
        for _, entity in pairs( myself.taskParams.entities ) do
            if masalife.brain.core.class.isOfType( entity, world.Area) then
                self.nbAreas = self.nbAreas + 1 
            end
        end
    end,

    getEntities = function( self, params )
        local nbFront = self:getNbrFront()
        if not myself.leadData.done then
            for i=1, #params.entities do
                local entity = params.entities[i]
                -- division de la zone en sous-zones
                if masalife.brain.core.class.isOfType( entity, world.Area) then
                    local nbOtherObjectives = #params.entities - self.nbAreas
                    local nbParts = math.max((( nbFront - nbOtherObjectives )/self.nbAreas),1)
                    local subAreas = integration.geometrySplitLocalisation( entity.source, nbParts, nil ) -- TODO: voir la fonction integration.splitArea pour remplacer la fonction integration.geometrySplitLocalisation
                    subAreas = subAreas.first
                    for _, area in pairs( subAreas ) do
                        myself.leadData.entities[#myself.leadData.entities + 1] = CreateKnowledge( world.Area, area )
                    end
                    if #subAreas == 0 then
                        myself.leadData.entities[#myself.leadData.entities + 1] = entity -- cas ou la zone est hors limite
                    end
                    myself.leadData.nbObj = myself.leadData.nbObj + #subAreas - 1
                else
                    myself.leadData.entities[#myself.leadData.entities + 1] = entity
                end
            end
            myself.leadData.done = true
        end
        local nextObjectives = {}
        myself.leadData.nbPions = myself.leadData.nbPions or nbFront
        local nbcible = math.max( ( myself.leadData.nbObj / myself.leadData.nbPions ), 1 )
        while #nextObjectives < nbcible do
            myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #myself.leadData.entities + 1
            nextObjectives[ #nextObjectives + 1 ]= myself.leadData.entities[ myself.leadData.objectiveIndex ]
            local nextObjective = myself.leadData.entities[ myself.leadData.objectiveIndex ]
        end
        myself.leadData.nbPions = myself.leadData.nbPions - 1
        myself.leadData.nbObj = myself.leadData.nbObj - #nextObjectives
        return nextObjectives
    end,
}