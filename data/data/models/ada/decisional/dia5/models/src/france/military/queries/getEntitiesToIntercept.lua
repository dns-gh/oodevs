queryImplementation "getEntitiesToIntercept" { ["execute"] = function ( params )
     local entities = {}
     local enemies = integration.getDestroyableInObjective ( params.objective )
     local realEnemies = {}
     if myself.taskParams.entities and #myself.taskParams.entities ~= 0 then
        for i, enemyKnowledge in pairs( myself.taskParams.entities ) do
            realEnemies[integration.getAgentIdFromKnowledge( enemyKnowledge )] = true
        end
     end
     
           -- Mission Harceler (ne prend pas d'unité en parmètre et harcèle tous les ennemis qui rentre dans la zone)
      if not myself.taskParams.entities or #myself.taskParams.entities == 0 then
           entities = enemies
      else -- Mission Intercepter : on ne tire que sur l'ennemi renseigné par l'utilisateur mais uniquement quand il est dans la zone
          -- Controle magique de l'objectif qui simule l'effet de patrouiller
           for _, element in pairs( enemies ) do
                local realEnemyId = integration.getAgentIdFromKnowledge( element )
                if realEnemies[ realEnemyId ] then
                    if element:isOperational() and 
                    ( ( masalife.brain.core.class.isOfType( params.objective,sword.military.world.Area ) 
              and DEC_Geometrie_EstPointDansLocalisation( element:getPosition(), params.objective.source ) ) 
                        or ( masalife.brain.core.class.isOfType( params.objective, sword.military.world.UrbanBlock ) 
              and DEC_IsPointInUrbanBlock( element:getPosition(), params.objective.source ) ) )
                    then
                        entities[ #entities + 1 ] = element
                    end
                end
           end
      end
     return entities
end}