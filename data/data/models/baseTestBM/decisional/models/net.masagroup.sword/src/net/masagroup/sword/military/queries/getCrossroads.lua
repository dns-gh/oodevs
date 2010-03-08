queryImplementation "getCrossroads" { ["execute"] = function ( params )
    local allRes = knowledgeManager.getQueryResult( "getCrossroads" )  
    
    if knowledgeManager.bCanCallStaticQuery or params.dynamic then
      for _, objective in pairs( params.objectives ) do
          local res = {}
          DEC_Crossroads( objective:getPosition(), 5000, res )
          if objective:isOfType( net.masagroup.military.generic.knowledge.Reaching ) then
            res[ #res + 1 ] = objective -- add objectif itself
          end
          allRes[ objective ] = res
      end
    end
    
    return allRes
end}