return
{

    getEntity = function( self, params )
      
       local entities = {}
       for i = 1, #params.entities do
           if masalife.brain.core.class.isOfType( params.entities[i], sword.military.world.Company) then
                local entitiesFromAutomat = integration.getEntitiesFromAutomat( params.entities[i], "none", true)
                for j = 1, #entitiesFromAutomat do
                    entities[#entities + 1] = entitiesFromAutomat[j]
                end
            else
                entities[#entities + 1] = params.entities[i]
            end
        end
        if not myself.leadData.objectiveIndex then
            myself.leadData.objectiveIndex = 0 
        end
        myself.leadData.objectiveIndex = myself.leadData.objectiveIndex % #entities + 1
        return entities[myself.leadData.objectiveIndex]
    end,
}